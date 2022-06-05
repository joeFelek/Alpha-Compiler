#ifndef quad_h_
#define	quad_h_

#include "symtable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

extern int yylineno;

typedef enum iopcode {
    assign,         add,            sub, 
    mul,            division,       mod, 
    if_eq,          if_noteq,       if_lesseq,
    if_greatereq,   if_less,        if_greater,
    call,           param,          ret,  
    getretval,      funcstart,      funcend,
    tablecreate,    tablegetelem,   tablesetelem,   
    jump,           and,            or
}iopcode;


typedef enum expr_t {
    var_e, tableitem_e, newtable_e,
    programfunc_e, libraryfunc_e,
    arithexpr_e, boolexpr_e, assignexpr_e,
    constnum_e, constbool_e, conststring_e,
    nil_e
}expr_t;

typedef struct expr {
    expr_t type;
    SymbolTableEntry* sym;
    struct expr* index;
    union value {
        double        numConst;
        char*         strConst;
        unsigned char boolConst;
    }val;
    struct expr* next;
    struct shortcircuit_t* truelist;
    struct shortcircuit_t* falselist;
}expr;

typedef struct quad {
    iopcode  op;
    expr*    result;
    expr*    arg1;
    expr*    arg2;
    unsigned label;
    unsigned line;
}quad;

typedef struct call_s {
    expr*           elist;
    unsigned char   method;
    char*           name;
}call_s;

typedef struct indexed_s {
    expr* index;
    expr* value;
    struct indexed_s *next;
}indexed_s;

typedef struct forloop_i {
	unsigned test, enter;
}forloop_i;

typedef struct stmt_t {
	int breaklist, contlist;
}stmt_t;

typedef struct shortcircuit_t {
    unsigned label;
    struct shortcircuit_t* next;
}shortcircuit_t;

typedef struct incomplFuncJumps_t {
    unsigned start;
    unsigned end;
    unsigned *retLabels;
    size_t retNum;
    struct incomplFuncJumps_t *next;
    struct incomplFuncJumps_t *prev;
}incomplFuncJumps_t;

struct Stack {
    size_t len;
    unsigned *stack;
};

/** emit **/
extern quad* quads;
extern unsigned total;
extern unsigned currQuad;
extern unsigned total_global_variables;

void expand(void);
void emit(iopcode op, expr* result, expr* arg1, expr* arg2, unsigned label, unsigned line);
unsigned nextQuadLabel(void);
void patchLabel(unsigned quadNo, unsigned label);

void printExpr(expr* e);
void printQuads(void);


/** temp variables **/
extern unsigned tempcounter;
extern char* tempname;

char* newTempName(void);
void resetTemp(void);
SymbolTableEntry* newTemp(void);
unsigned istempname (const char* s);
unsigned istempexpr (expr* e);
void freetempname(void);

/** scope space and offset **/
extern unsigned programVarOffset;
extern unsigned functionLocalOffset;
extern unsigned formalArgOffset;
extern unsigned scopeSpaceCounter;

scopespace_t currScopeSpace(void);
unsigned currScopeOffset(void);
void incCurrScopeOffset(void);
void enterScopeSpace(void);
void exitScopeSpace(void);
void resetFormalArgsOffset(void);
void resetFunctionLocalsOffset(void);
void restoreCurrScopeOffset(unsigned n);
void pushCurrScopeOffset(void);
unsigned popScopeOffset(void);


/** expresions **/
expr* lvalue_expr(SymbolTableEntry *sym);
expr* member_item (expr* lv, char* name);
expr* newexpr(expr_t t);
expr* newexpr_conststring(char* s);
expr* newexpr_constnum(double i);
expr* newexpr_constbool(unsigned int b);
expr* emit_iftableitem(expr* e);


/** calls **/
call_s* new_call(expr* elist, unsigned char method, char* name);
expr* make_call(expr* lv, expr* reversed_elist);


/** break and continue lists **/
stmt_t* make_stmt(void);
int newlist(int i);
int mergelist (int l1, int l2);
void patchlist (int list, int label);
void pushCurrLoopCounter(unsigned loopcounter);
unsigned popLoopCounter(void);


/** short circuit evaluation **/
shortcircuit_t* makeSceEntry(unsigned label);
shortcircuit_t* mergeSceEntries(shortcircuit_t* e1, shortcircuit_t* e2);
void backpatch(shortcircuit_t* list, unsigned label);


/** helper **/
int isInteger(double val);
void freeStacks(void);
void push(struct Stack *s, unsigned value);
unsigned pop(struct Stack *s);

#endif