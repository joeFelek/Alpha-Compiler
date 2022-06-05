#include "avm.h"

#define RESET   "\033[0m"
#define BOLD    "\033[1m" 
#define RED     "\033[31m"  
#define GREEN   "\033[32m"    
#define MAGENTA "\033[35m"      
#define CYN     "\033[1m\033[36m"
#define BWHT    "\e[1;37m"
#define BGRN    "\e[1;32m"
#define BBLU    "\e[1;94m"
#define BCYN    "\e[36m"
#define BBLA    "\e[2m"



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
    for(int i=0; i<AVM_TABLE_HASHSIZE-1; ++i) 
        p[i] = (avm_table_bucket*) 0;
}

avm_table* avm_tablenew(void) {
    avm_table *t = (avm_table*) malloc(sizeof(avm_table));
    t->numIndexed = malloc(sizeof(avm_table_bucket)*AVM_TABLE_HASHSIZE);
    t->strIndexed = malloc(sizeof(avm_table_bucket)*AVM_TABLE_HASHSIZE);

    t->refCounter = 0;
    t->total = 0;

    avm_table_buckets_init(t->numIndexed);
    avm_table_buckets_init(t->strIndexed);
    // add other table types

    return t;
}

void avm_memcell_clear(avm_memcell *m);

void avm_table_buckets_destroy(avm_table_bucket **p) {
    for(int i=0; i<AVM_TABLE_HASHSIZE-1; ++i, ++p) {
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
    free(t->strIndexed);
    free(t->numIndexed);
    free(t);
}


/** translate **/

avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned total_global_variables = 0;
unsigned top, topsp;

double *num_consts = (double*) 0;
unsigned total_num_consts = 0;

char **string_consts = (char**) 0;
unsigned total_string_consts = 0;

char **named_lib_funcs = (char**) 0;
unsigned total_named_lib_funcs = 0;

userfunc *user_funcs = (userfunc*) 0;
unsigned total_user_funcs = 0;

double consts_getnumber(unsigned index) {
    return num_consts[index];
}
char*  consts_getstring(unsigned index) {
    return string_consts[index];
}
char*  libfuncs_getused(unsigned index) {
    return named_lib_funcs[index];
}
unsigned userfuncs_getaddress(unsigned index) {
    return user_funcs[index].address;
}


avm_memcell* avm_translate_operand(vmarg *arg, avm_memcell *reg) {
    switch(arg->type) {
        case global_a: return &stack[AVM_STACKSIZE-1-arg->val];
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
            reg->data.funcVal = userfuncs_getaddress(arg->val);
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
    execute_jump,
    execute_nop
};

unsigned char execution_finished = 0;
unsigned pc = 0;
unsigned curr_line = 0;
unsigned code_size = 0;
instruction* code = (instruction*) 0;

#define AVM_ENDING_PC code_size

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

/** assign **/

extern void avm_warning(char* format,...);

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


extern void avm_assign(avm_memcell* lv, avm_memcell* rv);

void execute_assign(instruction *instr) {
    avm_memcell *lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell *rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv > &stack[top] || lv==&retval));
    assert(rv);

    avm_assign(lv,rv);
}

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

/** callin functions **/
extern void avm_error(char* format, ...);
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

extern char* avm_tostring(avm_memcell*);
extern void avm_call_libfunc(char* funcName);
extern void avm_call_save_environment(void);

void execute_call(instruction* instr) {
    avm_memcell* func = avm_translate_operand(&instr->arg1, &ax);
    assert(func);
    avm_call_save_environment();

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
        default:
            char *s = avm_tostring(func);
            avm_error("call: cannot bind "CYN"'%s'"RESET" to function!", s);
            free(s);
            execution_finished = 1;
    }
}

unsigned totalActuals = 0;

