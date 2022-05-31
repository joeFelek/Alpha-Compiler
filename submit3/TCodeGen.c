#include "avm.h"


#define EXPAND_SIZE 1024
#define CURR_SIZE   (total_instructions*sizeof(instruction))
#define NEW_SIZE    (EXPAND_SIZE*sizeof(instruction)+CURR_SIZE)

instruction* Tcode = (instruction*) 0;
unsigned total_instructions = 0;
unsigned curr_instruction = 0;

void expand_instructions(void) {
    instruction* t = (instruction*) malloc(NEW_SIZE);
    if (Tcode) {
        memcpy(t, Tcode, CURR_SIZE);
        free(Tcode);
    }
    Tcode = t;
    total_instructions += EXPAND_SIZE;
}

void emit_instruction(instruction t) {

    if (curr_instruction == total_instructions)
        expand_instructions();

    Tcode[curr_instruction].opcode = t.opcode;
    Tcode[curr_instruction].result = t.result;
    Tcode[curr_instruction].arg1 = t.arg1;
    Tcode[curr_instruction].arg2 = t.arg2;
    Tcode[curr_instruction].srcLine = curr_instruction;
    ++curr_instruction;
}

void init_vmarg(vmarg *arg) {
    arg->type = -1;
    arg->val = -1;
}

void init_instuction(instruction *t) {
    t->result.type = -1;
    t->result.val = -1;
    t->arg1.type = -1;
    t->arg1.val = -1;
    t->arg2.type = -1;
    t->arg2.val = -1;
}

char* vmopcode_toString[24] = {
    "assign",       "add",          "sub",
    "mul",          "div",          "mod", 
    "jeq",          "jne",          "jle",
    "jge",          "jlt",          "jgt",
    "call",         "pusharg",      "funcenter",    
    "funcexit",     "newtable",     "tablegetelem", 
    "tablesetelem", "jump",         "nop"
};

double *numConsts = (double*) 0;
unsigned totalNumConsts = 0;

char **stringConsts = (char**) 0;
unsigned totalStringConsts = 0;

char **namedLibfuncs = (char**) 0;
unsigned totalNamedLibFuncs = 0;

userfunc *userFuncs = (userfunc*) 0;
unsigned totalUserFuncs = 0;


unsigned consts_newstring(char *str) {
    for(unsigned i=0; i<totalStringConsts; ++i) 
        if(!strcmp(stringConsts[i], str))
            return i;

    if (!totalStringConsts)
        stringConsts = malloc(sizeof(char*));
    else 
        stringConsts = realloc(stringConsts, sizeof(char*)*(totalStringConsts+1));

    stringConsts[totalStringConsts] = strdup(str);
    return totalStringConsts++;
}   

unsigned consts_newnumber(double num) {
    for(unsigned i=0; i<totalNumConsts; ++i) 
        if(numConsts[i] == num)
            return i;

    if (!totalNumConsts)
        numConsts = malloc(sizeof(double));
    else 
        numConsts = realloc(numConsts, sizeof(double)*(totalNumConsts+1));
    
    numConsts[totalNumConsts] = num; 
    return totalNumConsts++;
}

unsigned userfuncs_newfunc(SymbolTableEntry *sym) {
    for(unsigned i=0; i<totalUserFuncs; ++i) 
        if(userFuncs[i].address == sym->value.funcVal->iaddress)
            return i;
        
    if (!totalUserFuncs)
        userFuncs = malloc(sizeof(userfunc));
    else 
        userFuncs = realloc(userFuncs, sizeof(userfunc)*(totalUserFuncs+1));
    
    userFuncs[totalUserFuncs].address = sym->value.funcVal->iaddress;
    userFuncs[totalUserFuncs].localSize = sym->value.funcVal->totalLocals;
    userFuncs[totalUserFuncs].id = sym->value.funcVal->name;  

    return totalUserFuncs++;
}

unsigned libfuncs_newused(const char *name) {
    for(unsigned i=0; i<totalNamedLibFuncs; ++i) 
        if(!strcmp(namedLibfuncs[i], name))
            return i;

    if (!totalNamedLibFuncs)
        namedLibfuncs = malloc(sizeof(char*));
    else 
        namedLibfuncs = realloc(namedLibfuncs, sizeof(char*)*(totalNamedLibFuncs+1));

    namedLibfuncs[totalNamedLibFuncs] = strdup(name);
    return totalNamedLibFuncs++;
}

