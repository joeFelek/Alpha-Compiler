// Terminal setup
(function () {
    const $ = (sel) => document.querySelector(sel);

    const term = new Terminal({
        convertEol: true,
        scrollback: 1000,
        fontFamily: 'ui-monospace, SFMono-Regular, Menlo, Consolas, monospace',
        fontSize: 14,
    });

    const showCursor = () => term.write('\x1b[?25h');
    const hideCursor = () => term.write('\x1b[?25l');

    const fitAddon = new FitAddon.FitAddon();
    term.loadAddon(fitAddon);
    term.open($('#term'));

    const termLayout = () => fitAddon.fit();

    const log = (s = '') => term.write(s);
    const logLn = (s = '') => term.writeln(s);
    const logErr = (s = '') => term.write(s);

    window.Alpha = window.Alpha || {};
    window.Alpha.Term = { term, termLayout, showCursor, hideCursor, log, logLn, logErr };
})();
