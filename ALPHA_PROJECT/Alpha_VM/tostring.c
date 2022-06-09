#include "avm.h"


char* memcell_type_tostring[] = {
    "number",
    "string",
    "bool",
    "table",
    "userfunc",
    "libfunc",
    "nil",
    "undef"
};

char* libraryFuncs_tostring[] = {
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
        sprintf(result, "%s"BBLU"{"BWHT"%s "RESET": %s"BBLU"}"RESET", ", result, key, value);
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

    for(int i=0; i<AVM_TABLE_HASHSIZE; i++) {
        elem = m->data.tableVal->numIndexed[i];
        result = table_bucket_tostring(elem, result, 1);
    }

    for(int i=0; i<AVM_TABLE_HASHSIZE; ++i) {
        elem = m->data.tableVal->strIndexed[i];
        result = table_bucket_tostring(elem, result, 0);
    }

    size_t size = strlen(result);
    if(result[size-2] == ',')
        result[size-2] = '\0';
    strcat(result, BGRN"]"RESET);
    return result;
}

char* userfunc_tostring(avm_memcell* m) {
    assert(m->type == userfunc_m);
    userfunc* f = avm_getfuncinfo(m->data.funcVal);
    char* result = malloc(strlen(f->id) + 100);
    sprintf(result, "(user function "CYN"'%s'"RESET" with address: "BCYN"%d"RESET")", f->id, f->address);
    return result;
}

char* libfunc_tostring(avm_memcell* m) {
    assert(m->type == libfunc_m);
    char* result = malloc(strlen(m->data.libfuncVal) + 100);
    sprintf(result, "(library function "CYN"'%s'"RESET")", m->data.libfuncVal);
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

typedef char* (*tostring_func_t)(avm_memcell*);
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