void avm_dec_top(void) {
    if(!top) {
        avm_error(BOLD RED"stack overflow" RESET "");
        execution_finished = 1;
    }else {
        --top;
    }
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




extern userfunc* avm_getfuncinfo(unsigned address);
userfunc* avm_getfuncinfo(unsigned address) {
    for(int i=0; i<total_user_funcs; ++i)
        if(user_funcs[i].address == address)
            return &user_funcs[i];
    return NULL;
}

void execute_funcenter(instruction *instr) {
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    assert(func);
    assert(pc == func->data.funcVal);

    // callee actions
    totalActuals = 0;
    topsp = top;
    userfunc* f = avm_getfuncinfo(func->data.funcVal);
    assert(f);
    top = top - f->localSize;
}

#define AVM_NUMACTUALS_OFFSET 4
#define AVM_SAVEDPC_OFFSET 3
#define AVM_SAVEDTOP_OFFSET 2
#define AVM_SAVEDTOPSP_OFFSET 1

unsigned avm_get_env_value(unsigned i) {
    
    if(stack[i].type != number_m) 
        return 0;
    
    unsigned val = (unsigned) stack[i].data.numVal;
    
    if(stack[i].data.numVal != ((double) val))
        return 0;
    
    return val;
}

void execute_funcexit(instruction* unused) {
    unsigned oldTop = top;
    top = avm_get_env_value(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_env_value(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_env_value(topsp + AVM_SAVEDTOPSP_OFFSET);

    while(++oldTop <= top)
        avm_memcell_clear(&stack[oldTop]);
}


extern void libfunc_print(void);
extern void libfunc_input(void);
extern void libfunc_objectmemberkeys(void);
extern void libfunc_objecttotalmembers(void);
extern void libfunc_objectcopy(void);
extern void libfunc_totalarguments(void);
extern void libfunc_argument(void);
extern void libfunc_typeof(void);
extern void libfunc_strtonum(void);
extern void libfunc_sqrt(void);
extern void libfunc_cos(void);
extern void libfunc_sin(void);

typedef void (*library_func_t)(void);

library_func_t libraryFuncs[] = {
    libfunc_print,
    libfunc_input,
    libfunc_objectmemberkeys,
    libfunc_objecttotalmembers,
    libfunc_objectcopy,
    libfunc_totalarguments,
    libfunc_argument,
    libfunc_typeof,
    libfunc_strtonum,
    libfunc_sqrt,
    libfunc_cos,
    libfunc_sin
};

#define TOTAL_LIBFUNCS 12

char* libraryFuncs_string[TOTAL_LIBFUNCS] = {
    "print",
    "input",
    "objectmemberkeys",
    "objecttotalmembers",
    "objectcopy",
    "totalarguments",
    "argument",
    "typeof",
    "strtonum",
    "sqrt",
    "cos",
    "sin"
};

library_func_t avm_getlibraryfunc(char* id) {
    for(int i=0; i<TOTAL_LIBFUNCS; ++i)
        if(!strcmp(libraryFuncs_string[i], id))
            return libraryFuncs[i];
    return (library_func_t) 0;
}

void avm_call_libfunc(char* id) {
    library_func_t f = avm_getlibraryfunc(id);
    if(!f) {
        avm_error("unsupported lib func '"CYN"%s"RESET"' called!", id);
        execution_finished = 1;
    }else {
        topsp = top;
        totalActuals = 0;
        (*f)();
        if(!execution_finished)
            execute_funcexit((instruction*) 0);
    }
}

unsigned avm_total_actuals(void) {
    return avm_get_env_value(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_get_actual(unsigned i) {
    assert(i < avm_total_actuals());
    return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}

char* typeStrings[] = {
    "number",
    "string",
    "bool",
    "table",
    "userfunc",
    "libfunc",
    "nil",
    "undef"
};

void libfunc_print(void) {
    unsigned n = avm_total_actuals();
    for(int i=0; i<n; ++i) {
        char* s = avm_tostring(avm_get_actual(i));
        printf("%s", s);
        free(s);
    }
}

int isNumber(char *s) {
    int dot = 0;
    for (int i = 0; s[i]!= '\0'; ++i) 
        if(!isdigit(s[i])) 
            if(s[i] == '.')
                if(dot)
                    return 0;
                else 
                    dot = 1;
            else
                return 0;
    return 1;
}

void libfunc_input(void) {
    char c;
    char* buf = malloc(sizeof(char));
    int buf_size = 0;

    while((c = getchar()) != '\n') {
        buf = realloc(buf, buf_size+2);
        buf[buf_size] = c;
        buf[buf_size+1] = '\0';
        ++buf_size;
    }

    avm_memcell_clear(&retval);

    if(isNumber(buf)) {
        retval.type = number_m;
        retval.data.numVal = strtod(buf, NULL);
    } else if(!strcmp(buf, "true")) {
        retval.type = bool_m;
        retval.data.boolVal = 1;
    } else if(!strcmp(buf, "false")) {
        retval.type = bool_m;
        retval.data.boolVal = 0;
    } else if(!strcmp(buf, "nil")) {
        retval.type = nil_m;
    } else {
        retval.type = string_m;
        if(buf[0] == '\"' && buf[buf_size-1] == '\"') {
            buf = buf+1;
            buf[buf_size-2] = '\0';
        }
        retval.data.strVal = strdup(buf);
    }
}

extern void avm_tablesetelem(avm_table* table, avm_memcell* index, avm_memcell* content);

void libfunc_objectmemberkeys(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in 'objectmemberkeys'!", n);
        retval.type = nil_m;
    } else {

        avm_memcell *arg = avm_get_actual(0);
        if(arg->type != table_m) {
            avm_warning("argument passed in 'objectmemberkeys' is not a table!"); 
            retval.type = nil_m;
            return;
        }
        
        avm_memcell key; 
        key.type = number_m;
        int index = 0;

        for(int i=0; i<AVM_TABLE_HASHSIZE-1; ++i) {
            avm_table_bucket *elem = arg->data.tableVal->numIndexed[i];
            while(elem) {
                key.data.numVal = index++;
                avm_tablesetelem(retval.data.tableVal, &key, &elem->key);
                elem = elem->next;
            }
        }

        for(int i=0; i<AVM_TABLE_HASHSIZE-1; ++i) {
            avm_table_bucket *elem = arg->data.tableVal->strIndexed[i];
            while(elem) {
                key.data.numVal = index++;
                avm_tablesetelem(retval.data.tableVal, &key, &elem->key);
                elem = elem->next;
            }
        }
    }
}

void libfunc_objecttotalmembers(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in 'objecttotalmembers'!", n);
        retval.type = nil_m;
    }else {
        avm_memcell *arg = avm_get_actual(0);
        if(arg->type != table_m) {
            avm_warning("argument passed in 'objecttotalmembers' is not a table!"); 
            retval.type = nil_m;
            return;
        }

        retval.data.numVal = arg->data.tableVal->total;
    }
}

void libfunc_objectcopy(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in 'objectcopy'!", n);
        retval.type = nil_m;
    }else {
        avm_memcell *arg = avm_get_actual(0);
        if(arg->type != table_m) {
            avm_warning("argument passed in 'objecttotalmembers' is not a table!"); 
            retval.type = nil_m;
            return;
        }
        retval.type = table_m;
        retval.data.tableVal = avm_tablenew();
        retval.data.tableVal->total = arg->data.tableVal->total;
        retval.data.tableVal->refCounter = arg->data.tableVal->refCounter;

        for(int i=0; i<AVM_TABLE_HASHSIZE-1; ++i) {
            avm_table_bucket *elem = arg->data.tableVal->numIndexed[i];
            while(elem) {
                avm_tablesetelem(retval.data.tableVal, &elem->key, &elem->value);
                elem = elem->next;
            }
        }

        for(int i=0; i<AVM_TABLE_HASHSIZE-1; ++i) {
            avm_table_bucket *elem = arg->data.tableVal->strIndexed[i];
            while(elem) {
                avm_tablesetelem(retval.data.tableVal, &elem->key, &elem->value);
                elem = elem->next;
            }
        }
    }
}

void libfunc_totalarguments(void) {
    unsigned p_topsp = avm_get_env_value(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcell_clear(&retval);

    if(p_topsp == AVM_STACKSIZE-total_global_variables-1) {
        avm_error("'"CYN"totalarguments"RESET"' called outside a function!");
        retval.type = nil_m;
    } else {
        retval.type = number_m;
        retval.data.numVal = avm_get_env_value(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

void libfunc_argument(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in "BWHT"'argument'"RESET"!", n);
        retval.type = nil_m;
    }else {
        avm_memcell *arg = avm_get_actual(0);
        if(arg->type != number_m) {
            avm_error("argument passed in "BWHT"'argument'"RESET" is not a number!");
            retval.type = nil_m; 
            return;
        }

        unsigned p_topsp = avm_get_env_value(topsp + AVM_SAVEDTOPSP_OFFSET);

        if(p_topsp == AVM_STACKSIZE-total_global_variables-1) {
            avm_error(BWHT"'argument'"RESET" called outside a function!");
            retval.type = nil_m;
        } else if(avm_get_env_value(p_topsp + AVM_NUMACTUALS_OFFSET) <= (int)arg->data.numVal) {
            retval.type = nil_m;
        }else {
            avm_assign(&retval, &stack[p_topsp + AVM_STACKENV_SIZE + 1 + (int)arg->data.numVal]);
        }
    }
}

void libfunc_typeof(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in 'typeof'!", n);
        retval.type = nil_m;    
    }else {
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_get_actual(0)->type]);
    }
}

void libfunc_strtonum(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in "BWHT"'strtonum'"RESET"!", n);
        retval.type = nil_m;   
    }else {
        avm_memcell* arg = avm_get_actual(0);
        if(arg->type != string_m) {
            avm_warning("expected argument of type string in function "BWHT"strtonum"RESET" but got %s", typeStrings[arg->type]);
            retval.type = nil_m;
        }else {
            if(isNumber(arg->data.strVal)) {
                retval.type = number_m;
                retval.data.numVal = strtod(arg->data.strVal, NULL);
            }else {
                retval.type = nil_m;
            }
        }
    }
}

void libfunc_sqrt(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in "BWHT"'sqrt'"RESET"!", n);
        retval.type = nil_m;   
    }else {
        avm_memcell* arg = avm_get_actual(0);
        if(arg->type != number_m) {
            avm_warning("expected argument of type number in function "BWHT"sqrt"RESET" but got %s", typeStrings[arg->type]);
            retval.type = nil_m;
        }else if(arg->data.numVal < 0) {
            retval.type = nil_m;
        }else {
            retval.type = number_m;
            retval.data.numVal = sqrt(arg->data.numVal);
        }
    }
}

