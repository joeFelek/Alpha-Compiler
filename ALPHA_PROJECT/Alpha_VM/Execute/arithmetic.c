#include "../avm.h"


double add_impl(double x, double y) {return x+y;}
double sub_impl(double x, double y) {return x-y;}
double mul_impl(double x, double y) {return x*y;}
double div_impl(double x, double y) {
    if(!y) {
        avm_error("division by zero");
        return 0;
    }
    return x/y;
}
double mod_impl(double x, double y) {
    if(!y) {
        avm_error("mod by zero");
        return 0;
    }
    return ((unsigned) x) % ((unsigned) y);
}

typedef double (*arithmetic_func_t)(double x, double y);
arithmetic_func_t arithmeticFuncs[] = {
    add_impl,
    sub_impl,
    mul_impl,
    div_impl,
    mod_impl
};

void execute_arithmetic (instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    assert(lv && (&stack[AVM_STACKSIZE -1] >= lv && lv > &stack[top] || lv==&retval));
    assert(rv1 && rv2);

    if(rv1->type == string_m && rv2->type == string_m && instr->opcode == add_v) {
        char *s = malloc(strlen(rv1->data.strVal)+strlen(rv2->data.strVal)+1);
        sprintf(s, "%s%s", rv1->data.strVal, rv2->data.strVal);
        avm_memcell_clear(lv);
        lv->type = string_m;
        lv->data.strVal = s;
    
    }else if(rv1->type != number_m || rv2->type != number_m) {
        avm_error("not a number in arithmetic!");
    }else {
        arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
        avm_memcell_clear(lv);
        lv->type = number_m;
        lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
    }
}
