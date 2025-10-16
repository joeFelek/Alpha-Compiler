(function () {
    if (typeof monaco === 'undefined') {
        console.error('[alpha] Monaco not loaded yet.');
        return;
    }

    const keywords = [
        'if', 'else', 'while', 'for', 'function',
        'return', 'break', 'continue', 'local'
    ];
    const wordOps = ['not', 'and', 'or'];
    const libfuncs = [
        'print', 'objectmemberkeys', 'objecttotalmembers', 'objectcopy',
        'totalarguments', 'argument', 'typeof', 'strtonum', 'sqrt', 'cos', 'sin',
        'trunc', 'random', 'sleep', 'width', 'height'
    ];
    const operators = [
        '+', '-', '*', '/', '%', '=', '==', '!=', '<', '>', '<=', '>=',
        '::', '..', '.', ',', ';', '++', '--'
    ];

    monaco.languages.register({ id: 'alpha' });

    monaco.languages.setMonarchTokensProvider('alpha', {
        defaultToken: '',
        tokenPostfix: '.alpha',
        keywords,
        wordops: wordOps,
        libfuncs,
        operators,
        symbols: /[=><!~?:&|+\-*\/\^%\.]+/,
        escapes: /\\(?:[abfnrtv\\"']|x[0-9A-Fa-f]{1,2}|[0-7]{1,3})/,

        tokenizer: {
            root: [
                { include: '@whitespace' },

                // 'function' keyword (named or anonymous)
                [/\bfunction\b/, { token: 'keyword', next: '@funcHead' }],

                // literals
                [/\b(?:true|false|nil)\b/, 'constant.language'],

                // word operators
                [/\b(?:not|and|or)\b/, 'operator'],

                // global accessor (::) stays plain
                [/::/, 'identifier'],

                // dot and dotdot, then member handling
                [/\.\.?/, { token: 'identifier', next: '@afterDot' }],

                // arrays (may contain objects)
                [/\[/, { token: '@brackets', next: '@arr' }],

                // identifier before '(' → call
                [/[a-zA-Z_]\w*(?=\s*\()/, {
                    cases: {
                        '@keywords': 'keyword',
                        '@libfuncs': 'support.function',
                        '@default': 'entity.name.function'
                    }
                }],

                // bare identifiers / keywords / libs
                [/[a-zA-Z_]\w*/, {
                    cases: {
                        '@keywords': 'keyword',
                        '@libfuncs': 'support.function',
                        '@default': 'identifier'
                    }
                }],

                // misc
                [/[{}()\[\]]/, '@brackets'],
                [/@symbols/, { cases: { '@operators': 'operator', '@default': '' } }],
                [/\d+(\.\d+)?/, 'number'],
                [/"([^"\\]|\\.)*$/, 'string.invalid'],
                [/"/, { token: 'string.quote', bracket: '@open', next: '@string' }],
            ],

            // after '.' or '..' → member (red) or method (blue)
            afterDot: [
                [/[a-zA-Z_]\w*(?=\s*\()/, {
                    cases: {
                        '@libfuncs': 'support.function',
                        '@default': 'entity.name.function'
                    }
                }],
                [/[a-zA-Z_]\w*/, { token: 'property', next: '@pop' }],
                [/[ \t\r\n]+/, 'white', '@pop'],
                ['', '', '@pop']
            ],

            // function head: name + '(' or anonymous '('
            funcHead: [
                { include: '@whitespace' },
                [/[a-zA-Z_]\w*/, { token: 'entity.name.function', next: '@funcParamsStart' }],
                [/\(/, { token: '@brackets', next: '@params' }],
                ['', '', '@pop']
            ],

            // named function: expect '(' next
            funcParamsStart: [
                { include: '@whitespace' },
                [/\(/, { token: '@brackets', next: '@params' }],
                ['', '', '@pop']
            ],

            // function params (...)
            params: [
                { include: '@whitespace' },
                [/\)/, { token: '@brackets', next: '@pop' }],

                [/\b(?:true|false|nil)\b/, 'constant.language'],
                [/\b(?:not|and|or)\b/, 'operator'],

                // dot/dotdot inside params
                [/\.\.?/, { token: 'identifier', next: '@afterDot' }],

                // nested calls
                [/[a-zA-Z_]\w*(?=\s*\()/, {
                    cases: {
                        '@keywords': 'keyword',
                        '@libfuncs': 'support.function',
                        '@default': 'entity.name.function'
                    }
                }],

                // identifiers / literals / strings / symbols
                [/[a-zA-Z_]\w*/, 'identifier'],
                [/\d+(\.\d+)?/, 'number'],
                [/"([^"\\]|\\.)*$/, 'string.invalid'],
                [/"/, { token: 'string.quote', bracket: '@open', next: '@string' }],
                [/@symbols/, { cases: { '@operators': 'operator', '@default': 'delimiter' } }],
            ],

            // array contents (allows objects/arrays)
            arr: [
                { include: '@whitespace' },
                [/\]/, { token: '@brackets', next: '@pop' }],
                [/\{/, { token: '@brackets', next: '@obj' }],
                [/\[/, { token: '@brackets', next: '@arr' }],

                [/\b(?:true|false|nil)\b/, 'constant.language'],
                [/\b(?:not|and|or)\b/, 'operator'],
                [/::/, 'identifier'],
                [/\.\.?/, { token: 'identifier', next: '@afterDot' }],
                [/[a-zA-Z_]\w*(?=\s*\()/, {
                    cases: {
                        '@keywords': 'keyword',
                        '@libfuncs': 'support.function',
                        '@default': 'entity.name.function'
                    }
                }],
                [/[a-zA-Z_]\w*/, {
                    cases: {
                        '@keywords': 'keyword',
                        '@libfuncs': 'support.function',
                        '@default': 'identifier'
                    }
                }],
                [/\d+(\.\d+)?/, 'number'],
                [/"([^"\\]|\\.)*$/, 'string.invalid'],
                [/"/, { token: 'string.quote', bracket: '@open', next: '@string' }],
                [/[{}\[\]]/, '@brackets']
            ],

            // object literal: "key": value
            obj: [
                { include: '@whitespace' },
                [/\}/, { token: '@brackets', next: '@pop' }],

                [/\"([^"\\]|\\.)*\"(?=\s*:)/, 'property'], // key
                [/:/, 'identifier'],
                [/,/, 'delimiter'],

                [/\b(?:true|false|nil)\b/, 'constant.language'],
                [/\b(?:not|and|or)\b/, 'operator'],
                [/::/, 'identifier'],
                [/\.\.?/, { token: 'identifier', next: '@afterDot' }],

                [/[a-zA-Z_]\w*(?=\s*\()/, {
                    cases: {
                        '@keywords': 'keyword',
                        '@libfuncs': 'support.function',
                        '@default': 'entity.name.function'
                    }
                }],
                [/[a-zA-Z_]\w*/, {
                    cases: {
                        '@keywords': 'keyword',
                        '@libfuncs': 'support.function',
                        '@default': 'identifier'
                    }
                }],
                [/\d+(\.\d+)?/, 'number'],
                [/"([^"\\]|\\.)*$/, 'string.invalid'],
                [/"/, { token: 'string.quote', bracket: '@open', next: '@string' }],
                [/\{/, { token: '@brackets', next: '@obj' }],
                [/\[/, { token: '@brackets', next: '@arr' }]
            ],

            // strings
            string: [
                [/\\x(?![0-9A-Fa-f])/, 'string.escape.invalid'], // bare \x
                [/@escapes/, 'string.escape'],
                [/\\./, 'string.escape.invalid'],
                [/"/, { token: 'string.quote', bracket: '@close', next: '@pop' }],
                [/[^\\"]+/, 'string'],
            ],

            // whitespace & comments
            whitespace: [
                [/[ \t\r\n]+/, 'white'],
                [/\/\*/, 'comment', '@blockComment'],
                [/\/\/.*$/, 'comment'],
            ],

            // /* ... */ (multiline)
            blockComment: [
                [/[^/*]+/, 'comment'],
                [/\/\*/, 'comment', '@push'],
                [/\*\//, 'comment', '@pop'],
                [/[\/*]/, 'comment'],
            ],
        }
    });

    monaco.languages.setLanguageConfiguration('alpha', {
        comments: { lineComment: '//', blockComment: ['/*', '*/'] },
        brackets: [['{', '}'], ['[', ']'], ['(', ')']],
        autoClosingPairs: [
            { open: '{', close: '}' },
            { open: '[', close: ']' },
            { open: '(', close: ')' },
            { open: '"', close: '"' },
            { open: '/*', close: '*/' },
        ],
        surroundingPairs: [
            { open: '{', close: '}' },
            { open: '[', close: ']' },
            { open: '(', close: ')' },
            { open: '"', close: '"' },
            { open: '/*', close: '*/' },
        ],
        onEnterRules: [
            {
                beforeText: /^\s*.*\{\s*$/,
                afterText: /^\s*\}/,
                action: {
                    indentAction: monaco.languages.IndentAction.IndentOutdent
                }
            },
            {
                beforeText: /^\s*.*\{\s*$/,
                action: {
                    indentAction: monaco.languages.IndentAction.Indent
                }
            },
            {
                beforeText: /^\s*\}/,
                action: {
                    indentAction: monaco.languages.IndentAction.Outdent
                }
            }
        ],
        indentationRules: {
            increaseIndentPattern: /{\s*$/,
            decreaseIndentPattern: /^\s*}/
        }
    });

    // Autocomplete 
    const registerCompletion = () => {
        monaco.languages.registerCompletionItemProvider('alpha', {
            triggerCharacters: ['.', '(', ' '],
            provideCompletionItems: (model, position) => {
                const word = model.getWordUntilPosition(position);
                const range = {
                    startLineNumber: position.lineNumber,
                    endLineNumber: position.lineNumber,
                    startColumn: word.startColumn,
                    endColumn: word.endColumn
                };

                const text = model.getValue();
                const varMatches = [...text.matchAll(/\b(?:local\s+)?([a-zA-Z_]\w*)\s*=/g)];
                const userVars = [...new Set(varMatches.map(m => m[1]))];
                const funcMatches = [...text.matchAll(/\bfunction\s+([a-zA-Z_]\w*)\s*\(/g)];
                const userFuncs = [...new Set(funcMatches.map(m => m[1]))];

                const suggestions = [
                    // user vars first
                    ...userVars.map(v => ({
                        label: v,
                        kind: monaco.languages.CompletionItemKind.Variable,
                        insertText: v,
                        detail: 'User variable',
                        sortText: '1_' + v,
                        range
                    })),

                    // user functions
                    ...userFuncs.map(fn => ({
                        label: fn,
                        kind: monaco.languages.CompletionItemKind.Function,
                        insertText: fn + '($0)',
                        insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
                        detail: 'User function',
                        sortText: '2_' + fn,
                        range
                    })),

                    // keywords
                    ...keywords.map(k => ({
                        label: k,
                        kind: monaco.languages.CompletionItemKind.Keyword,
                        insertText: k,
                        sortText: '3_' + k,
                        detail: 'Keyword',
                        range
                    })),

                    // word operators and constants
                    ...wordOps.map(op => ({
                        label: op,
                        kind: monaco.languages.CompletionItemKind.Operator,
                        insertText: op,
                        sortText: '4_' + op,
                        detail: 'Logic operator',
                        range
                    })),
                    ...['true', 'false', 'nil'].map(val => ({
                        label: val,
                        kind: monaco.languages.CompletionItemKind.Constant,
                        insertText: val,
                        sortText: '5_' + val,
                        detail: 'Constant',
                        range
                    })),

                    // library functions
                    ...libfuncs.map(fn => ({
                        label: fn,
                        kind: monaco.languages.CompletionItemKind.Function,
                        insertText: fn + '($0)',
                        insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
                        detail: 'Library function',
                        sortText: '6_' + fn,
                        range
                    }))
                ];

                return { suggestions };
            }
        });
    }

    // One Dark theme
    monaco.editor.defineTheme('alpha-one-dark', {
        base: 'vs-dark',
        inherit: true,
        colors: {
            'editor.background': '#282C34',
            'editor.foreground': '#ABB2BF',
            'editor.lineHighlightBackground': '#99BBFF0A',
            'editor.selectionBackground': '#3E4451',
            'editorCursor.foreground': '#528BFF',
            'editorLineNumber.foreground': '#636D83',
            'editorLineNumber.activeForeground': '#ABB2BF',
            'editorIndentGuide.background': '#ABB2BF26',
            'editorIndentGuide.activeBackground': '#626772',
            'editorHoverWidget.background': '#21252B',
            'editorHoverWidget.border': '#181A1F',
            'editorSuggestWidget.background': '#21252B',
            'editorSuggestWidget.border': '#181A1F',
            'editorSuggestWidget.selectedBackground': '#2C313A',
            'editorWidget.background': '#21252B',
            'editorWidget.border': '#3A3F4B',
            'scrollbarSlider.background': '#4E566680',
            'scrollbarSlider.activeBackground': '#747D9180',
            'scrollbarSlider.hoverBackground': '#5A637580'
        },
        rules: [
            { token: 'comment', foreground: '5C6370', fontStyle: 'italic' },
            { token: 'keyword', foreground: 'C678DD' },
            { token: 'operator', foreground: '56B6C2' },
            { token: 'entity.name.function', foreground: '61AFEF' },
            { token: 'support.function', foreground: '61AFEF' },
            { token: 'property', foreground: 'E06C75' },
            { token: 'constant.language', foreground: 'D19A66' },
            { token: 'number', foreground: 'D19A66' },
            { token: 'string', foreground: '98C379' },
            { token: 'string.escape', foreground: '56B6C2' },
            { token: 'string.escape.invalid', foreground: 'F44747' },
        ]
    });

    // Create Editor
    const savedCode = localStorage.getItem('alpha:editorCode');

    window.editor = monaco.editor.create(document.getElementById('editor'), {
        value: savedCode ? savedCode : '',
        language: 'alpha',
        theme: 'alpha-one-dark',
        automaticLayout: true,
        fontSize: 14,
        minimap: { enabled: true },
        autoIndent: 'full',
        autoClosingBrackets: 'languageDefined',
        mouseWheelZoom: true,
    });

    if (savedCode === null) {
        window.Alpha.Utils.loadExample('examples/introduction.al');
    }

    if ('requestIdleCallback' in window) {
        requestIdleCallback(registerCompletion, { timeout: 1500 });
    } else {
        setTimeout(registerCompletion, 0);
    }

    // Store editor's current code &
    // Remove diagnostic error/warning if the line is edited
    window.editor.onDidChangeModelContent((e) => {
        localStorage.setItem('alpha:editorCode', window.editor.getValue());

        const diags = window.Alpha.Diagnostics.currentDiags;
        if (!diags.length) return;

        const touched = new Set();
        for (const ch of e.changes) {
            for (let ln = ch.range.startLineNumber; ln <= ch.range.endLineNumber; ln++) touched.add(ln);
        }

        const filtered = diags.filter(d => !touched.has(d.line));
        if (filtered.length !== diags.length) window.Alpha.Diagnostics.setAlphaMarkers(filtered, { jumpNow: false });
    });

})();