void libfunc_cos(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in "BWHT"'cos'"RESET"!", n);
        retval.type = nil_m;   
    }else {
        avm_memcell* arg = avm_get_actual(0);
        if(arg->type != number_m) {
            avm_warning("expected argument of type number in function "BWHT"cos"RESET" but got %s", typeStrings[arg->type]);
            retval.type = nil_m;
        }else {
            retval.type = number_m;
            retval.data.numVal = cos(arg->data.numVal*(3.14159265/180));
        }
    }
}

void libfunc_sin(void) {
    unsigned n = avm_total_actuals();
    avm_memcell_clear(&retval);

    if(n!=1) {
        avm_error("one argument (not "CYN"%d"RESET") expected in "BWHT"'sin'"RESET"!", n);
        retval.type = nil_m;   
    }else {
        avm_memcell* arg = avm_get_actual(0);
        if(arg->type != number_m) {
            avm_warning("expected argument of type number in function "BWHT"sin"RESET" but got %s", typeStrings[arg->type]);
            retval.type = nil_m;
        }else {
            retval.type = number_m;
            retval.data.numVal = sin(arg->data.numVal*(3.14159265/180));
        }
    }
}

void avm_registerlibfunc(char* id, library_func_t addr);

void execute_pusharg(instruction* instr) {
    avm_memcell* arg = avm_translate_operand(&instr->arg1, &ax);
    assert(arg);

    avm_assign(&stack[top], arg);
    ++totalActuals;
    avm_dec_top();
}

