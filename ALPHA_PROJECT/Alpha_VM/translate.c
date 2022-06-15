#include "avm.h"


double consts_getnumber(unsigned index) {
    return num_consts[index];
}
char*  consts_getstring(unsigned index) {
    return string_consts[index];
}
char*  libfuncs_getused(unsigned index) {
    return named_lib_funcs[index];
}
unsigned userfuncs_getaddress(unsigned index) {
    return user_funcs[index].address;
}

avm_memcell* avm_translate_operand(vmarg *arg, avm_memcell *reg) {
    switch(arg->type) {
        case global_a: return &stack[AVM_STACKSIZE-1-arg->val];
        case local_a:  return &stack[topsp-arg->val];
        case formal_a: return &stack[topsp+AVM_STACKENV_SIZE+1+arg->val];
        case retval_a: return &retval;
        case number_a: 
            reg->type = number_m;
            reg->data.numVal = consts_getnumber(arg->val);
            return reg;
        case string_a:
            reg->type = string_m;
            reg->data.strVal = strdup(consts_getstring(arg->val));
            return reg;
        case bool_a:
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;
        case nil_a:
            reg->type = nil_m;
            return reg;
        case userfunc_a:
            reg->type = userfunc_m;
            reg->data.funcVal = userfuncs_getaddress(arg->val);
            return reg;
        case libfunc_a:
            reg->type = libfunc_m;
            reg->data.libfuncVal = libfuncs_getused(arg->val);
            return reg;
        default: assert(0);
    }
}