#include "avm.h"

#define magic 340200501

extern int execute_cycle(void);

/********** global vars **************/

/** final code lists **/
double *num_consts = (double*) 0;
unsigned total_num_consts = 0;
char **string_consts = (char**) 0;
unsigned total_string_consts = 0;
char **named_lib_funcs = (char**) 0;
unsigned total_named_lib_funcs = 0;
userfunc *user_funcs = (userfunc*) 0;
unsigned total_user_funcs = 0;
instruction* code = (instruction*) 0;
unsigned code_size = 0;
unsigned total_global_variables = 0;

/** execution cycle and stack **/
avm_memcell stack[AVM_STACKSIZE];
avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned top = 0;
unsigned topsp = 0;
unsigned pc = 0;
unsigned curr_line = 0;
unsigned char execution_finished = 0;

/** call **/
int totalActuals = 0;

/*************************************/


int read_binary(char* file_name) {
    FILE *f;
    unsigned size;
    char *s;
    
    f = fopen(file_name, "rb");
    if(!f) {
        avm_error("'%s' not found", file_name);
        return 1;
    }

    unsigned magic_number = 0;
    fread(&magic_number, sizeof(unsigned), 1, f);
    if(magic_number != magic) {
        avm_error("'%s' is not an alpha binary file", file_name);
        return 1;
    }

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

    for(int i=0; i<AVM_STACKSIZE; ++i) {
        avm_memcell_free(&stack[i]);
    }
}

static void avm_init_stack(void) {
    for(int i=0; i<AVM_STACKSIZE; ++i) {
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

void avm_error(char* format, ...) {
    va_list ap;
    unsigned size = strlen(format);
    unsigned num;
    double dnum;
    const char *s;

    fprintf(stderr, "\n"BOLD"%d"RESET": "BOLD RED"error"RESET":  ", curr_line);
    
    va_start(ap, format);
    for(int i=0; i<size; i++) {
        if(format[i] == '%') {
            switch(format[++i]) {
                case 'd':
                    num = va_arg(ap, unsigned);
                    fprintf(stderr, "%d", num);
                    break;
                case 's':
                    s = va_arg(ap, const char*);
                    fprintf(stderr, "%s", s);
                    break;
                case 'l':
                    dnum = va_arg(ap, double);
                    fprintf(stderr, "%lf", dnum);
                    ++i;
                    break;
            }
        }else {
            fprintf(stderr, "%c", format[i]);
        }
    }
    fprintf(stderr, "\n\n");
    va_end(ap);

    execution_finished = 1;
}

void avm_warning(char* format, ...) {
    va_list ap;
    unsigned size = strlen(format);
    unsigned num;
    double dnum;
    const char *s;

    fprintf(stderr, "\n"BOLD"%d"RESET": "BOLD MAGENTA"warning"RESET":  ", curr_line);

    va_start(ap, format);
    for(int i=0; i<size; i++) {
        if(format[i] == '%') {
            switch(format[++i]) {
                case 'd':
                    num = va_arg(ap, unsigned);
                    fprintf(stderr, "%d", num);
                    break;
                case 's':
                    s = va_arg(ap, const char*);
                    fprintf(stderr, "%s", s);
                    break;
                case 'l':
                    dnum = va_arg(ap, double);
                    fprintf(stderr, "%lf", dnum);
                    ++i;
                    break;
            }
        }else {
            fprintf(stderr, "%c", format[i]);
        }
    }
    fprintf(stderr, "\n\n");
    va_end(ap);
}

