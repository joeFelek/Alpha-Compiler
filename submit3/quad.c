#include "quad.h"


/** emit and quads **/

#define EXPAND_SIZE 1024
#define CURR_SIZE   (total*sizeof(quad))
#define NEW_SIZE    (EXPAND_SIZE*sizeof(quad)+CURR_SIZE)

quad* quads = (quad*) 0;
unsigned total = 0;
unsigned currQuad = 0;
unsigned total_global_variables = 0;

char *iopcodeStrings[25] = {
    "assign",         "add",            "sub", 
    "mul",            "div",	        "mod", 
	"if_eq",          "if_noteq",		"if_lesseq",
	"if_greatereq",   "if_less", 		"if_greater",
	"call",           "param",			"return",
	"getretval",      "funcstart", 		"funcend",
	"tablecreate",	  "tablegetelem",   "tablesetelem",
	"jump", 		  "and",            "or", 			
};

void printExpr(expr* e) {
	if(!e) {
		printf(" %-15c", ' ');
		return;
	}
	
	switch(e->type) {
		case constnum_e:    
			if(isInteger(e->val.numConst))
				printf(" %-15d", (int)e->val.numConst);
			else
				printf(" %-15.2f", e->val.numConst);
			break;
		case conststring_e: 
			char* s = malloc(strlen(e->val.strConst)+3);
			sprintf(s, "\"%s\"", e->val.strConst);
			printf(" %-15s", s); 
			free(s);
			break;
		case constbool_e:
			if(e->val.boolConst) 
				printf(" %-15s", "TRUE");
			else  
				printf(" %-15s", "FALSE"); 
			break;
		case nil_e:
			printf(" %-15s", "nil");
			break;
		default: 
			if(e->sym) 
				printf(" %-15s", e->sym->value.varVal->name); 
			else 
				printf(" %-15c", ' ');
	}
}

void printQuads(void) {
	quad q;
	printf("quad\topcode\t\tresult\t\targ1\t\targ2\t\tlabel\n");
	printf("-----------------------------------------------------------------------------------------------\n");
	for(int i=0; i<currQuad; ++i) {
		q = quads[i];
		
		printf("%d: \t%-15s", i, iopcodeStrings[q.op]);
		printExpr(q.result);
		printExpr(q.arg1);
		printExpr(q.arg2);
		if(q.label)
			printf(" %-15d", q.label);
		printf("\n");
	}
}

void expand(void) {
    assert(total==currQuad);
    quad* p = (quad*) malloc(NEW_SIZE);
    if (quads) {
        memcpy(p, quads, CURR_SIZE);
        free(quads);
    }
    quads = p;
    total += EXPAND_SIZE;
}

void emit(iopcode op, expr* result, expr* arg1, expr* arg2, unsigned label, unsigned line) {

    if (currQuad == total)
        expand();
    
    quad* p = quads + currQuad++;
	p->op = op;
    p->arg1 = arg1;
    p->arg2 = arg2;
    p->result = result;
    p->label = label;
    p->line = line;
}

unsigned nextQuadLabel(void) {
	return currQuad;
}

void patchLabel(unsigned quadNo, unsigned label) {
	assert(quadNo < currQuad);
	if(quads[quadNo].label == 0)
		quads[quadNo].label = label;
}


/** temp variables **/
unsigned tempcounter = 1;
char* tempname = (char*) 0;

char* newTempName(void) {
    tempname = realloc(tempname, (int)log(tempcounter)+3);
	sprintf(tempname, "$t%d", tempcounter++);
	return tempname;
}

void resetTemp(void) {
    tempcounter = 1;
}

SymbolTableEntry* newTemp(void) {
    char* name = newTempName();
    SymbolTableEntry* sym = lookup(name, current_scope);
    if(sym)
        return sym;
    
	sym = insert(name, 0, TEMP);
	sym->space = currScopeSpace();
	if(sym->space == PROGRAMVAR) ++total_global_variables; 
	sym->offset = currScopeOffset();
	incCurrScopeOffset();
	return sym;
}

unsigned istempname (const char* s) {
	return *s == '$' && *(s+1) == 't';
}

unsigned istempexpr (expr* e) {
	return e->sym && istempname(e->sym->value.varVal->name);
}

void freetempname(void) {
	free(tempname);
}

/** scope space and offset **/
unsigned programVarOffset    = 0;
unsigned functionLocalOffset = 0;
unsigned formalArgOffset     = 0;
unsigned scopeSpaceCounter   = 1;
struct Stack offset_stack;

scopespace_t currScopeSpace(void) {
	if (scopeSpaceCounter == 1)
		return PROGRAMVAR;
	else if(scopeSpaceCounter % 2 == 0)
		return FORMALARG;
	else 
		return FUNCTIONLOCAL;
}

unsigned currScopeOffset(void) {
	switch (currScopeSpace()) {
		case PROGRAMVAR     : return programVarOffset;
		case FUNCTIONLOCAL  : return functionLocalOffset;
		case FORMALARG      : return formalArgOffset;
		default: assert(0);
	}
}

void incCurrScopeOffset(void) {
	switch (currScopeSpace()) {
		case PROGRAMVAR     : ++programVarOffset; break;
		case FUNCTIONLOCAL  : ++functionLocalOffset; break;
		case FORMALARG      : ++formalArgOffset; break;
		default: assert(0);
	}
}

void enterScopeSpace(void) {
	++scopeSpaceCounter;
}

