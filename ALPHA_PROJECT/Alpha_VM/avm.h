#ifndef avm_h
#define avm_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include "../Alpha_Compiler/Final_Code/instruction.h"

/** colors **/
#define RESET   "\033[0m"
#define BOLD    "\033[1m" 
#define RED     "\033[31m"  
#define GREEN   "\033[32m"    
#define MAGENTA "\033[35m"      
#define CYN     "\033[1m\033[36m"
#define BWHT    "\e[1;37m"
#define BGRN    "\e[1;32m"
#define BBLU    "\e[1;94m"
#define BCYN    "\e[36m"
#define BBLA    "\e[2m"

typedef struct avm_memcell avm_memcell;
typedef struct avm_table avm_table;

typedef enum avm_memcell_t {
    number_m,
    string_m,
    bool_m,
    table_m,
    userfunc_m,
    libfunc_m,
    nil_m,
    undef_m
}avm_memcell_t;

struct avm_memcell {
    avm_memcell_t type;
    union {
        double        numVal;
        char*         strVal;
        unsigned char boolVal;
        avm_table*    tableVal;
        unsigned      funcVal;
        char*         libfuncVal;
    }data;
};

typedef struct avm_table_bucket {
    avm_memcell key;
    avm_memcell value;
    struct avm_table_bucket* next;
}avm_table_bucket;

struct avm_table {
    unsigned refCounter;
    unsigned total;
    avm_table_bucket** strIndexed;
    avm_table_bucket** numIndexed;
    // TODO: add other table types
};


/** final code lists **/
extern double *num_consts;
extern unsigned total_num_consts;

extern char **string_consts;
extern unsigned total_string_consts;

extern char **named_lib_funcs;
extern unsigned total_named_lib_funcs;

extern userfunc *user_funcs;
extern unsigned total_user_funcs;

extern instruction *code;
extern unsigned code_size;

extern unsigned total_global_variables;


/** error handling **/
extern void avm_error(char* format, ...);
extern void avm_warning(char* format, ...);


/** stack **/
#define AVM_STACKSIZE  4096
#define AVM_STACKENV_SIZE 4
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m));

#define AVM_NUMACTUALS_OFFSET 4
#define AVM_SAVEDPC_OFFSET 3
#define AVM_SAVEDTOP_OFFSET 2
#define AVM_SAVEDTOPSP_OFFSET 1

#define AVM_ENDING_PC code_size

extern avm_memcell ax, bx, cx;
extern avm_memcell retval;
extern unsigned pc;
extern unsigned top, topsp; 
extern unsigned curr_line;
extern unsigned char execution_finished;

extern avm_memcell stack[AVM_STACKSIZE];


/** translate **/
extern avm_memcell* avm_translate_operand(vmarg *arg, avm_memcell *reg);


/** tables **/
#define AVM_TABLE_HASHSIZE 211

avm_table* avm_tablenew(void);
void avm_table_destroy(avm_table *t);
void avm_table_incr_refcounter(avm_table* t);
void avm_table_decr_refcounter(avm_table* t);
extern avm_memcell* avm_tablegetelem(avm_table* table, avm_memcell* index);
extern void avm_tablesetelem(avm_table* table, avm_memcell* index, avm_memcell* content);


/** garbage collection **/
extern void avm_memcell_clear(avm_memcell *m);
extern void avm_memcell_free(avm_memcell *m);


/** tostring **/
extern char* libraryFuncs_tostring[];
extern char* memcell_type_tostring[];
char* avm_tostring(avm_memcell* m);


/** tobool **/
unsigned char avm_tobool(avm_memcell* m);


/** library **/
void avm_call_libfunc(char* id);


/** assign **/
extern void avm_assign(avm_memcell* lv, avm_memcell* rv);


/** call **/
extern int totalActuals;
extern void avm_call_save_environment(void);
extern unsigned avm_get_env_value(unsigned i);
extern void avm_dec_top(void);
userfunc* avm_getfuncinfo(unsigned address);


#endif