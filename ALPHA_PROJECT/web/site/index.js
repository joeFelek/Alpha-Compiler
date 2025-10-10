require.config({ paths: { vs: 'https://cdn.jsdelivr.net/npm/monaco-editor@0.44.0/min/vs' } });

require(['vs/editor/editor.main'], function () {
    window.editor = monaco.editor.create(document.getElementById('editor'), {
        value: `// Type your Alpha program here`,
        language: 'plaintext',   // will switch to 'alpha' after it registers
        theme: 'vs-dark',
        automaticLayout: true,
        fontSize: 14,
        minimap: { enabled: true },
        autoIndent: 'full',
        autoClosingBrackets: 'languageDefined',
    });

    const oldDefine = window.define;
    window.define = undefined;

    const loadScript = (src) => new Promise((resolve, reject) => {
        const s = document.createElement('script');
        s.src = src;
        s.onload = resolve;
        s.onerror = reject;
        document.body.appendChild(s);
    });

    (async () => {
        await loadScript('alphac.js');
        await loadScript('alpha.js');
        await loadScript('site/alpha_editor.js');
        await loadScript('site/main.js');
        window.define = oldDefine;
    })().catch(err => {
        window.define = oldDefine;
        console.error(err);
    });
});
