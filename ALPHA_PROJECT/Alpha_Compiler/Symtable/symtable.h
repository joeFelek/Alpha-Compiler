#ifndef symtable_h_
#define	symtable_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define RESET   "\033[0m"
#define BOLD    "\033[1m" 
#define RED     "\033[31m"  
#define GREEN   "\033[32m"    
#define MAGENTA "\033[35m"      
#define CYN     "\033[1m\033[36m"

#define yyerror(...) custom_yyerror(__VA_ARGS__, NULL)
#define yywarning(...) custom_warning(__VA_ARGS__, NULL)
extern void custom_yyerror(char const *str, ...);
extern void custom_warning(const char *str, ...);

extern unsigned total_global_variables;
extern unsigned currScopeSpace(void);
extern unsigned currScopeOffset(void);
extern void incCurrScopeOffset(void);

extern int yylineno;


#define HASH_MULTIPLIER 90
#define DEFAULT_SIZE 512 				   /* symbol Table size */
#define LIBFUNCNUM 12

#define SCOPEUP current_scope++
#define SCOPEDOWN current_scope--
#define FUNCUP func_scope++
#define FUNCDOWN func_scope--

extern unsigned int current_scope;		   /* variables scope */
extern unsigned int func_scope;			   /* functions scope */

extern char *libFuncNames[LIBFUNCNUM];

extern char* anonymousFuncBuf;				/* the name of the current anonymous func */
extern unsigned int anonymousFuncIter;      

typedef struct SymTable_S {
	unsigned int len;
	struct SymbolTableEntry* hash_t[DEFAULT_SIZE];
} SymTable_S;

typedef struct Scope {
	unsigned int len;
	unsigned int scope;
	struct SymbolTableEntry *head;				/* first symbol in scope */
} Scope;

typedef struct ScopeList {
	unsigned int len;
	struct Scope *scopes;
} ScopeList;

typedef struct Variable {
	const char *name;
	unsigned int scope;
	unsigned int fscope;
	unsigned int line;
} Variable;

typedef struct Function {
	const char *name;
	unsigned int scope;
	unsigned int fscope;
	unsigned int line;
	unsigned int iaddress;
	unsigned int totalLocals;
} Function;

enum SymbolType {
	GLOBALVAR, LOCALVAR, FORMALVAR, USERFUNC, LIBFUNC, TEMP
};

typedef enum scopespace_t {
	PROGRAMVAR, FUNCTIONLOCAL, FORMALARG
}scopespace_t;

typedef struct SymbolTableEntry { 
	int isActive;
	union {
		Variable *varVal;
 		Function *funcVal;
	} value;
	unsigned offset;
	scopespace_t space;
	enum SymbolType type;
	struct SymbolTableEntry *next_inScope;		/* next symbol in same scope */
    struct SymbolTableEntry *next_inFunc;       /* next symbol in same function */
    struct SymbolTableEntry *next_inHash;       /* next symbol in same bucket */
} SymbolTableEntry;

extern struct SymTable_S *symTable;				/* The symbol Table */
extern struct ScopeList *scopeList;				/* a list of all variables in the same scope */
extern struct ScopeList  *funcScopeList;		/* a list of all variables in the same function scope */

/* scope handling */
ScopeList* newScopeList(void);
Scope* addScope(ScopeList* scopeList, unsigned int scope);
Scope* getScope(ScopeList* scopeList, unsigned int scope);
void ScopeList_put(ScopeList *scopeList, SymbolTableEntry *e, int scope, int inFunc);
void printScopeList(ScopeList* scopeList);

/* symtable handling */
void initSymTable(void);
unsigned int SymTable_hash(const char *pcKey);
SymTable_S* SymTable_new(void);
void SymTable_put(SymTable_S *oSymTable, SymbolTableEntry *e);
SymbolTableEntry* newSymEntry(char* name, int line, int type, int scope, int fscope);

/* insert, search and hide symbols */
SymbolTableEntry* insert(char* name, int line, int type);
SymbolTableEntry* lookup(char *name, int scope);
SymbolTableEntry* lookupAll(char *name, int start_scope);
void hide(int scope);

/* add to symtable X */
SymbolTableEntry* symtable_ID(char* name);
SymbolTableEntry* symtable_LOCAL_ID(char* name);
SymbolTableEntry* symtable_GLOBAL_ID(char* name);
SymbolTableEntry* symtable_FUNC(char* name);
SymbolTableEntry* symtable_IDLIST(char* name);


void insertLibFuncs(void);
int libraryCollision(char *name);
int inFunc(SymbolTableEntry *e);
int equalSymEntries(SymbolTableEntry *a, SymbolTableEntry *b);
char* newTempFuncName(void);
char* symbolTypeToString(int type);
char* scopeSpaceToString(int space);


#endif