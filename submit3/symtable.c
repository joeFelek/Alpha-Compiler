#include "symtable.h"

struct SymTable_S *symTable;
struct ScopeList *scopeList;
struct ScopeList  *funcScopeList;

unsigned int current_scope = 0;
unsigned int func_scope = 0;

char *libFuncNames[LIBFUNCNUM] = {
	"print", "input", "objectmemberkeys", "objecttotalmembers", 
	"objectcopy", "totalarguments", "argument", "typeof", "strtonum",
	"sqrt", "cos", "sin"
};

char* anonymousFuncBuf = NULL;
unsigned int anonymousFuncIter = 1;

ScopeList* newScopeList(void) {
	anonymousFuncBuf = NULL;
	ScopeList* newList = malloc(sizeof(ScopeList));
	assert(newList);
	newList->len = 0;
	newList->scopes = NULL;
	return newList;
}

Scope* addScope(ScopeList* scopeList, unsigned int scope) {
	Scope *newscope = malloc(sizeof(Scope));
	assert(newscope);
	newscope->len = 0;
	newscope->scope = scope;
	newscope->head = NULL;
	
	scopeList->len++;
	scopeList->scopes = realloc(scopeList->scopes, sizeof(Scope)*scopeList->len);
	scopeList->scopes[scopeList->len-1] = *newscope;
	return &(scopeList->scopes[scopeList->len-1]);
}

Scope* getScope(ScopeList* scopeList, unsigned int scope) {
	for(int i=0; i<scopeList->len; i++) 
		if(scopeList->scopes[i].scope == scope)
			return &(scopeList->scopes[i]); 
	return NULL;
}

int ScopeList_put(ScopeList *scopeList, SymbolTableEntry *e, int scope, int inFunc) {
	Scope *s = getScope(scopeList, scope);
	if(!s) {
		s = addScope(scopeList, scope);
	}
	if(inFunc) 
		e->next_inFunc = s->head;
	else
		e->next_inScope = s->head;
	s->head = e;
	return 1;
}

char* symbolTypeToString(int type) {
    switch(type) {
        case GLOBALVAR: return "global variable";
        case LOCALVAR: return "local variable";
        case FORMALVAR: return "formal argument";
        case USERFUNC: return "user function";
        case LIBFUNC: return "library function";
		case TEMP: return "temp variable";
		default: assert(0);
	}	
}

char* scopeSpaceToString(int space) {
	switch(space) {
        case PROGRAMVAR: return "program variable";
        case FUNCTIONLOCAL: return "function local";
        case FORMALARG: return "formal argument";
		default: assert(0);
	}	
}

void printScopeList(ScopeList* scopeList) {

	printf("\n\n");
    for(int i=0; i<scopeList->len; i++) {
        SymbolTableEntry *e = scopeList->scopes[i].head;
        printf("------------\tScope #%d\t------------\n", scopeList->scopes[i].scope);
        while(e) {
			if(e->type == USERFUNC) {
				printf("\"%s\" [%s] (line %d) (scope %d) (func scope %d) (isActive %d) (locals %d) (iaddress %d)\n",
           	 		   e->value.varVal->name, symbolTypeToString(e->type), 
           	 		   e->value.varVal->line, e->value.varVal->scope, e->value.varVal->fscope, 
					   e->isActive, e->value.funcVal->totalLocals, e->value.funcVal->iaddress);

			} else {
            	printf("\"%s\" [%s] (line %d) (scope %d) (func scope %d) (isActive %d) (%s) (offset %d)\n",
            		   e->value.varVal->name, symbolTypeToString(e->type), 
            		   e->value.varVal->line, e->value.varVal->scope, e->value.varVal->fscope, 
					   e->isActive, scopeSpaceToString(e->space), e->offset);
			}
			e = e->next_inScope;
        }
        printf("\n");
    } 
	printf("\n\n");  
}

unsigned int SymTable_hash(const char *pcKey) {
	size_t ui;
  	unsigned int uiHash = 0U;
  	for (ui = 0U; pcKey[ui] != '\0'; ui++)
    	uiHash = (uiHash + pcKey[ui]%HASH_MULTIPLIER) % DEFAULT_SIZE;
  	return uiHash;
} 

SymTable_S* SymTable_new(void) {
	SymTable_S *newsymt;
	SymbolTableEntry *newitem;

	newsymt = malloc(sizeof(struct SymTable_S));
	newitem = malloc(sizeof(struct SymbolTableEntry));
	assert(newsymt && newitem);

	newitem->isActive = 0;
	newitem->next_inHash = NULL;
    newitem->next_inScope = NULL;
	newitem->next_inFunc = NULL;
	newsymt->len = 0;
	for(int i=0; i<DEFAULT_SIZE; i++) 
		newsymt->hash_t[i] = newitem;
	return newsymt;
}