void make_operand(expr* e, vmarg* arg) {
    if(!e) {
        init_vmarg(arg);
        return;
    }

    switch(e->type) {
        case var_e:
        case assignexpr_e:
        case tableitem_e:
        case arithexpr_e:
        case boolexpr_e:
        case newtable_e:
            assert(e->sym);
            arg->val = e->sym->offset;

            switch(e->sym->space) {
                case PROGRAMVAR:    arg->type = global_a; break;
                case FUNCTIONLOCAL: arg->type = local_a; break;
                case FORMALARG:     arg->type = formal_a; break;
                default: assert(0);
            }
            break;
        case constbool_e:
            arg->type = bool_a;
            arg->val = (unsigned) e->val.boolConst;
            break;
        case conststring_e:
            arg->type = string_a;
            arg->val = consts_newstring(e->val.strConst);
            break;
        case constnum_e:
            arg->type = number_a;
            arg->val = consts_newnumber(e->val.numConst);
            break;
        case nil_e:
            arg->type = number_a;
            break;
        case programfunc_e:
            arg->type = userfunc_a;
            arg->val = userfuncs_newfunc(e->sym);
            break;
        case libraryfunc_e:
            arg->type = libfunc_a;
            arg->val = libfuncs_newused(e->sym->value.funcVal->name);
            break;
        default: assert(0);
    }   
}

void generate(vmopcode op, quad q) {
    instruction t;
    init_instuction(&t);
    t.opcode = op;
    make_operand(q.arg1, &t.arg1);
    make_operand(q.arg2, &t.arg2);
    make_operand(q.result, &t.result);
    emit_instruction(t);
}

void generate_relational(vmopcode op, quad q) {
    instruction t;
    init_instuction(&t);
    t.opcode = op;
    make_operand(q.arg1, &t.arg1);
    make_operand(q.arg2, &t.arg2);
    t.result.type = label_a;
    t.result.val = q.label;
    emit_instruction(t);
}

void generate_ADD(quad q) {generate(add_v, q);}
void generate_SUB(quad q) {generate(sub_v, q);}
void generate_MUL(quad q) {generate(mul_v, q);}
void generate_DIV(quad q) {generate(div_v, q);}
void generate_MOD(quad q) {generate(mod_v, q);}

void generate_NEWTABLE(quad q)     {generate(newtable_v, q);}
void generate_TABLEGETELEM(quad q) {generate(tablegetelem_v, q);}
void generate_TABLESETELEM(quad q) {generate(tablesetelem_v, q);}
void generate_ASSIGN(quad q)       {generate(assign_v, q);}

void generate_NOP(void) {instruction t; t.opcode = nop_v; emit_instruction(t);}

void generate_JUMP(quad q)         {generate_relational(jump_v, q);}
void generate_IF_EQ(quad q)        {generate_relational(jeq_v, q);}
void generate_IF_NOTEQ(quad q)     {generate_relational(jne_v, q);}
void generate_IF_GREATER(quad q)   {generate_relational(jgt_v, q);}
void generate_IF_GREATEREQ(quad q) {generate_relational(jge_v, q);}
void generate_IF_LESS(quad q)      {generate_relational(jlt_v, q);}
void generate_IF_LESSEQ(quad q)    {generate_relational(jle_v, q);}

void generate_PARAM(quad q) {
    instruction t;
    init_instuction(&t);
    t.opcode = pusharg_v;
    make_operand(q.arg1, &t.arg1);
    emit_instruction(t);
}

void generate_CALL(quad q) {
    instruction t;
    init_instuction(&t);
    t.opcode = call_v;
    make_operand(q.arg1, &t.arg1);
    emit_instruction(t);
}

void generate_GETRETVAL(quad q) {
    instruction t;
    init_instuction(&t);
    t.opcode = assign_v;
    make_operand(q.result, &t.result);
    t.arg1.type = retval_a;
    emit_instruction(t);
}

