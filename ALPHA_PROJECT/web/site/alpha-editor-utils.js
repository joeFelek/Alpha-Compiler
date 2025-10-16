// Editor helpers + example loaders
(function () {

    // Replace "width()" and "height()" functions with the terminal's current width and height 
    function adjustConsoleResolution(code) {
        code = code.replace(/(?<![\w$])width\s*\(\s*\)(?![\w$])/g, String(window.Alpha.Term.term.cols));
        code = code.replace(/(?<![\w$])height\s*\(\s*\)(?![\w$])/g, String(window.Alpha.Term.term.rows - 2));
        return code;
    }

    function putInEditor(code) {
        if (!code) return;
        window.editor.setValue(code);
        window.Alpha.Diagnostics.clearAlphaMarkers();
        window.editor.setScrollPosition({ scrollTop: 0 });
        window.editor.setPosition({ lineNumber: 1, column: 1 });
    }

    async function loadExample(url) {
        const res = await fetch(url + '?cb=' + Date.now());
        const code = await res.text();
        putInEditor(code);
    }
    
    window.Alpha = window.Alpha || {};
    window.Alpha.Utils = { putInEditor, adjustConsoleResolution, loadExample };
})();
