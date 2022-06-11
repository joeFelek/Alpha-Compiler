#include "../avm.h"


unsigned char avm_compare_memcells(avm_memcell* rv1, avm_memcell* rv2) {
    if(rv1->type != rv2->type) 
        return 0;
    
    char *sv1, *sv2;
    unsigned char result;

    switch(rv1->type) {
        case number_m: return rv1->data.numVal == rv2->data.numVal;
        case string_m: return !strcmp(rv1->data.strVal, rv2->data.strVal);
        case table_m: 
            sv1 = avm_tostring(rv1);
            sv2 = avm_tostring(rv2);
            result = !strcmp(sv1, sv2);
            free(sv1);
            free(sv2);
            return result;
        case userfunc_m: return rv1->data.funcVal == rv2->data.funcVal;
        case libfunc_m: return !strcmp(rv1->data.libfuncVal, rv2->data.libfuncVal);
        default: assert(0);
    }
}

unsigned char avm_eq(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    unsigned char result = 0;

    if(rv1->type == undef_m || rv2->type == undef_m) 
        avm_error("'"CYN"undefined"RESET"' involved in equality!");
    else if(rv1->type == nil_m || rv2->type == nil_m)
        result = rv1->type == nil_m && rv2->type == nil_m;
    else if(rv1->type == bool_m || rv2->type == bool_m)
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    else if(rv1->type != rv2->type)
        avm_error("'"CYN"%s"RESET"' == '"CYN"%s"RESET"' is illegal!", memcell_type_tostring
    [rv1->type], memcell_type_tostring
    [rv2->type]);
    else {
        result = avm_compare_memcells(rv1, rv2);
    } 

    return result;
}

void execute_jeq(instruction* instr) {
    
    unsigned char result = avm_eq(instr);

    if(!execution_finished && result) 
        pc = instr->result.val;
}

void execute_jne(instruction* instr) {
    
    unsigned char result = avm_eq(instr);

    if(!execution_finished && !result) 
        pc = instr->result.val;
}

unsigned char jle_impl(double x, double y) {return x<=y;}
unsigned char jge_impl(double x, double y) {return x>=y;}
unsigned char jlt_impl(double x, double y) {return x<y;}
unsigned char jgt_impl(double x, double y) {return x>y;}

typedef unsigned char (*comparison_func_t)(double x, double y);
comparison_func_t comparisonFuncs[] = {
    jle_impl,
    jge_impl,
    jlt_impl,
    jgt_impl
};

void execute_comparison(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    assert(rv1 && rv2);

    if(rv1->type != number_m || rv2->type != number_m) {
        avm_error("not a number in comparison!");
    }else {
        comparison_func_t op = comparisonFuncs[instr->opcode - jle_v];
        unsigned char result = (*op)(rv1->data.numVal, rv2->data.numVal);
        if(result) 
            pc = instr->result.val;
    }
} 

void execute_jump(instruction *instr) {
    assert(instr->result.type == label_a);
    pc = instr->result.val;
}