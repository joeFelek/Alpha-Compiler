<div id="top"></div>

<h2 align="center">Alpha Online Compiler</h2>
<p align="center">
  <a href="https://joefelek.github.io/Alpha-Compiler/" target="_blank" rel="noopener">
    <img alt="Live Demo" src="https://img.shields.io/badge/Live%20Demo-Open-brightgreen?style=for-the-badge">
  </a>
</p>
<h4 align="center">Code editor + C-based compiler & VM in WebAssembly</h4>


<br>


This project contains the source code of the Alpha programming language compiler and virtual machine, written in C. It brings Alpha to the browser by compiling both to WebAssembly, allowing you to write, compile, and run Alpha code entirely client-side.

#### What is the Alpha Language?

Alpha is a dynamically typed, imperative, block-structured language with a syntax inspired by C and modern scripting languages. It was originally designed as part of a university compiler course.<br>

#### Language Features
- Variables and expressions with dynamic types (`number`, `string`, `bool`, `nil`, `table`, `array`, `function`)
- Functions (named and anonymous), scoping, and closures
- Control flow: `if`, `else`, `while`, `for`, `break`, `continue`, `return`
- Array and map constructors
- Operator precedence for arithmetic, comparison, logical, and unary operators
- Various library functions
- Compiles to a custom bytecode format executed by a stack-based VM
- Automatic garbage colection
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


#### Project Features
- Compiler and VM written in C up and running in the browser using Emscripten compile to wasm.
- Monaco editor configured for the alpha language with syntax highlighting, autocomplete and one dark pro theme.
- Xterm terminal with support calling `width()` and `height()` functions from alpha code to get the current resolution of the terminal.
- Responsive UI design with run/stop button, github, 3 auto load example buttons, auto clear toggle and clear terminal toggle.
- Intruction, syntax and 2 complete examples writen in alpha (game of life, donut).
- Entirely client-side no server needed.

<img width="2558" height="1297" alt="image" src="https://github.com/user-attachments/assets/96789c87-d62a-4aef-9515-d436cf1da0e6" /><div id="top"></div>


### MIT License
This project comes with the MIT license, you are free to use, change and publish any part of it. Have fun experimenting!