void generate_FUNCSTART(quad q) {
    instruction t;
    init_instuction(&t);
    t.opcode = funcenter_v;
    make_operand(q.result, &t.result);
    emit_instruction(t);
}

void generate_FUNCEND(quad q) {
    instruction t;
    init_instuction(&t);
    t.opcode = funcexit_v;
    make_operand(q.result, &t.result);
    emit_instruction(t);
}

void generate_RETURN(quad q) {
    instruction t;
    init_instuction(&t);
    t.opcode = assign_v;
    t.result.type = retval_a;
    make_operand(q.result, &t.arg1);
    emit_instruction(t);
}

void (*const generators[22])(quad) = 
{
    generate_ASSIGN,
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_LESSEQ,
    generate_IF_GREATEREQ,
    generate_IF_LESS,
    generate_IF_GREATER,
    generate_CALL,
    generate_PARAM,
    generate_RETURN,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_FUNCEND,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_JUMP
};

void generateTCode(void) {
    for(unsigned i = 0; i<currQuad; ++i) {
        (*generators[quads[i].op])(quads[i]);
    }
}

void generateBitCode(char* output_name) {
    char* out_name = malloc(strlen(output_name)+4);
    sprintf(out_name, "%s.abc", output_name);
    FILE *f = fopen(out_name, "wb");

    unsigned magic_number = 340200501;
    fwrite(&magic_number, sizeof(unsigned), 1, f);

    // write string array
    fwrite(&totalStringConsts, sizeof(unsigned), 1, f);
    for(int i=0; i<totalStringConsts; ++i) {
        unsigned size = strlen(stringConsts[i]) + 1;
        fwrite(&size, sizeof(unsigned), 1, f);
        fwrite(stringConsts[i], size, 1, f);    
    }

    // write numbers array
    fwrite(&totalNumConsts, sizeof(unsigned), 1, f);
    for(int i=0; i<totalNumConsts; ++i) {
        fwrite(&numConsts[i], sizeof(double), 1, f);
    }

    // write userfunctions array
    fwrite(&totalUserFuncs, sizeof(unsigned), 1, f);
    for(int i=0; i<totalUserFuncs; ++i) {
        unsigned size = strlen(userFuncs[i].id) + 1;
        fwrite(&userFuncs[i].address, sizeof(unsigned), 1, f);
        fwrite(&userFuncs[i].localSize, sizeof(unsigned), 1, f);
        fwrite(&size, sizeof(unsigned), 1, f);
        fwrite(userFuncs[i].id, size, 1, f);
    }

    // write libfunctions array
    fwrite(&totalNamedLibFuncs, sizeof(unsigned), 1, f);
    for(int i=0; i<totalNamedLibFuncs; ++i) {
        unsigned size = strlen(namedLibfuncs[i]) + 1;
        fwrite(&size, sizeof(unsigned), 1, f);
        fwrite(namedLibfuncs[i], size, 1, f);
    }

    // write instructions
    fwrite(&curr_instruction, sizeof(unsigned), 1, f);
    for(int i=0; i<curr_instruction; ++i) {
        instruction t = Tcode[i];
        fwrite(&t.opcode, sizeof(int), 1, f);
        fwrite(&t.result.type, sizeof(int), 1, f);
        fwrite(&t.result.val, sizeof(unsigned), 1, f);
        fwrite(&t.arg1.type, sizeof(int), 1, f);
        fwrite(&t.arg1.val, sizeof(unsigned), 1, f);
        fwrite(&t.arg2.type, sizeof(int), 1, f);
        fwrite(&t.arg2.val, sizeof(unsigned), 1, f);
    }

    fclose(f);
    free(out_name);
}

char *iopcodeString[25] = {
    "assign",         "add",            "sub", 
    "mul",            "div",	        "mod", 
	"if_eq",          "if_noteq",		"if_lesseq",
	"if_greatereq",   "if_less", 		"if_greater",
	"call",           "param",			"return",
	"getretval",      "funcstart", 		"funcend",
	"tablecreate",	  "tablegetelem",   "tablesetelem",
	"jump", 		  "and",            "or", 			
};