int SymTable_put(SymTable_S *oSymTable, SymbolTableEntry *e) {
													   							
	SymbolTableEntry *newitem, *head;
	unsigned int hash;
	assert(oSymTable && e);

    newitem = e;
	hash = SymTable_hash(newitem->value.varVal->name);
	head = oSymTable->hash_t[hash];
	newitem->next_inHash = head;
	head = newitem;
	oSymTable->hash_t[hash] = head;
	oSymTable->len++;
	return 1;

}

SymbolTableEntry* newSymEntry(char* name, int line, int type, int scope, int fscope) {
	Variable *var;
	Function *func;
	SymbolTableEntry *e = malloc(sizeof(struct SymbolTableEntry));
	assert(e);

	if(type == USERFUNC || type == LIBFUNC) {
		func = malloc(sizeof(struct Function));
		func->name = strdup(name);
		func->line = line;
		func->scope = scope;
		func->fscope = fscope;
		e->type = type;
		e->value.funcVal = func;
	}else {
		var = malloc(sizeof(struct Variable));
		var->name = strdup(name);
		var->line = line;
		var->scope = scope;
		var->fscope = fscope;
		if(scope == 0 && type != TEMP)
			e->type = GLOBALVAR;
		else 
			e->type = type;
		e->value.varVal = var;	
	}
	e->isActive = 1;
	e->next_inHash = NULL;
	e->next_inScope = NULL;
	e->next_inFunc = NULL;

	return e;
}

SymbolTableEntry* insert(char* name, int line, int type) {
	SymbolTableEntry *e = newSymEntry(name, line, type, current_scope, func_scope);
	SymTable_put(symTable, e);
	ScopeList_put(scopeList, e, current_scope, 0); 
	ScopeList_put(funcScopeList, e, func_scope, 1);
	return e;
}

SymbolTableEntry* lookup(char *name, int scope) {
	SymbolTableEntry *e;
  	unsigned int hash;
	  
  	hash = SymTable_hash(name);
  	e = symTable->hash_t[hash];
  	while (e) {
  	  	if (e->value.varVal && e->isActive  &&
			e->value.varVal->scope == scope &&
			!strcmp(e->value.varVal->name, name)) 
			break; 
  	  	e = e->next_inHash;
 	}
  	return e;
}

SymbolTableEntry* lookupAll(char *name, int start_scope) {
	
	SymbolTableEntry *e;
  	unsigned int hash;

  	hash = SymTable_hash(name);
  	e = symTable->hash_t[hash];

  	while (e) {
  	  	if (e->value.varVal && e->isActive && !strcmp(e->value.varVal->name, name)) 
			for(int i=start_scope; i>=0; i--) 
				if(e->value.varVal->scope == i) 
					return e; 
  	  	e = e->next_inHash;
 	}

  	return e;
}

void hide(int scope) {
	Scope *s;
	SymbolTableEntry *e;
	
	if(!scope) return;
	if(!(s = getScope(scopeList, scope))) return;
	
	e = s->head;
	while(e) {
		if(!e->isActive) break; 
		e->isActive = 0;
		e = e->next_inScope;
	}
}

int inFunc(SymbolTableEntry *e) {
	Scope *s;
	SymbolTableEntry *tmp;
	
	if(!(s = getScope(funcScopeList, func_scope))) return 0;

	tmp = s->head;
	while(tmp) {
		if(equalSymEntries(e, tmp)) return 1;
		tmp = tmp->next_inFunc;
	}
	return 0;
}

int libraryCollision(char *name) {
	for(int i=0; i<LIBFUNCNUM; i++) {
		if(!strcmp(libFuncNames[i], name))
			return 1;
	}
	return 0;
}

void insertLibFuncs(void) {
	char *name;
	for(int i=0; i<LIBFUNCNUM; i++) {
		name = libFuncNames[i];
		insert(name, 0, LIBFUNC);
	}
}

char* newTempFuncName(void) {
	anonymousFuncBuf = realloc(anonymousFuncBuf, (int)log(anonymousFuncIter)+3);
	sprintf(anonymousFuncBuf, "$f%d", anonymousFuncIter++);
	return anonymousFuncBuf;
}

int equalSymEntries(SymbolTableEntry *a, SymbolTableEntry *b) {
	if(strcmp(a->value.varVal->name, b->value.varVal->name)) return 0;
	if(a->value.varVal->line != b->value.varVal->line) return 0;
	if(a->value.varVal->scope != b->value.varVal->scope) return 0;
	return 1;
}







