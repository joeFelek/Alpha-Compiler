#include "../avm.h"


void avm_dec_top(void) {
    if(!top)
        avm_error(BOLD RED"stack overflow" RESET "");
    else 
        --top;
}

void avm_push_env_value(unsigned val) {
    stack[top].type = number_m;
    stack[top].data.numVal = val;
    avm_dec_top();
}

void avm_call_save_environment(void) {
    avm_push_env_value(totalActuals);
    avm_push_env_value(pc+1);
    avm_push_env_value(top + totalActuals + 2);
    avm_push_env_value(topsp);
}

unsigned avm_get_env_value(unsigned i) {
    
    if(stack[i].type != number_m) 
        return 0;
    
    unsigned val = (unsigned) stack[i].data.numVal;
    
    if(stack[i].data.numVal != ((double) val))
        return 0;
    
    return val;
}

userfunc* avm_getfuncinfo(unsigned address) {
    for(int i=0; i<total_user_funcs; ++i)
        if(user_funcs[i].address == address)
            return &user_funcs[i];
    return NULL;
}

int dispatch(avm_memcell* func) {
    avm_memcell dispatch;
    dispatch.type = string_m;
    dispatch.data.strVal = strdup("()"); 

    avm_memcell* functor = avm_tablegetelem(func->data.tableVal, &dispatch);
    free(dispatch.data.strVal);
    
    if(functor) {
        if(functor->type == userfunc_m) {
            avm_assign(&stack[top], func);
            ++totalActuals;
            avm_dec_top();
            avm_call_save_environment();
            pc = functor->data.funcVal;
            assert(pc < AVM_ENDING_PC);
            assert(code[pc].opcode == funcenter_v);
            return 1;
        }
        if(functor->type == libfunc_m) {
            avm_assign(&stack[top], func);
            ++totalActuals;
            avm_dec_top();
            avm_call_save_environment();
            avm_call_libfunc(func->data.libfuncVal);
            return 1;
        }
    }
    return 0;
}

void execute_call(instruction* instr) {
    avm_memcell* func = avm_translate_operand(&instr->arg1, &ax);
    assert(func);

    if(func->type != table_m)
        avm_call_save_environment();

    char *s;
    switch(func->type) {
        case userfunc_m:
            pc = func->data.funcVal;
            assert(pc < AVM_ENDING_PC);
            assert(code[pc].opcode == funcenter_v);
            break;
        case string_m:
            avm_call_libfunc(func->data.strVal);
            break;
        case libfunc_m:
            avm_call_libfunc(func->data.libfuncVal);
            break;
        case table_m:
            if(dispatch(func)) 
                break;
        default:
            s = avm_tostring(func);
            avm_error("call: cannot bind "CYN"'%s'"RESET" to function!", s);
            free(s);
    }
}

void execute_pusharg(instruction* instr) {
    avm_memcell* arg = avm_translate_operand(&instr->arg1, &ax);
    assert(arg);

    avm_assign(&stack[top], arg);
    ++totalActuals;
    avm_dec_top();
}

void execute_funcenter(instruction *instr) {
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    assert(func);
    assert(pc == func->data.funcVal);

    totalActuals = 0;
    topsp = top;
    userfunc* f = avm_getfuncinfo(func->data.funcVal);
    assert(f);
    top = top - f->localSize;
}

void execute_funcexit(instruction* unused) {
    unsigned oldTop = top;
    top = avm_get_env_value(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_env_value(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_env_value(topsp + AVM_SAVEDTOPSP_OFFSET);

    while(++oldTop <= top)
        avm_memcell_clear(&stack[oldTop]);
}