// NOTE: loaded after monaco in index.js; uses global `window.editor`
const $ = (sel) => document.querySelector(sel);

/* ---------- Terminal (xterm.js) ---------- */
const GREY = '\x1b[38;5;247m';
const RESET = '\x1b[0m';

const term = new Terminal({
    convertEol: true,
    scrollback: 1000,
    fontFamily: 'ui-monospace, SFMono-Regular, Menlo, Consolas, monospace',
    fontSize: 14,
});
const fitAddon = new FitAddon.FitAddon();
term.loadAddon(fitAddon);
term.open($('#term'));
const showCursor = () => term.write('\x1b[?25h');
const hideCursor = () => term.write('\x1b[?25l');

/* ---------- Layout / Resize ---------- */
let rId = 0;
function relayout() {
    try { if (window.editor) window.editor.layout(); } catch { }
    try { fitAddon.fit(); } catch { }
}
window.addEventListener('resize', () => {
    cancelAnimationFrame(rId);
    rId = requestAnimationFrame(relayout);
});
// First layout pass after initial paint
setTimeout(relayout, 0);

/* ---------- Simple writers (strings only) ---------- */
const log = (s = '') => term.write(s);
const logLn = (s = '') => term.writeln(s);
const logErr = (s = '') => term.write(s);

