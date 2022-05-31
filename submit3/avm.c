#include "avm.h"

static void avm_init_stack(void) {
    for(int i=0; i<AVM_STACKSIZE; ++i) {
        AVM_WIPEOUT(stack[i]);
        stack[i].type = undef_m;
    }
}

/********** tables **************/

void avm_table_incr_refcounter(avm_table *t) {
    ++t->refCounter;
}

void avm_table_decr_refcounter(avm_table *t) {
    assert(t->refCounter > 0);
    if(!--t->refCounter)
        avm_table_destroy(t);
}

void avm_table_buckets_init(avm_table_bucket **p) {
    for(int i=0; i<AVM_TABLE_HASHSIZE; ++i) 
        p[i] = (avm_table_bucket*) 0;
}

avm_table* avm_tablenew(void) {
    avm_table *t = (avm_table*) malloc(sizeof(avm_table));
    AVM_WIPEOUT(*t);

    t->refCounter = 0;
    t->total = 0;

    avm_table_buckets_init(t->numIndexed);
    avm_table_buckets_init(t->strIndexed);
    // add other table types

    return t;
}

void avm_memcell_clear(avm_memcell *m);

void avm_table_buckets_destroy(avm_table_bucket **p) {
    for(int i=0; i<AVM_TABLE_HASHSIZE; ++i, ++p) {
        for(avm_table_bucket *b = *p; b;) {
            avm_table_bucket *del = b;
            b = b->next;
            avm_memcell_clear(&del->key);
            avm_memcell_clear(&del->value);
            free(del);
        }
        p[i] = (avm_table_bucket*) 0;
    }
}

void avm_table_destroy(avm_table *t) {
    avm_table_buckets_destroy(t->strIndexed);
    avm_table_buckets_destroy(t->numIndexed);
    free(t);
}


/** translate **/

double consts_getnumber(unsigned index) {}
char*  consts_getstring(unsigned index) {}
char*  libfuncs_getused(unsigned index) {}

avm_memcell* avm_translate_operand(vmarg *arg, avm_memcell *reg) {
    switch(arg->type) {
        case global_a: return &stack[AVM_STACKENV_SIZE-1-arg->val];
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
            reg->data.funcVal = arg->val;
            return reg;
        case libfunc_a:
            reg->type = libfunc_m;
            reg->data.libfuncVal = libfuncs_getused(arg->val);
            return reg;
        default: assert(0);
    }
}

/** execute cycle **/

#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v

void execute_assign(instruction *instr) {}
void execute_add(instruction *instr) {}
void execute_sub(instruction *instr) {}
void execute_mul(instruction *instr) {}
void execute_div(instruction *instr) {}
void execute_mod(instruction *instr) {}
void execute_jeq(instruction *instr) {}
void execute_jne(instruction *instr) {}
void execute_jle(instruction *instr) {}
void execute_jge(instruction *instr) {}
void execute_jlt(instruction *instr) {}
void execute_jgt(instruction *instr) {}
void execute_call(instruction *instr) {}
void execute_pusharg(instruction *instr) {}
void execute_funcenter(instruction *instr) {}
void execute_funcexit(instruction *instr) {}
void execute_newtable(instruction *instr) {}
void execute_tablegetelem(instruction *instr) {}
void execute_tablesetelem(instruction *instr) {}
void execute_nop(instruction *instr) {}

void (*const executeFuncs[21])(instruction*) = 
{
    execute_assign,
    execute_add,
    execute_sub,
    execute_mul,
    execute_div,
    execute_mod,
    execute_jeq,
    execute_jne,
    execute_jle,
    execute_jge,
    execute_jlt,
    execute_jgt,
    execute_call,
    execute_pusharg,
    execute_funcenter,
    execute_funcexit,
    execute_newtable,
    execute_tablegetelem,
    execute_tablesetelem,
    0,
    execute_nop
};

unsigned char execution_finished = 0;
unsigned pc = 0;
unsigned curr_line = 0;
unsigned code_size = 0;
instruction* code = (instruction*) 0;

#define AVM_ENDING_PC code_size

void execute_cycle(void) {
    if(execution_finished)
        return;
    else {
        if(pc == AVM_ENDING_PC) {
            execution_finished = 1;
            return;
        } else {
            assert(pc < AVM_ENDING_PC);
            instruction *instr = code + pc;
            assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);
            if(instr->srcLine)
                curr_line = instr->srcLine;
            unsigned oldPC = pc;
            (*executeFuncs[instr->opcode])(instr);
            if(pc==oldPC)
                ++pc;
        }
    }
}


/** ilopoihsh entolwn **/

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

int main() {
    return 0;
}