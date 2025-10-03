# Alpha Language Syntax Reference

This document summarizes the Alpha language syntax and reserved keywords as implemented by the Bison grammar in `parser.y`.

## Program Structure

An Alpha program is a sequence of statements; an empty file is valid. Statements can be combined sequentially, and their associated break/continue lists are merged during parsing.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L89-L107】

### Statements
Alpha supports the following kinds of statements:

- **Expression statements** end with a semicolon (`expr;`).【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L109-L109】
- **Selection statements** use `if`/`else` with optional else branches.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L399-L426】
- **Iteration statements** include `while` loops, `for` loops, and any statement marked as a loop body for break/continue tracking.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L428-L473】【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L497-L514】
- **Function definitions** introduce new functions using the `function` keyword.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L335-L386】
- **Block statements** enclose zero or more statements in braces (`{}`), managing scope visibility.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L327-L329】
- **Jump statements** include `return`, `break`, and `continue`, each with loop/function-scope checks.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L475-L514】
- A lone semicolon acts as an empty statement and resets temporary state, while syntax errors also recover by clearing the current token.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L117-L120】

## Expressions

Expressions encompass assignments, arithmetic, comparisons, logical operators, unary operations, and primary forms (constants, l-values, function calls, and table constructors). Operator precedence is encoded via associativity declarations, matching conventional arithmetic and logical precedence.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L61-L71】【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L122-L171】

### Assignment
Assignments use `=` between an l-value and any expression. Table items on the left-hand side update via `tablesetelem`, and functions are immutable (assigning to a function identifier triggers an error).【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L173-L188】

### Arithmetic and Comparison Operators
Binary arithmetic operators include `+`, `-`, `*`, `/`, and `%`. Comparison operators include `>`, `<`, `>=`, `<=`, `==`, and `!=`. Each emits the appropriate quadruple and supports backpatching for relational results.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L122-L136】

### Logical Operators
Alpha provides short-circuit `and` and `or` operators plus unary `not`. Logical expressions emit conditional jumps and manage true/false lists for backpatching.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L134-L170】

### Unary Operators and Increment/Decrement
Prefix and postfix `++`/`--` operate on l-values with arithmetic type checks. Unary minus (`-expr`) is implemented via multiplication by `-1`, and logical negation (`not expr`) flips true/false lists, emitting comparison jumps when needed.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L142-L170】

## Primary Expressions and L-Values

Primary expressions include function calls, table constructors, literals, l-values, and parenthesized expressions.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L142-L207】 L-values cover:

- Simple identifiers, optionally qualified with `local` or `::` for local or global lookup.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L198-L202】
- Member access via dot notation (`expr.id`) or bracket indexing (`expr[expr]`). Table accesses first ensure the base expression is a table item, then emit index operations.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L204-L224】

Function calls support normal invocation (`f(args)`), method calls (`expr..name(args)`), and immediate invocation of anonymous function definitions. Argument lists (`elist`) evaluate expressions and chain them for emission.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L226-L275】

## Table Constructors

Table constructors use brackets and can be either array-style (`[expr1, expr2, ...]`) or map-style (`[{key : value}, ...]`). Each constructor allocates a new table expression and emits `tablecreate` followed by `tablesetelem` instructions for elements or keyed pairs.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L277-L325】

## Function Definitions

Functions are introduced with `function` followed by an optional name and parameter list. An unnamed function receives a compiler-generated name. Function bodies are blocks, and the parser tracks scope offsets, loop counters, and return jumps when entering/exiting functions.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L331-L386】

Parameters are declared within the parentheses after the function keyword using comma-separated identifiers. Each identifier is added to the symbol table with scope adjustments.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L350-L397】

## Control Flow Statements

- **If/Else:** `if (expr) stmt` with optional `else stmt`. Conditions are backpatched to ensure correct jump targets.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L399-L426】
- **While:** `while (expr) stmt` loops back to the start label and patches break/continue lists to the correct targets.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L428-L447】
- **For:** `for (init; test; update) stmt` supports optional init/update expression lists (`E`). The parser records entry/test labels and patches jumps after the loop completes.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L449-L473】

`break` and `continue` are valid only inside loops; attempts outside loops raise errors. Each statement emits a jump and records its target list for later patching.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L497-L514】

`return` statements are valid only within functions, optionally returning an expression. Each return emits a `ret` instruction followed by a jump for backpatching.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L475-L496】

## Constants

Literal constants include numeric values, string literals, the keywords `true`, `false`, and `nil`. Each literal constructs the appropriate expression node type during parsing.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L388-L393】

## Keywords and Tokens

The Alpha grammar reserves the following keywords and symbolic tokens:

- Flow control and declarations: `if`, `else`, `while`, `for`, `function`, `return`, `break`, `continue`, `local`.
- Logical/boolean literals: `true`, `false`, `nil`, `and`, `or`, `not`.
- Operators and punctuation: `::`, `..`, `==`, `!=`, `++`, `--`, `>=`, `<=`, as well as standard arithmetic (`+`, `-`, `*`, `/`, `%`), relational (`>`, `<`), assignment (`=`), member access (`.`), indexing (`[ ]`), function call (`( )`), block braces (`{ }`), separators (`,` and `;`), and colon (`:`) within table constructors.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L49-L71】【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L226-L325】【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L327-L397】【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L399-L514】

Identifier tokens (`ID`) represent user-defined names, and `NUMBER`/`STRING` provide numeric and string literals as terminals.【F:ALPHA_PROJECT/Alpha_Compiler/parser.y†L49-L55】

---
This documentation reflects the grammar rules currently encoded in `parser.y` and should be updated if the grammar evolves.
