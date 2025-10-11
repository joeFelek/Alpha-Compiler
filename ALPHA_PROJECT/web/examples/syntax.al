/*
    Alpha Language Syntax & Features
    Everything is in one file, you can run it as is and skim the comments.
*/

/* 1) Basic values & variables */

// integers & floats
i = 42;
f = 3.14159;

// strings (common escapes: \n, \t, \", \\, \xHH hex, \ooo octal)
s1 = "hello\nworld";
s2 = "tab\tseparated";
s3 = "quote: \" and backslash: \\";
s4 = "A via hex: \x41";   // prints "A"

// booleans & nil
t = true;
q = false;
z = nil;

/* typeof(...) returns: "number", "string", "boolean", "nil", "table", "function" */
print("typeof(i) = ", typeof(i), "\n");
print("typeof(s1) = ", typeof(s1), "\n");
print("typeof(z) = ", typeof(z), "\n\n");


/* 2) Tables: arrays & objects
    - [] creates an array/table
    - {"key" : value} creates a map (string or integer keys)
    - Access via dot (obj.key) or bracket (obj["key"] / arr[index])
*/

// array
arr = [10, 20, 30];
print("arr[0]=", arr[0], " arr[1]=", arr[1], " arr[2]=", arr[2], "\n");

// map
user = [{ "name": "Ada"}, {"age": 36 }, {2025 : "10 commits"}];
print("user.name=", user.name, ", user[\"age\"]=" , user["age"], ", user[2025]=" , user[2025], "\n");

// dynamic fields
key = "lang";
user[key] = "Alpha";
print("user.lang=", user.lang, "\n");

// nested tables (mixing arrays & objects)
grid = [
    [{ "x": 0}, {"y": 0 }],   // row 0
    [{ "x": 1}, {"y": 1 }]    // row 1
];
print("grid[1].x=", grid[1].x, ", grid[1][\"y\"]=", grid[0]["y"], "\n\n");

/* library helpers */
print("objectmemberkeys(user) -> ", objectmemberkeys(user), "\n");
print("objecttotalmembers(user) -> ", objecttotalmembers(user), "\n\n");


/* 3) Operators
    - Arithmetic: + - * / %
    - Comparisons: == != < <= > >=
    - Logic: not, and, or
    - Incr/Decr: ++ -- (prefix & postfix)
*/

a = 5; b = 2;
print("a+b=", a+b, " a-b=", a-b, " a*b=", a*b, " a/b=", a/b, " a%b=", a%b, "\n");
print("a==5?", a==5, " a!=b?", a!=b, " a>b?", a>b, "\n");

// boolean short-circuit
x = false and true;
y = true or false;
print("x=", x, " y=", y, "\n");

// ++ and --
c = 10;
++c; print("++c -> ", c, "\n");
c--; print("c-- -> ", c, "\n\n");


/* 4) Control flow: if/else, while, for, break/continue */

// if/else
n = 7;
if (n % 2 == 0) {
    print(n, " is even\n");
} else {
    print(n, " is odd\n");
}

// while
k = 0;
while (k < 3) {
    print("while k=", k, "\n");
    ++k;
}

// for(init; cond; step)
for (j = 0; j < 5; ++j) {
    if (j == 2) { continue; }   // skip 2
    if (j == 4) { break; }      // stop at 4
    print("for j=", j, "\n");
}
print("\n");


/* 5) Functions
   - Named functions
   - Anonimus functions
   - Return values
   - Nested functions & closures (scoping rules depend on your implementation)
*/

function greet(name) {
    print("Hello, ", name, "!\n");
}
greet("Alpha");

// return
function square(x) {
    return x * x;
}
print("square(12) = ", square(12), "\n");

// nested function
function outer(a) {
    function inner(b) {
        return b + 100; 
    }
    return inner(5);
}
print("outer(7) -> ", outer(7), "\n");

// anonymous function
anonymous = (function(x) {
    return x * x;
});
print("anonymous(2) = ", anonymous(2), "\n\n");


/* 8) Scope & globals
   - 'local' declares function-scoped variable
   - '::name' accesses a global explicitly
   -  By default all variables declared inside a scope are local to the scope
*/

global = 111;  
function scopeDemo() {
    local global = 222;           // shadows outer
    print("local global = ", global, "\n");
    print("real global = ", ::global, "\n");
}
scopeDemo();
print("outer global = ", global, "\n\n");


/* 6) Variadic-style helpers (builtin libfuncs)
   - totalarguments(): number of args actually passed
   - argument(i): the i-th actual argument
*/

function sumAll() {
    total = 0;
    n = totalarguments();
    for (i = 0; i < n; ++i) {
        total = total + argument(i);
    }
    return total;
}
print("sumAll(1,2,3,4) = ", sumAll(1,2,3,4), "\n\n");


/* 7) Standard library snippets */
 
print("sqrt(9)=", sqrt(9), " cos(0)=", cos(0), " sin(0)=", sin(0), "\n");
print("trunc(3.99)=", trunc(3.99), "\n");
 
numStr = "123.50";
numVal = strtonum(numStr);     // parse string to number
print("strtonum(\"", numStr, "\") = ", numVal, "\n\n");


/* 8) Mutation */

p = [
    {"x": 0}, 
    {"y": 0},
    {"move": (function(dx, dy) {
        p.x = p.x + dx;
        p.y = p.y + dy;
    })}
];

p.move(3, -2);
print("p=(" , p.x, ",", p.y, ")\n");

// shallow copy
p2 = objectcopy(p);
p2.x = 999;
print("p.x=", p.x, " p2.x=", p2.x, "\n\n");

