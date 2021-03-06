#include "avm.h"


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