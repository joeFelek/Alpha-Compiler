const $ = (sel) => document.querySelector(sel);
const outEl = $('#output');

// ANSI coloring
const ansi_up = new AnsiUp();
ansi_up.use_classes = false;

function appendAnsi(text) {
    const html = ansi_up.ansi_to_html(text);
    outEl.innerHTML += html + '<br>';
    outEl.scrollTop = outEl.scrollHeight;
}

const log = (msg = '') => appendAnsi(msg);
const logErr = (msg = '') => appendAnsi(msg);

let vm = null;

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

async function initializeVM(byteCode) {
    vm = await AlphaModule({
        print: log,
        printErr: logErr,
    });
    vm.FS.writeFile('alpha.abc', byteCode);
}

async function runBytecode() {
    const exitCode = vm.callMain(['alpha.abc']);
    vm.FS.unlink('alpha.abc');
    if (exitCode !== 0) {
        logErr(`[vm] Runtime failed`);
    }
}

$('#btn-run').addEventListener('click', async () => {
    outEl.innerHTML = '';
    try {
        const byteCode = await compileSource();
        if (byteCode === null) return;
        await initializeVM(byteCode);
        await runBytecode();
    } catch (e) {
        console.error(e);
    }
});

$('#btn-clear').addEventListener('click', () => { outEl.innerHTML = ''; });

