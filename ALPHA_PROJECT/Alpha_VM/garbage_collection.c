#include "avm.h"

#define memfree_string memclear_string

void memclear_string(avm_memcell *m) {
    assert(m->data.strVal);        
    free(m->data.strVal);
}

void memclear_table(avm_memcell *m) {
    assert(m->data.tableVal);
    avm_table_decr_refcounter(m->data.tableVal);
}

typedef void (*memclear_func_t)(avm_memcell*);
memclear_func_t memclearFuncs[] = {
    0,  // number
    memclear_string,
    0,  // bool
    memclear_table,
    0,  // userfunc
    0,  // libfunc
    0,  // nil
    0   //undef
};

void avm_memcell_clear(avm_memcell *m) {
    if(m->type != undef_m) {
        memclear_func_t f = memclearFuncs[m->type];
        if(f)
            (*f)(m);
        m->type = undef_m;
    }
}

void memfree_table(avm_memcell* m) {
    assert(m->data.tableVal);
    avm_table_destroy(m->data.tableVal);
}

typedef void (*memfree_func_t)(avm_memcell*);
memfree_func_t memfreeFuncs[] = {
    0,  // number
    memfree_string,
    0,  // bool
    memfree_table,
    0,  // userfunc
    0,  // libfunc
    0,  // nil
    0   //undef
};

void avm_memcell_free(avm_memcell *m) {
    if(m->type != undef_m) {
        memfree_func_t f = memfreeFuncs[m->type];
        if(f)
            (*f)(m);
        m->type = undef_m;
    }
}