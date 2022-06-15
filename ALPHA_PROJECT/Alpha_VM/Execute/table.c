#include "../avm.h"


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
    t->numIndexed = malloc(sizeof(avm_table_bucket)*AVM_TABLE_HASHSIZE);
    t->strIndexed = malloc(sizeof(avm_table_bucket)*AVM_TABLE_HASHSIZE);

    t->refCounter = 0;
    t->total = 0;

    avm_table_buckets_init(t->numIndexed);
    avm_table_buckets_init(t->strIndexed);
    // TODO: add other table types

    return t;
}

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
    free(t->strIndexed);
    free(t->numIndexed);
    free(t);
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
            avm_error("can't use table index of type '"CYN"%s"RESET"'", memcell_type_tostring
        [index->type]);
    }

    return elem;
}

avm_memcell* avm_tablegetelem(avm_table* table, avm_memcell* index) {
    
    avm_memcell* result = (avm_memcell*) 0;
    avm_table_bucket* elem = avm_tablelookup(table, index);
   
    if(elem) result = &elem->value;
    return result;
}

void avm_tablesetelem(avm_table* table, avm_memcell* index, avm_memcell* content) {
    avm_table_bucket* elem = avm_tablelookup(table, index);
    if(execution_finished) return;
    unsigned hash = avm_tablehash(index);
    printf("hash: %d\n", hash);
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
        
        if(index->type == number_m) {
            tmp = table->numIndexed[hash];
            table->numIndexed[hash] = new_bucket;
            new_bucket->next = tmp;
        } else {
            tmp = table->strIndexed[hash];
            table->strIndexed[hash] = new_bucket;
            new_bucket->next = tmp;
        }

        table->total++;
    }
}

void execute_newtable(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv > &stack[top] || lv==&retval));
    avm_memcell_clear(lv);

    lv->type = table_m;
    lv->data.tableVal = avm_tablenew();
    avm_table_incr_refcounter(lv->data.tableVal);
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
        avm_error("illegal use of type "CYN"%s"RESET" as table!", memcell_type_tostring
    [t->type]);
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

void execute_tablesetelem(instruction* instr) {
    avm_memcell* t = avm_translate_operand(&instr->result, (avm_memcell*)0);
    avm_memcell* i = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* c = avm_translate_operand(&instr->arg2, &bx);

    assert(t && &stack[AVM_STACKSIZE-1] >= t && t > &stack[top]);
    assert(i && c);

    if(t->type != table_m)
        avm_error("illegal use of type "CYN"%s"RESET" as table!", memcell_type_tostring
    [t->type]);
    else
        avm_tablesetelem(t->data.tableVal, i, c);
}