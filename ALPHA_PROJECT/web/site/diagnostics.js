// Monaco diagnostics helpers
(function () {
    const STRIP_ANSI_RE = /\x1B\[[0-9;]*m/g;
    const stripAnsi = (s) => (s || '').replace(STRIP_ANSI_RE, '');

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

    let currentDiags = [];

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
                startLineNumber: line, startColumn: start,
                endLineNumber: line, endColumn: end
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

    window.Alpha = window.Alpha || {};
    window.Alpha.Diagnostics = { setAlphaMarkers, clearAlphaMarkers, parseDiagnostics, get currentDiags() { return currentDiags; } };
})();