typedef char* (*tostring_func_t)(avm_memcell*);
extern char* number_tostring(avm_memcell*);
extern char* string_tostring(avm_memcell*);
extern char* bool_tostring(avm_memcell*);
extern char* table_tostring(avm_memcell*);
extern char* userfunc_tostring(avm_memcell*);
extern char* libfunc_tostring(avm_memcell*);
extern char* nil_tostring(avm_memcell*);
extern char* undef_tostring(avm_memcell*);

int isInteger(double val) {
    int truncated = (int)val;
    return (val == truncated);
}

char* number_tostring(avm_memcell* m) {
    assert(m->type == number_m);
    char* result = malloc(100);
    
    if(isInteger(m->data.numVal)) 
        sprintf(result, "%d", (int)m->data.numVal);
    else 
        sprintf(result, "%.3lf", m->data.numVal);

    return result;
}

char* string_tostring(avm_memcell* m) {
    assert(m->type == string_m);
    char* result = malloc(strlen(m->data.strVal) + 1);
    strcpy(result, m->data.strVal);
    return result;
}

char* bool_tostring(avm_memcell* m) {
    assert(m->type == bool_m);
    char* result = malloc(20);
    if(m->data.boolVal) 
        strcpy(result, GREEN"true"RESET);
    else 
        strcpy(result, RED"false"RESET);
    return result;
}