char *type_to_string[] = {
    "label", 
    "global", 
    "formal", 
    "local", 
    "number", 
    "string", 
    "bool", 
    "nil", 
    "userfunc", 
    "libfunc", 
    "retval"
};

void print_instructions(void) {
    char s[100];

    printf("\n\n");
    printf("instr\topcode\t\tresult%-15carg1%-15carg2\n", ' ', ' ');
	printf("-----------------------------------------------------------------------------------------------\n");

    for(int i=0; i<curr_instruction; i++) {
        instruction c = Tcode[i];
        printf("%-7d %-15s", c.srcLine, vmopcode_toString[c.opcode]);

            if(c.result.type != -1) {
                if(c.result.val != -1)
                    sprintf(s, "%d(%s), %d", c.result.type, type_to_string[c.result.type], c.result.val);
                else 
                    sprintf(s, "%d(%s)", c.result.type, type_to_string[c.result.type]);
                printf(" %-20s", s);
            }else {
                printf(" %-20c", ' ');
            }

            if(c.arg1.type != -1) {
                if(c.arg1.val != -1)
                    sprintf(s, "%d(%s), %d", c.arg1.type, type_to_string[c.arg1.type], c.arg1.val);
                else
                    sprintf(s, "%d(%s)", c.arg1.type, type_to_string[c.arg1.type]);
                printf(" %-20s", s);
            }else {
                printf(" %-20c", ' ');
            }
            
            if(c.arg2.type != -1) {
                sprintf(s, "%d(%s), %d", c.arg2.type, type_to_string[c.arg2.type], c.arg2.val);
                printf(" %-20s", s);
            }else { 
                printf(" %-20c", ' ');
            }

        printf("\n");
    }
}

#define a_c_r     "\x1b[31m"
#define a_c_g     "\x1b[32m"
#define a_c_y     "\x1b[33m"
#define a_c_b     "\x1b[34m"
#define a_c_m     "\x1b[35m"
#define a_c_c     "\x1b[36m"
#define a_c_re    "\x1b[0m"
#define under     "\e[4m"
#define under_re  "\e[0m"
#define italic    "\e[3m"
#define italic_re "\e[0m"
#define bold      "\e[1m"
#define bold_re   "\e[0m"

void PRINTER_NUM(void){

	int i = 0;
	printf(a_c_r bold under"PINAKAS ARITHMITIKON STATHERON\n"under_re bold_re a_c_re);

	for(i = 0; i<totalNumConsts; i++){
		printf(a_c_b "|" a_c_re);
		printf(a_c_b italic "%d" italic_re a_c_re, i);
		printf(a_c_b "| %lf\n" a_c_re, numConsts[i]);
	}
	printf("\n");

} 

void PRINTER_STR(void){

	int i = 0;
	printf(a_c_r bold under"PINAKAS STATHERON STRINGS\n"under_re bold_re a_c_re);

	for(i = 0; i<totalStringConsts; i++){
		printf(a_c_b "|" a_c_re);
		printf(a_c_b italic "%d" italic_re a_c_re, i);
		printf(a_c_b "| %s\n" a_c_re, stringConsts[i]);
	}
	printf("\n");
}

void PRINTER_USERFUNC(void){

	int i = 0;
	printf(a_c_r bold under"PINAKAS SUNARTISEON XRISTI\n"under_re bold_re a_c_re);

	for(i = 0; i<totalUserFuncs; i++){
		printf(a_c_b "|" a_c_re);
		printf(a_c_b italic "%d" italic_re a_c_re, i);
		printf(a_c_b "| address %d, localSize %d, id %s\n" a_c_re,userFuncs[i].address,userFuncs[i].localSize,userFuncs[i].id);
	}
	printf("\n");

}

void PRINTER_LIB(void){

	int i = 0;
	printf(a_c_r bold under"PINAKAS SUNARTISEON VIVLIOTHIKIS\n"under_re bold_re a_c_re);

	for(i = 0; i<totalNamedLibFuncs; i++){
		printf(a_c_b "|" a_c_re);
		printf(a_c_b italic "%d" italic_re a_c_re, i);
		printf(a_c_b "| %s\n" a_c_re, namedLibfuncs[i]);
	}
	printf("\n");

}