// App wiring: UI, compile, vm worker
(function () {
    const $ = (sel) => document.querySelector(sel);
    const { term, termLayout, showCursor, hideCursor } = window.Alpha.Term;
    const { setAlphaMarkers, clearAlphaMarkers, parseDiagnostics } = window.Alpha.Diagnostics;
    const { adjustConsoleResolution, loadExample } = window.Alpha.Utils;

    const GREY = '\x1b[38;5;247m';
    const RESET = '\x1b[0m';

    /* ---------- Create Editor ---------- */
    // Load stored editor's code
    const savedCode = localStorage.getItem('alpha:editorCode');

    window.editor = monaco.editor.create($('#editor'), {
        value: savedCode ? savedCode : '',
        language: 'alpha',
        theme: 'alpha-one-dark',
        automaticLayout: true,
        fontSize: 14,
        minimap: { enabled: true },
        autoIndent: 'full',
        autoClosingBrackets: 'languageDefined',
        mouseWheelZoom: true,
    });

    if (savedCode === null) loadExample('examples/introduction.al');

    // Store editor's current code &
    // Remove diagnostic error/warning if the line is edited
    window.editor.onDidChangeModelContent((e) => {
        localStorage.setItem('alpha:editorCode', window.editor.getValue());

        const diags = window.Alpha.Diagnostics.currentDiags;
        if (!diags.length) return;

        const touched = new Set();
        for (const ch of e.changes) {
            for (let ln = ch.range.startLineNumber; ln <= ch.range.endLineNumber; ln++) touched.add(ln);
        }

        const filtered = diags.filter(d => !touched.has(d.line));
        if (filtered.length !== diags.length) setAlphaMarkers(filtered, { jumpNow: false });
    });

    /* --- Editor & Terminal Relayout --- */
    function relayout() {
        window.editor.layout();
        termLayout();
    }

    let rId = 0;
    window.addEventListener('resize', () => {
        cancelAnimationFrame(rId);
        rId = requestAnimationFrame(relayout);
    });
    setTimeout(relayout, 0);

    /* ---------- VM Worker ---------- */
    let vmWorker = null;

    function initWorker() {
        return new Promise((resolve) => {
            const url = new URL('./site/vm-worker.js', window.location.href);
            url.searchParams.set('cb', Date.now().toString());
            vmWorker = new Worker(url.toString());

            let capturedStderr = '';

            vmWorker.onmessage = (e) => {
                const { type, text } = e.data;
                if (type === 'stdout') term.writeln(text);
                if (type === 'stderr') { capturedStderr += (text || '') + '\n'; term.writeln(text); }
                if (type === 'ready') resolve();
                if (type === 'done') term.writeln(GREY + '[vm] Execution finished' + RESET);
                if (type === 'failed') term.writeln(GREY + '[vm] Execution failed' + RESET);
                if (type === 'done' || type === 'failed' || type === 'memory') {
                    terminateWorker();
                    btnRun.textContent = 'Run';
                    btnRun.classList.remove('btn-stop');
                    btnRun.classList.add('btn-primary');
                    showCursor();
                    const diags = parseDiagnostics(capturedStderr);
                    if (diags.length) setAlphaMarkers(diags, { jumpNow: true });
                    if (type === 'memory') term.writeln(GREY + '[vm] Out of memory' + RESET);
                }
            };

            vmWorker.postMessage({ type: 'init' });
        });
    }

    function terminateWorker() {
        if (vmWorker) vmWorker.terminate();
        vmWorker = null;
    }

    async function restartWorker() {
        terminateWorker();
        await initWorker();
    }

    /* ---------- Compile on Run ---------- */
    async function compileSource() {
        let capturedStderr = '';
        const compiler = await CompilerModule({
            print: (t) => term.writeln(t),
            printErr: (t) => { capturedStderr += (t || '') + '\n'; term.writeln(t); },
            stdin: () => { },
        });

        let code = window.editor.getValue();
        code = adjustConsoleResolution(code);
        clearAlphaMarkers();

        compiler.FS.writeFile('alpha.al', code);
        const exitCode = compiler.callMain(['alpha.al']);

        const diags = parseDiagnostics(capturedStderr);
        if (diags.length) setAlphaMarkers(diags, { jumpNow: true });

        if (exitCode !== 0) {
            term.writeln(GREY + '[compiler] Compilation failed' + RESET);
            try { compiler.FS.unlink('alpha.abc'); } catch (_) { }
            if (vmWorker) vmWorker.terminate();
            showCursor();
            return null;
        }
        return compiler.FS.readFile('alpha.abc');
    }

    /* ---------- Buttons ---------- */
    const btnRun = $('#btn-run');
    const btnClear = $('#btn-clear');
    const btnExSyntax = $('#btn-ex-syntax');
    const btnExLife = $('#btn-ex-life');
    const btnExDonut = $('#btn-ex-donut');
    const tglAutoClear = $('#toggle-autoclear');

    let autoClear = true;

    btnRun.addEventListener('click', async () => {
        if (vmWorker) {
            showCursor();
            terminateWorker();
            term.writeln(GREY + '[vm] Execution stopped.' + RESET);
            btnRun.textContent = 'Run';
            btnRun.classList.remove('btn-stop');
            btnRun.classList.add('btn-primary');
            return;
        }

        try {
            if (autoClear) term.write('\x1b[2J\x1b[3J\x1b[H');
            const byteCode = await compileSource();
            if (byteCode === null) return;

            btnRun.textContent = 'Stop';
            btnRun.classList.remove('btn-primary');
            btnRun.classList.add('btn-stop');
            hideCursor();

            await restartWorker();
            vmWorker.postMessage({ type: 'run', byteCode });
        } catch (e) {
            console.error(e);
            btnRun.textContent = 'Run';
            btnRun.classList.remove('btn-stop');
            btnRun.classList.add('btn-primary');
        }
    });

    btnExSyntax.addEventListener('click', () => loadExample('examples/syntax.al'));
    btnExLife.addEventListener('click', () => loadExample('examples/game_of_life.al'));
    btnExDonut.addEventListener('click', () => loadExample('examples/donut.al'));

    btnClear.addEventListener('click', () => {
        term.write('\x1b[2J\x1b[3J\x1b[H');
        clearAlphaMarkers();
    });

    // Load stored auto clear toggle
    const savedAC = localStorage.getItem('alpha:autoClear');
    if (savedAC !== null) autoClear = savedAC === '1';
    tglAutoClear.checked = autoClear;

    tglAutoClear.addEventListener('change', () => {
        autoClear = tglAutoClear.checked;
        localStorage.setItem('alpha:autoClear', autoClear ? '1' : '0');
    });

})();