void exitScopeSpace(void) {
	assert(scopeSpaceCounter>1);
	--scopeSpaceCounter;
}

void resetFormalArgsOffset(void) {
	formalArgOffset = 0;
}

void resetFunctionLocalsOffset(void) {
	functionLocalOffset = 0;
}

void restoreCurrScopeOffset(unsigned n) {
	switch(currScopeSpace()) {
		case PROGRAMVAR		: programVarOffset = n; break;
		case FUNCTIONLOCAL 	: functionLocalOffset = n; break;
		case FORMALARG		: formalArgOffset = n; break;
		default: assert(0);
	}
}

void pushCurrScopeOffset(void) {
	push(&offset_stack, currScopeOffset()); 
}

unsigned popScopeOffset(void) {
    return pop(&offset_stack);
}


/** expresions **/
expr* lvalue_expr(SymbolTableEntry *sym) {
    assert(sym);
    expr* e = (expr*) malloc(sizeof(expr));
    memset(e, 0, sizeof(expr));

    e->next = (expr*) 0;
    e->sym = sym;

    if(sym->type == LIBFUNC)
       e->type = libraryfunc_e;
    else if(sym->type == USERFUNC)
       e->type = programfunc_e;
    else 
       e->type = var_e;

    return e;
}

expr* member_item (expr* lv, char* name) {
	lv = emit_iftableitem(lv); // Emit code if r-value use of table item
	expr* ti = newexpr(tableitem_e); // Make a new expression
	ti->sym = lv->sym;
	ti->index = newexpr_conststring(name); // Const string index
	return ti;
}

expr* newexpr(expr_t t) {
	expr* e = (expr*) malloc(sizeof(expr));
	memset(e, 0, sizeof(expr));
	e->type = t;
	return e;
}

expr* newexpr_conststring(char* s) {
	expr* e = newexpr(conststring_e);
	e->val.strConst = strdup(s);
	return e;
}

expr* newexpr_constnum(double i) {
	expr* e = newexpr(constnum_e);
	e->val.numConst = i;
	return e;
}

expr* newexpr_constbool(unsigned int b) {
	expr* e = newexpr(constbool_e);
	e->val.boolConst = !!b;
	return e;
}

expr* emit_iftableitem(expr* e) {
	if(e->type != tableitem_e)
		return e;
	
	expr* result = newexpr(var_e);
	result->sym = newTemp();
	emit(tablegetelem, result, e, e->index, 0, yylineno);
	return result;	
}


/** call **/
call_s* new_call(expr* elist, unsigned char method, char* name) {
	call_s* c = malloc(sizeof(call_s));
	c->elist = elist;
	c->method = method;
	c->name = name;
	return c;
}

expr* make_call(expr* lv, expr* reversed_elist) {
	expr* func = emit_iftableitem(lv);
	while(reversed_elist) {
		emit(param, NULL, reversed_elist, NULL, 0, yylineno);
		reversed_elist = reversed_elist->next;
	}
	emit(call, NULL, func, NULL, 0, yylineno);
	expr* result = newexpr(var_e);
	result->sym = newTemp();
	emit(getretval, result, NULL, NULL, 0, yylineno);
	return result;
}


/** quad lists **/
struct Stack loop_counter_stack;

stmt_t* make_stmt(void) {
	stmt_t *s = malloc(sizeof(stmt_t));
	s->breaklist = 0;
	s->contlist = 0; 
	return s;
}

int newlist(int i) {
	quads[i].label = 0;
	return i; 
}

int mergelist (int l1, int l2) {
	if (!l1)
		return l2;
	else
		if (!l2)
			return l1;
		else {
			int i = l1;
			while (quads[i].label)
				i = quads[i].label;
			quads[i].label = l2;
			return l1;
		}
}

void patchlist (int list, int label) {
	while (list) {
		int next = quads[list].label;
		quads[list].label = label;
		list = next;
	}
}

void pushCurrLoopCounter(unsigned loopcounter) {
	push(&loop_counter_stack, loopcounter); 
}

unsigned popLoopCounter(void) {
	return pop(&loop_counter_stack);
}


/** short circuit evaluation **/
shortcircuit_t* makeSceEntry(unsigned label) {
    shortcircuit_t* e = malloc(sizeof(shortcircuit_t));
    e->label = label;
    e->next = (shortcircuit_t*) 0;
    return e; 
}

shortcircuit_t* mergeSceEntries(shortcircuit_t* e1, shortcircuit_t* e2) {
	if(!e1)
		return e2;
	if(!e2)
		return e1;
	shortcircuit_t *e = e1;
	while(e->next) e = e->next;
	e->next = e2;
    return e1;
}

void backpatch(shortcircuit_t* list, unsigned label) {
    shortcircuit_t* e = list;
    while(e) {
		patchLabel(e->label, label);
        e = e->next;
    }
}


/** helper **/
int isInteger(double val) {
    int truncated = (int)val;
    return (val == truncated);
}

void push(struct Stack *s, unsigned value) {

    if(!s->stack) {
        s->stack = malloc(sizeof(unsigned));
        s->len = 0;
    } else {
        s->stack = realloc(s->stack, sizeof(unsigned)*(s->len+1));
    } 
    s->stack[s->len++] = value;
}

unsigned pop(struct Stack *s) {
    assert(s->len > 0);
    return s->stack[(s->len--)-1];
} 

void freeStacks(void) {
	free(offset_stack.stack);
	free(loop_counter_stack.stack);
}