/* ---------- Diagnostics (Monaco) ---------- */
const STRIP_ANSI_RE = /\x1B\[[0-9;]*m/g;
const stripAnsi = (s) => (s || '').replace(STRIP_ANSI_RE, '');
let currentDiags = [];

function firstCodeColumn(model, lineNumber) {
    const ln = Math.max(1, Math.min(lineNumber, model.getLineCount()));
    const text = model.getLineContent(ln);
    const m = /^\s*/.exec(text);
    return (m ? m[0].length : 0) + 1;
}

function lastCodeColumn(model, lineNumber) {
    const ln = Math.max(1, Math.min(lineNumber, model.getLineCount()));
    const text = model.getLineContent(ln);

    let inStr = false, esc = false, cut = -1;
    for (let i = 0; i < text.length; i++) {
        const ch = text[i];
        if (inStr) {
            if (esc) esc = false;
            else if (ch === '\\') esc = true;
            else if (ch === '"') inStr = false;
        } else {
            if (ch === '"') inStr = true;
            else if (ch === '/' && i + 1 < text.length) {
                const nx = text[i + 1];
                if (nx === '/' || nx === '*') { cut = i; break; }
            }
        }
    }

    let end = (cut >= 0 ? cut : text.length);
    while (end > 0 && /\s/.test(text[end - 1])) end--;
    return Math.max(1, end + 1);
}

function setAlphaMarkers(diags, { jumpNow = false } = {}) {
    currentDiags = diags.slice();
    const model = window.editor.getModel();

    const markers = currentDiags.map(d => {
        const line = d.line || 1;
        const start = firstCodeColumn(model, line);
        let end = lastCodeColumn(model, line);
        if (end < start) end = start;
        return {
            message: d.msg,
            severity: d.kind === 'warning' ? monaco.MarkerSeverity.Warning : monaco.MarkerSeverity.Error,
            startLineNumber: line,
            startColumn: start,
            endLineNumber: line,
            endColumn: end
        };
    });

    monaco.editor.setModelMarkers(model, 'alpha-diagnostics', markers);

    if (jumpNow) {
        const firstErr = currentDiags.find(d => d.kind === 'error');
        if (firstErr?.line) {
            window.editor.revealLineInCenter(firstErr.line);
            window.editor.setPosition({ lineNumber: firstErr.line, column: firstCodeColumn(model, firstErr.line) });
            window.editor.focus();
        }
    }
}

function clearAlphaMarkers() {
    currentDiags = [];
    monaco.editor.setModelMarkers(window.editor.getModel(), 'alpha-diagnostics', []);
}

function parseDiagnostics(stderrText) {
    const clean = stripAnsi(stderrText);
    const diags = [];
    const re = /^\s*(\d+)\s*:\s*(error|warning)\s*:\s*(.+)$/i;
    for (const raw of clean.split(/\r?\n/)) {
        const line = raw.trim();
        const m = re.exec(line);
        if (!m) continue;
        diags.push({
            line: parseInt(m[1], 10) || 1,
            kind: m[2].toLowerCase() === 'warning' ? 'warning' : 'error',
            msg: m[3].trim()
        });
    }
    return diags;
}

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

    const code = window.editor.getValue();
    clearAlphaMarkers();

    compiler.FS.writeFile('program.al', code);
    const exitCode = compiler.callMain(['program.al']);

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

/* ---------- Clear markers on edited lines only ---------- */
window.editor.onDidChangeModelContent((e) => {
    localStorage.setItem('alpha:editorCode', window.editor.getValue());

    if (!currentDiags.length) return;

    const touched = new Set();
    for (const ch of e.changes) {
        for (let ln = ch.range.startLineNumber; ln <= ch.range.endLineNumber; ln++) {
            touched.add(ln);
        }
    }

    const filtered = currentDiags.filter(d => !touched.has(d.line));
    if (filtered.length !== currentDiags.length) {
        setAlphaMarkers(filtered, { jumpNow: false });
    }
});

/* ---------- Buttons ---------- */
const btnRun = $('#btn-run');
const btnClear = $('#btn-clear');
const btnExSyntax = $('#btn-ex-syntax');
const btnExLife = $('#btn-ex-life');
const btnExDonut = $('#btn-ex-donut');
const tglAutoClear = $('#toggle-autoclear');

let autoClear = true;

function putInEditor(code) {
    if (!code) return;
    window.editor.setValue(code);
    clearAlphaMarkers();
    window.editor.setScrollPosition({ scrollTop: 0 });
    window.editor.setPosition({ lineNumber: 1, column: 1 });
    window.editor.focus();
}

function adjustExampleResolution(code) {
    // xterm.js exposes current geometry
    const rows = (typeof term !== "undefined" && term && term.rows) ? term.rows : 24;
    const cols = (typeof term !== "undefined" && term && term.cols) ? term.cols : 80;

    // Alpha draws each cell as "# " or "  " -> 2 columns per cell
    // You asked: H = (xterm rows - 1)
    const H = rows - 2;

    // Width: fit in available columns (2 chars per cell), leave a tiny safety margin
    const W = Math.floor(cols / 2);

    // Replace the W = <num>; and H = <num>; lines in the Alpha source
    let out = code;
    out = out.replace(/^\s*W\s*=\s*\d+\s*;/m, `W = ${W};`);
    out = out.replace(/^\s*H\s*=\s*\d+\s*;/m, `H = ${H};`);

    return out;
}

async function loadExample(url) {
    const res = await fetch(url + '?cb=' + Date.now());
    let code = await res.text();
    if (!url.includes('syntax.al')) {
        code = adjustExampleResolution(code);
    }
    putInEditor(code);
    requestAnimationFrame(relayout);
}

btnExSyntax.addEventListener('click', () => loadExample('examples/syntax.al'));
btnExLife.addEventListener('click', () => loadExample('examples/game_of_life.al'));
btnExDonut.addEventListener('click', () => loadExample('examples/donut.al'));

btnRun.addEventListener('click', async () => {
    if (vmWorker) {
        // Already running → stop it
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
        
        // Change to stop state (red)
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

btnClear.addEventListener('click', () => {
    term.write('\x1b[2J\x1b[3J\x1b[H'); // clear + home
    clearAlphaMarkers();
});

const savedAC = localStorage.getItem('alpha:autoClear');
if (savedAC !== null) {
    autoClear = savedAC === '1';
}
tglAutoClear.checked = autoClear;

tglAutoClear.addEventListener('change', () => {
    autoClear = tglAutoClear.checked;
    localStorage.setItem('alpha:autoClear', autoClear ? '1' : '0');
});

const savedCode = localStorage.getItem('alpha:editorCode');
if (savedCode !== null) {
    window.editor.setValue(savedCode);
}
