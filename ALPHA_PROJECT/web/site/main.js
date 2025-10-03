const $ = (sel) => document.querySelector(sel);
const outEl = $('#output');

const ansi_up = new AnsiUp();
ansi_up.use_classes = false;

function appendAnsi(text) {
    const html = ansi_up.ansi_to_html(text);
    outEl.innerHTML += html + '<br>';
    outEl.scrollTop = outEl.scrollHeight;
}

const log = (msg = '') => appendAnsi(msg);
const logErr = (msg = '') => appendAnsi(msg);

let vmWorker = null;

async function compileSource() {
    const compiler = await CompilerModule({
        print: log,
        printErr: logErr,
        stdin: () => {}
    });

    const code = $('#source').value;
    compiler.FS.writeFile('program.al', code);
    const exitCode = compiler.callMain(['program.al']);
    if (exitCode !== 0) {
        logErr(`[compiler] Compilation failed`);
        try { compiler.FS.unlink('alpha.abc'); } catch (_) {}
        return null;
    }
    return compiler.FS.readFile('alpha.abc');
}

function initWorker() {
    return new Promise((resolve) => {
        vmWorker = new Worker('site/vm-worker.js');

        vmWorker.onmessage = (e) => {
            const { type, text } = e.data;
            if (type === 'stdout') log(text);
            if (type === 'stderr') logErr(text);
            if (type === 'done') log('[vm] Execution finished');
            if (type === 'ready') resolve();
        };

        vmWorker.postMessage({ type: 'init' });
    });
}

async function restartWorker() {
    if (vmWorker) 
        vmWorker.terminate();
    await initWorker();  
}

$('#btn-run').addEventListener('click', async () => {
    outEl.innerHTML = '';
    try {
        const byteCode = await compileSource();
        if (byteCode === null) 
            return;

        await restartWorker();
        vmWorker.postMessage({ type: 'run', byteCode });
    } catch (e) {
        console.error(e);
    }
});

$('#btn-stop').addEventListener('click', () => {
    if (vmWorker) {
        vmWorker.terminate();
        logErr('[vm] Execution stopped.');
        vmWorker = null;
    }
});

$('#btn-clear').addEventListener('click', () => { outEl.innerHTML = ''; });
