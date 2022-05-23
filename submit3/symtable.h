/* Felekidis Iosif
 * A.M. 4062
 * symtable.h
 */

#ifndef symtable_h_
#define	symtable_h_

#define RESET   "\033[0m"
#define BOLD    "\033[1m" 
#define RED     "\033[31m"  
#define GREEN   "\033[32m"    
#define MAGENTA "\033[35m"      
#define CYN     "\033[1m\033[36m"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <math.h>


#define HASH_MULTIPLIER 90
#define DEFAULT_SIZE 512
#define LIBFUNCNUM 12

#define SCOPEUP current_scope++
#define SCOPEDOWN current_scope--
#define FUNCUP func_scope++
#define FUNCDOWN func_scope--

extern unsigned int current_scope;
extern unsigned int func_scope;

extern char *libFuncNames[LIBFUNCNUM];

extern char* anonymousFuncBuf;
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

extern struct SymTable_S *symTable;
extern struct ScopeList *scopeList;
extern struct ScopeList  *funcScopeList;

ScopeList* newScopeList(void);
Scope* addScope(ScopeList* scopeList, unsigned int scope);
Scope* getScope(ScopeList* scopeList, unsigned int scope);
int ScopeList_put(ScopeList *scopeList, SymbolTableEntry *e, int scope, int inFunc);
char* symbolTypeToString(int type);
char* scopeSpaceToString(int space);
void printScopeList(ScopeList* scopeList);
unsigned int SymTable_hash(const char *pcKey);
SymTable_S* SymTable_new(void);
int SymTable_put(SymTable_S *oSymTable, SymbolTableEntry *e);
SymbolTableEntry* newSymEntry(char* name, int line, int type, int scope, int fscope);
SymbolTableEntry* insert(char* name, int line, int type);
SymbolTableEntry* lookup(char *name, int scope);
SymbolTableEntry* lookupAll(char *name, int start_scope);
void hide(int scope);
int inFunc(SymbolTableEntry *e);
int libraryCollision(char *name);
void insertLibFuncs(void);
char* newTempFuncName(void);
int equalSymEntries(SymbolTableEntry *a, SymbolTableEntry *b);


#endif