char* table_bucket_tostring(avm_table_bucket *elem, char* result, int numIndexed) {
    char *key, *value, *tmp;

    while(elem) {
    
        if(elem->key.type == undef_m) {
            elem = elem->next;
            continue;
        }
        
        key = avm_tostring(&elem->key);
        value = strdup(avm_tostring(&elem->value));
        
        if(elem->value.type == string_m) {
            tmp = malloc(strlen(value)+50);
            sprintf(tmp, BBLA"\""RESET"%s"BBLA"\""RESET, value);
            free(value);
            value = tmp;
        }
        
        result = realloc(result, strlen(result)+strlen(key)+strlen(value)+100);
        if(numIndexed)
            sprintf(result, "%s %s,", result, value);
        else     
            sprintf(result, "%s "BBLU"{ "BWHT"%s "RESET": %s "BBLU"}"RESET",", result, key, value);
        free(value);
        elem = elem->next; 
    }

    return result;
}

char* table_tostring(avm_memcell* m) {
    assert(m->type == table_m);

    char* result;
    avm_table_bucket* elem;
    
    result = malloc(sizeof(char)*30);
    sprintf(result, BGRN"["RESET);

    for(int i=0; i<AVM_TABLE_HASHSIZE-1; i++) {
        elem = m->data.tableVal->numIndexed[i];
        result = table_bucket_tostring(elem, result, 1);
        elem = m->data.tableVal->strIndexed[i];
        result = table_bucket_tostring(elem, result, 0);
    }

    size_t size = strlen(result);
    if(result[size-1] == ',')
        result[size-1] = ' ';
    strcat(result, BGRN"]"RESET);
    return result;
}

char* userfunc_tostring(avm_memcell* m) {
    assert(m->type == userfunc_m);
    userfunc* f = avm_getfuncinfo(m->data.funcVal);
    char* result = malloc(strlen(f->id) + 100);
    sprintf(result, "[(user function) "CYN"'%s'"RESET", address: "BCYN"%d"RESET"]", f->id, f->address);
    return result;
}

