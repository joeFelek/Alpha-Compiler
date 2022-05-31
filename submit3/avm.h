#ifndef avm_h
#define avm_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tcode.h"

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

#define AVM_TABLE_HASHSIZE 211

struct avm_table {
    unsigned refCounter;
    avm_table_bucket* strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* numIndexed[AVM_TABLE_HASHSIZE];
    // add other table types
    unsigned total;
};




/** stack **/
#define AVM_STACKSIZE  4096
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m));

avm_memcell stack[AVM_STACKSIZE];

static void avm_init_stack(void);


/** tables **/


avm_table* avm_table_new(void);
void avm_table_destroy(avm_table *t);
avm_memcell* avm_table_getelem(avm_memcell *key);
void avm_table_setelem(avm_memcell *key, avm_memcell *value);


/** translate **/
#define AVM_STACKENV_SIZE 4

avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned top, topsp;

double consts_getnumber(unsigned index);
char*  consts_getstring(unsigned index);
char*  libfuncs_getused(unsigned index);



#endif