const $ = (sel) => document.querySelector(sel);
const consoleEl = $('#output');

function clearOutput() {
    consoleEl.innerHTML = '';
}

/************* Ansi output  **************/
const GREY = '\x1b[38;5;247m';
const RESET = '\x1b[0m';

const ansi_up = new AnsiUp();
ansi_up.use_classes = false;

function appendAnsi(text) {
    const html = ansi_up.ansi_to_html(text);
    consoleEl.innerHTML += html + '<br>';
    consoleEl.scrollTop = consoleEl.scrollHeight;
}

const log = (msg = '') => appendAnsi(msg);
const logErr = (msg = '') => appendAnsi(msg);
/*****************************************/

/*************** Worker *****************/
let vmWorker = null;

async function compileSource() {
    const compiler = await CompilerModule({
        print: log,
        printErr: logErr,
        stdin: () => { }
    });

    const code = editor.getValue();
    compiler.FS.writeFile('program.al', code);
    const exitCode = compiler.callMain(['program.al']);
    if (exitCode !== 0) {
        logErr(GREY + '[compiler] Compilation failed' + RESET);
        try { compiler.FS.unlink('alpha.abc'); } catch (_) { }
        if (vmWorker) vmWorker.terminate();
        return null;
    }
    return compiler.FS.readFile('alpha.abc');
}

function initWorker() {
    return new Promise((resolve) => {
        const url = new URL('./site/vm-worker.js', window.location.href);
        url.searchParams.set('cb', Date.now().toString());
        vmWorker = new Worker(url.toString());

        vmWorker.onmessage = (e) => {
            const { type, text } = e.data;
            if (type === 'stdout') log(text);
            if (type === 'stderr') logErr(text);
            if (type === 'ready') resolve();
            if (type === 'done') log(GREY + '[vm] Execution finished' + RESET);
            if (type === 'failed') logErr(GREY + '[vm] Execution failed' + RESET);
            if (type === 'done' || type === 'failed') terminateWorker();
        };

        vmWorker.postMessage({ type: 'init' });
    });
}

function terminateWorker() {
    if (vmWorker)
        vmWorker.terminate();
    vmWorker = null;
}

async function restartWorker() {
    terminateWorker();
    await initWorker();
}
/****************************************/

/*********** Button Events **************/

$('#btn-run').addEventListener('click', async () => {
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
        terminateWorker();
        logErr(GREY + '[vm] Execution stopped.' + RESET);
    }
});

$('#btn-clear').addEventListener('click', () => { consoleEl.innerHTML = ''; });