char* libfunc_tostring(avm_memcell* m) {
    assert(m->type == libfunc_m);
    char* result = malloc(strlen(m->data.libfuncVal) + 100);
    sprintf(result, "[(library function) "CYN"'%s'"RESET"]", m->data.libfuncVal);
    return result;
}

char* nil_tostring(avm_memcell* m) {
    assert(m->type == nil_m);
    char* result = malloc(20);
    strcpy(result, BBLA"nil"RESET);
    return result;
}

char* undef_tostring(avm_memcell* m) {
    assert(m->type == undef_m);
    char* result = malloc(20);
    strcpy(result, BBLA"undefined"RESET);
    return result;
}

tostring_func_t tostringFuncs[] = {
    number_tostring,
    string_tostring,
    bool_tostring,
    table_tostring,
    userfunc_tostring,
    libfunc_tostring,
    nil_tostring,
    undef_tostring
};

char* avm_tostring (avm_memcell* m) {
    assert(m->type >= 0 && m->type <= undef_m);
    return (*tostringFuncs[m->type])(m);
}

/** arithmetic operations **/


typedef double (*arithmetic_func_t)(double x, double y);

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

    if(rv1->type != number_m || rv2->type != number_m) {
        avm_error("not a number in arithmetic!");
    }else {
        arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
        avm_memcell_clear(lv);
        lv->type = number_m;
        lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
    }
}

/** to boolean **/

typedef unsigned char (*tobool_func_t)(avm_memcell*);

unsigned char number_tobool(avm_memcell* m)   {return m->data.numVal != 0;}
unsigned char string_tobool(avm_memcell* m)   {return m->data.strVal[0] != 0;}
unsigned char bool_tobool(avm_memcell* m)     {return m->data.boolVal;}
unsigned char table_tobool(avm_memcell* m)    {return 1;}
unsigned char userfunc_tobool(avm_memcell* m) {return 1;}
unsigned char libfunc_tobool(avm_memcell* m)  {return 1;}
unsigned char nil_tobool(avm_memcell* m)      {return 0;}
unsigned char undef_tobool(avm_memcell* m)    {assert(0);return 0;}

tobool_func_t toboolFuncs[] = {
    number_tobool,
    string_tobool,
    bool_tobool,
    table_tobool,
    userfunc_tobool,
    libfunc_tobool,
    nil_tobool,
    undef_tobool
};

unsigned char avm_tobool(avm_memcell* m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}

/** eq ,neq execution **/

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
        avm_error("'"CYN"%s"RESET"' == '"CYN"%s"RESET"' is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
    else {
        switch(rv1->type) {
            case number_m: result = rv1->data.numVal == rv2->data.numVal; break;
            case string_m: result = !strcmp(rv1->data.strVal, rv2->data.strVal); break;
            case table_m: /*to dis*/ break;
            case userfunc_m: result = rv1->data.funcVal == rv2->data.funcVal; break;
            case libfunc_m: result = !strcmp(rv1->data.libfuncVal, rv2->data.libfuncVal); break;
            default: assert(0);
        }
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

/* other comparison execution */


typedef unsigned char (*comparison_func_t)(double x, double y);

unsigned char jle_impl(double x, double y) {return x<=y;}
unsigned char jge_impl(double x, double y) {return x>=y;}
unsigned char jlt_impl(double x, double y) {return x<y;}
unsigned char jgt_impl(double x, double y) {return x>y;}

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

void execute_newtable(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv > &stack[top] || lv==&retval));
    avm_memcell_clear(lv);

    lv->type = table_m;
    lv->data.tableVal = avm_tablenew();
    avm_table_incr_refcounter(lv->data.tableVal);
}

unsigned avm_tablehash(avm_memcell* index) {
    
    unsigned hash = 0;
    
    switch(index->type) {
        case number_m:
            hash = (unsigned)index->data.numVal % AVM_TABLE_HASHSIZE;
            break;
        case string_m:
        	for(int i=0; index->data.strVal[i] != '\0'; ++i)
    	        hash = (hash + index->data.strVal[i]) % AVM_TABLE_HASHSIZE;
            break;
        default: break;
    }
    return hash;
}

