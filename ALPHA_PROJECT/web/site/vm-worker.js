importScripts('../alpha.js');

let vmInstance = null;

onmessage = async (e) => {
  const { type, byteCode } = e.data;

  if (type === 'init') {
    vmInstance = await AlphaModule({
      print: (text) => postMessage({ type: 'stdout', text }),
      printErr: (text) => postMessage({ type: 'stderr', text }),
      locateFile: (path, prefix) => {
        if (path.endsWith('.wasm')) return '../' + path;
        return prefix + path;
      }
    });
    postMessage({ type: 'ready' });
  }

  if (type === 'run' && vmInstance) {
    vmInstance.FS.writeFile('alpha.abc', byteCode);
    vmInstance.callMain(['alpha.abc']);
    vmInstance.FS.unlink('alpha.abc');
    postMessage({ type: 'done' });
  }
};
