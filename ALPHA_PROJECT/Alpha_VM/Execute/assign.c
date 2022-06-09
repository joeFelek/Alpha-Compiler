#include "../avm.h"


void avm_assign(avm_memcell* lv, avm_memcell* rv) {

    if(lv == rv)
        return;
    
    if(lv->type == table_m &&
       rv->type == table_m &&
       lv->data.tableVal == rv->data.tableVal)
       return;
    
    avm_memcell_clear(lv);

    memcpy(lv, rv, sizeof(avm_memcell));

    if(lv->type == string_m)
        lv->data.strVal = strdup(rv->data.strVal);
    else if (lv->type == table_m) {
        avm_table_incr_refcounter(lv->data.tableVal);
    }
}

void execute_assign(instruction *instr) {
    avm_memcell *lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell *rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv > &stack[top] || lv==&retval));
    assert(rv);

    avm_assign(lv,rv);
}