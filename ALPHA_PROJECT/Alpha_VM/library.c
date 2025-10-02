#include "avm.h"

#define PI 3.14159265
#define TOTAL_LIBFUNCS 12

extern void execute_funcexit(instruction* instr);

unsigned avm_total_actuals(void) {
    return avm_get_env_value(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_get_actual(unsigned i) {
    assert(i < avm_total_actuals());
    return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}

void libfunc_print(void) {
    unsigned n = avm_total_actuals();
    for(int i=0; i<n; ++i) {
        char* s = avm_tostring(avm_get_actual(i));
        printf("%s", s);
        free(s);
    }
    avm_memcell_clear(&retval);
    retval.type = nil_m;
}

int isNumber(char *s) {
    int dot = 0;
    for (int i = 0; s[i] != '\0'; ++i) {
        if (isdigit(s[i])) continue;

        if (s[i] == '.') {
            if (dot) return 0;
            dot = 1;
        } else {
            return 0;
        }
    }
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
        retval.data.strVal = strdup(buf);
    }
    free(buf);
}

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
        
        avm_memcell *result = malloc(sizeof(avm_memcell));
        result->type = table_m;
        result->data.tableVal = avm_tablenew();

        avm_memcell* key = malloc(sizeof(avm_memcell)); 
        key->type = number_m;
        int index = 0;

        for(int i=0; i<AVM_TABLE_HASHSIZE; ++i) {
            avm_table_bucket *elem = arg->data.tableVal->numIndexed[i];
            while(elem) {
                key->data.numVal = index++;
                avm_tablesetelem(result->data.tableVal, key, &elem->key);
                elem = elem->next;
            }
        }

        for(int i=0; i<AVM_TABLE_HASHSIZE; ++i) {
            avm_table_bucket *elem = arg->data.tableVal->strIndexed[i];
            while(elem) {
                key->data.numVal = index++;
                avm_tablesetelem(result->data.tableVal, key, &elem->key);
                elem = elem->next;
            }
        }

        avm_assign(&retval, result);
        free(result);
        free(key);
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
        retval.type = number_m;
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

        for(int i=0; i<AVM_TABLE_HASHSIZE; ++i) {
            avm_table_bucket *elem = arg->data.tableVal->numIndexed[i];
            while(elem) {
                avm_tablesetelem(retval.data.tableVal, &elem->key, &elem->value);
                elem = elem->next;
            }
        }

        for(int i=0; i<AVM_TABLE_HASHSIZE; ++i) {
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
        retval.data.strVal = strdup(memcell_type_tostring[avm_get_actual(0)->type]);
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
            avm_warning("expected argument of type string in function "BWHT"strtonum"RESET" but got %s", memcell_type_tostring[arg->type]);
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
            avm_warning("expected argument of type number in function "BWHT"sqrt"RESET" but got %s", memcell_type_tostring[arg->type]);
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
            avm_warning("expected argument of type number in function "BWHT"cos"RESET" but got %s", memcell_type_tostring[arg->type]);
            retval.type = nil_m;
        }else {
            retval.type = number_m;
            retval.data.numVal = cos(arg->data.numVal*PI/180);
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
            avm_warning("expected argument of type number in function "BWHT"sin"RESET" but got %s", memcell_type_tostring[arg->type]);
            retval.type = nil_m;
        }else {
            retval.type = number_m;
            retval.data.numVal = sin(arg->data.numVal*PI/180);
        }
    }
}

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

library_func_t avm_getlibraryfunc(char* id) {
    for(int i=0; i<TOTAL_LIBFUNCS; ++i)
        if(!strcmp(libraryFuncs_tostring[i], id))
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