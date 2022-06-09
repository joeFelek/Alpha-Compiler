#ifndef final_code_h_
#define	final_code_h_

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "../Quads/quad.h"
#include "instruction.h"

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

void print_instructions(void);
void print_num_consts(void);
void print_string_consts(void);
void print_user_funcs(void);
void print_named_lib_funcs(void);

#endif