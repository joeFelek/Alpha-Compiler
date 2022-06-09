#include "../avm.h"

#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v
#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic
#define execute_jle execute_comparison
#define execute_jge execute_comparison
#define execute_jlt execute_comparison
#define execute_jgt execute_comparison

extern void execute_assign(instruction *instr);
extern void execute_add(instruction *instr);
extern void execute_sub(instruction *instr);
extern void execute_mul(instruction *instr);
extern void execute_div(instruction *instr);
extern void execute_mod(instruction *instr);
extern void execute_jeq(instruction *instr);
extern void execute_jne(instruction *instr);
extern void execute_jle(instruction *instr);
extern void execute_jge(instruction *instr);
extern void execute_jlt(instruction *instr);
extern void execute_jgt(instruction *instr);
extern void execute_call(instruction *instr);
extern void execute_pusharg(instruction *instr);
extern void execute_funcenter(instruction *instr);
extern void execute_funcexit(instruction *instr);
extern void execute_newtable(instruction *instr);
extern void execute_tablegetelem(instruction *instr);
extern void execute_tablesetelem(instruction *instr);
extern void execute_jump(instruction *instr);

void execute_nop(instruction *instr) {avm_error("unexpected instruction got excecuted!");}

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
    execute_jump,
    execute_nop
};

int execute_cycle(void) {
    if(execution_finished)
        return 0;
    else {
        if(pc == AVM_ENDING_PC) {
            execution_finished = 1;
            return 0;
        } else {
            //fprintf(stderr,"IN pc: %d\n", pc);
            assert(pc < AVM_ENDING_PC);
            instruction *instr = code + pc;
            assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);
            if(instr->srcLine)
                curr_line = instr->srcLine;
            unsigned oldPC = pc;
            (*executeFuncs[instr->opcode])(instr);
            //fprintf(stderr,"OUT pc: %d\n", pc);
            if(pc==oldPC)
                ++pc;
            return 1;
        }
    }
}