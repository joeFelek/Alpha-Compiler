// Editor helpers + example loaders
(function () {
    const { term } = window.Alpha.Term;
    const { clearAlphaMarkers } = window.Alpha.Diagnostics;

    // Replace "width()" and "height()" functions with the terminal's current width and height 
    function adjustConsoleResolution(code) {
        code = code.replace(/(?<![\w$])width\s*\(\s*\)(?![\w$])/g, String(term.cols));
        code = code.replace(/(?<![\w$])height\s*\(\s*\)(?![\w$])/g, String(term.rows - 2));
        return code;
    }

    function putInEditor(code) {
        if (!code) return;
        window.editor.setValue(code);
        clearAlphaMarkers();
        window.editor.setScrollPosition({ scrollTop: 0 });
        window.editor.setPosition({ lineNumber: 1, column: 1 });
        window.editor.focus();
    }

    async function loadExample(url) {
        const res = await fetch(url + '?cb=' + Date.now());
        code = await res.text();
        putInEditor(code);
    }
    
    window.Alpha = window.Alpha || {};
    window.Alpha.Utils = { putInEditor, adjustConsoleResolution, loadExample };
})();