avm_table_bucket* avm_tablelookup(avm_table* table, avm_memcell* index) {
    
    avm_table_bucket* elem = (avm_table_bucket*) 0;
    
    unsigned hash = avm_tablehash(index);
    switch(index->type) {
        case number_m: 
            elem = table->numIndexed[hash];
            while(elem && elem->key.data.numVal != index->data.numVal) 
                elem = elem->next;
            break;
        case string_m:
            elem = table->strIndexed[hash];
            while(elem && strcmp(elem->key.data.strVal, index->data.strVal))
                elem = elem->next;
            break;
        default:
            avm_error("can't use table index of type '"CYN"%s"RESET"'", typeStrings[index->type]);
    }

    return elem;
}

avm_memcell* avm_tablegetelem(avm_table* table, avm_memcell* index) {
    
    avm_memcell* result = (avm_memcell*) 0;
    avm_table_bucket* elem = avm_tablelookup(table, index);
   
    if(elem) result = &elem->value;
    return result;
}

void execute_tablegetelem(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell* t = avm_translate_operand(&instr->arg1, (avm_memcell*) 0);
    avm_memcell* i = avm_translate_operand(&instr->arg2, &ax);

    assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv > &stack[top] || lv==&retval));
    assert(t && &stack[AVM_STACKSIZE-1] >= t && t > &stack[top]);
    assert(i);

    avm_memcell_clear(lv);
    lv->type = nil_m;

    if(t->type != table_m) {
        avm_error("illegal use of type "CYN"%s"RESET" as table!", typeStrings[t->type]);
    }else {
        avm_memcell* content = avm_tablegetelem(t->data.tableVal, i);
        if(content)
            avm_assign(lv, content);
        else {
            char* ts = avm_tostring(t);
            char* is = avm_tostring(i);
            avm_warning("key: "BWHT"%s"RESET" not found in table %s!", is, ts);
            free(ts);
            free(is);
        }
    }
}

void avm_tablesetelem(avm_table* table, avm_memcell* index, avm_memcell* content) {
    avm_table_bucket* elem = avm_tablelookup(table, index);
    unsigned hash = avm_tablehash(index);

    if(content->type == nil_m) {
        if(elem) {
            avm_memcell_clear(&elem->key);
            avm_memcell_clear(&elem->value);
        }
        if(table->total > 0) 
            table->total--;
        return;
    }

    if(elem) {
        avm_assign(&elem->value, content);
    }else {
        avm_table_bucket* tmp;
        avm_table_bucket* new_bucket = malloc(sizeof(avm_table_bucket));
        new_bucket->key.type = undef_m;
        new_bucket->value.type = undef_m;
        
        avm_assign(&new_bucket->key, index);
        avm_assign(&new_bucket->value, content);
        
        switch(index->type) {
            case number_m:
                tmp = table->numIndexed[hash];
                table->numIndexed[hash] = new_bucket;
                new_bucket->next = tmp;
                break;
            case string_m:
                tmp = table->strIndexed[hash];
                table->strIndexed[hash] = new_bucket;
                new_bucket->next = tmp;
                break;
            default:
                avm_error("can't use table index of type '"CYN"%s"RESET"'", typeStrings[index->type]);
        }

        table->total++;
    }
}

void execute_tablesetelem(instruction* instr) {
    avm_memcell* t = avm_translate_operand(&instr->result, (avm_memcell*)0);
    avm_memcell* i = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* c = avm_translate_operand(&instr->arg2, &bx);

    assert(t && &stack[AVM_STACKSIZE-1] >= t && t > &stack[top]);
    assert(i && c);

    if(t->type != table_m)
        avm_error("illegal use of type "CYN"%s"RESET" as table!", typeStrings[t->type]);
    else
        avm_tablesetelem(t->data.tableVal, i, c);
}
