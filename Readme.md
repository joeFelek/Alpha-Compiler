<div id="top"></div>

<h2 align="center">Alpha Online</h2>

<h3 align="center">C-based Compiler in WebAssembly</h3>

<br>

This project contains the source code of the Alpha programming language compiler and virtual machine, written in C. It brings Alpha to the browser by compiling both to WebAssembly with Emscripten, allowing you to write, compile, and run Alpha code entirely client-side.

#### 🧪 About the Alpha Language

Alpha is a dynamically typed, imperative, block-structured language with a syntax inspired by C and modern scripting languages. It was originally designed as part of a university compiler course.<br>

```javascript
function Fibonacci(n) {
    if (n <= 1) return n;
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

function Factorial(x) {
    if (x == 1) return 1;
    return x * Factorial(x - 1);
}

// Store functions in a table
math_funcs = [
    {"fib" : Fibonacci},
    {"fac" : Factorial}
];

inter = 10;

print("Fibonacci(", inter, ") = ", math_funcs.fib(inter), "\n");   // Fibonacci(10) = 55
print("Factorial(", inter, ") = ", math_funcs.fac(inter), "\n");   // Factorial(10) = 3628800
```

#### ✨ Language Features
- Variables and expressions with dynamic types (`number`, `string`, `bool`, `nil`, `table`, `array`, `function`)
- Functions (named and anonymous), scoping, and closures
- Control flow: `if`, `else`, `while`, `for`, `break`, `continue`, `return`
- Array and map constructors
- Operator precedence for arithmetic, comparison, logical, and unary operators
- Compiles to a custom bytecode format executed by a stack-based VM


#### 🚀 Project Features
- 🧠 Full Alpha compiler (Flex/Bison) and VM written in C  
- 🌐 Compiled to WebAssembly using Emscripten  
- 🖥️ In-browser code editor and terminal-style console  
- 🧵 Runs inside a Web Worker to prevent UI freezes  
- 💻 Entirely client-side — no server needed
