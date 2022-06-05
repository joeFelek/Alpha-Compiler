#include "avm.h"

unsigned magic_number = 0;

int read_binary(char* file_name) {
    FILE *f;
    unsigned size;
    char *s;
    
    f = fopen(file_name, "rb");
    if(!f) {
        fprintf(stderr, "error: file %s not found\n", file_name);
        return 1;
    }

    fread(&magic_number, sizeof(unsigned), 1, f);

    // read string consts
    fread(&total_string_consts, sizeof(unsigned), 1, f);
    string_consts = (char**) malloc(sizeof(char*)*total_string_consts);
    for(int i=0; i<total_string_consts; ++i) {
        fread(&size, sizeof(unsigned), 1, f);
        string_consts[i] = (char*) malloc(sizeof(char)*size);
        fread(string_consts[i], size, 1, f);
    }

    // read number consts
    fread(&total_num_consts, sizeof(unsigned), 1, f);
    num_consts = (double*) malloc(sizeof(double)*total_num_consts);
    for(int i=0; i<total_num_consts; ++i) 
        fread(&num_consts[i], sizeof(double), 1, f);

    // read user funcs
    fread(&total_user_funcs, sizeof(unsigned), 1, f);
    user_funcs = (userfunc*) malloc(sizeof(userfunc)*total_user_funcs);
    for(int i=0; i<total_user_funcs; ++i) {
        fread(&user_funcs[i].address, sizeof(unsigned), 1, f);
        fread(&user_funcs[i].localSize, sizeof(unsigned), 1, f);
        fread(&size, sizeof(unsigned), 1, f);
        user_funcs[i].id = (char*) malloc(sizeof(char)*size);
        fread((char*)user_funcs[i].id, size, 1, f);
    }

    // read lib funcs
    fread(&total_named_lib_funcs, sizeof(unsigned), 1, f);
    named_lib_funcs = (char**) malloc(sizeof(char*)*total_named_lib_funcs);
    for(int i=0; i<total_named_lib_funcs; ++i) {
        fread(&size, sizeof(unsigned), 1, f);
        named_lib_funcs[i] = (char*) malloc(sizeof(char)*size);
        fread(named_lib_funcs[i], size, 1, f);
    }

    // read instructions
    fread(&code_size, sizeof(unsigned), 1, f);
    code = (instruction*) malloc(sizeof(instruction)*code_size);
    for(int i=0; i<code_size; ++i) {
        fread(&code[i].opcode, sizeof(int), 1, f);
        fread(&code[i].result.type, sizeof(int), 1, f);
        fread(&code[i].result.val, sizeof(unsigned), 1, f);
        fread(&code[i].arg1.type, sizeof(int), 1, f);
        fread(&code[i].arg1.val, sizeof(unsigned), 1, f);
        fread(&code[i].arg2.type, sizeof(int), 1, f);
        fread(&code[i].arg2.val, sizeof(unsigned), 1, f);
        fread(&code[i].srcLine, sizeof(unsigned), 1, f);
    }
    
    fread(&total_global_variables, sizeof(unsigned), 1, f);

    fclose(f);
    return 0;
}

void freeAll(void) {
    free(num_consts);
    for(int i=0; i<total_string_consts; ++i) 
        free(string_consts[i]);
    free(string_consts);
    for(int i=0; i<total_named_lib_funcs; ++i)
        free(named_lib_funcs[i]);
    free(named_lib_funcs);
    for(int i=0; i<total_user_funcs; ++i) 
        free((char*)user_funcs[i].id);
    free(user_funcs);
    free(code);
}

avm_memcell stack[AVM_STACKSIZE];

static void avm_init_stack(void) {
    for(int i=0; i<AVM_STACKSIZE-1; ++i) {
        AVM_WIPEOUT(stack[i]);
        stack[i].type = undef_m;
    }
    top = AVM_STACKSIZE - 1 - total_global_variables;
    topsp = top;
}

int main(int argc, char **argv) {

    int error_flag = 0;

    if(argc > 1) {
        error_flag = read_binary(argv[1]);
    }else {
        error_flag = read_binary("alpha.abc");
    }
    
    if(error_flag) return -1;

    avm_init_stack();
    for(;execute_cycle(););
    
    return 0;
}