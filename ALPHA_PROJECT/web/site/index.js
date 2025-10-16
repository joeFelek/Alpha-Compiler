require.config({ paths: { vs: 'https://cdn.jsdelivr.net/npm/monaco-editor@0.44.0/min/vs' } });

require(['vs/editor/editor.main'], function () {
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
    await Promise.all([
      loadScript('site/diagnostics.js'),
      loadScript('site/terminal.js'),
      loadScript('site/alpha-editor-utils.js'),
      loadScript('site/alpha-editor.js'),
    ]);
    await loadScript('site/main.js');

    window.define = oldDefine;
  })().catch(err => {
    window.define = oldDefine;
    console.error(err);
  });
});
