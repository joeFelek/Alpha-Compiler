#ifndef tcode_h_
#define	tcode_h_

#include "quad.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

typedef enum vmopcode {
    assign_v,       add_v,          sub_v,
    mul_v,          div_v,          mod_v, 
    jeq_v,          jne_v,          jle_v,
    jge_v,          jlt_v,          jgt_v,
    call_v,         pusharg_v,      funcenter_v,    
    funcexit_v,     newtable_v,     tablegetelem_v, 
    tablesetelem_v, jump_v,         nop_v
}vmopcode;

typedef enum vmarg_t {
    label_a, 
    global_a, 
    formal_a, 
    local_a, 
    number_a, 
    string_a, 
    bool_a, 
    nil_a, 
    userfunc_a, 
    libfunc_a, 
    retval_a
}vmarg_t;

typedef struct vmarg {
    vmarg_t  type;
    unsigned val;
}vmarg;

typedef struct instruction {
    vmopcode opcode;
    vmarg    result;
    vmarg    arg1;
    vmarg    arg2;
    unsigned srcLine;
}instruction;

typedef struct userfunc {
    unsigned    address;
    unsigned    localSize;
    const char* id;
}userfunc;


extern double *num_consts;
extern unsigned total_num_consts;

extern char **string_consts;
extern unsigned total_string_consts;

extern char **named_lib_funcs;
extern unsigned total_named_lib_funcs;

extern userfunc *user_funcs;
extern unsigned total_user_funcs;


void generateTCode(void);
void generateBitCode(char* output_name);
void print_instructions(void);

void PRINTER_NUM(void);
void PRINTER_STR(void);
void PRINTER_USERFUNC(void);
void PRINTER_LIB(void);

#endif