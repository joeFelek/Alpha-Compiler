

%{

#include "Alpha_Compiler/Symtable/symtable.h"
#include "Alpha_Compiler/Quads/quad.h"
#include "Alpha_Compiler/Final_Code/final_code.h"

#define yyerror(...) custom_yyerror(__VA_ARGS__, NULL)
#define yywarning(...) custom_warning(__VA_ARGS__, NULL)
void custom_yyerror(char const *str, ...);
void custom_warning(const char *str, ...);

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE *yyin;
void yyrestart(FILE *);

extern struct SymTable_S *symTable;
extern struct ScopeList *scopeList;
extern struct ScopeList  *funcScopeList;

unsigned loopcounter = 0;
unsigned errorcounter = 0;
unsigned warningcounter = 0;

%}

%define parse.error verbose
%define parse.lac full

%code requires {
	extern unsigned errorcounter;
	extern unsigned warningcounter;
}

%union {
	double numVal;
	char* strVal;
	struct SymbolTableEntry *symbol;
	struct expr *exprNode;
	struct call_s *Call;
	struct indexed_s *indexedVal;
	struct forloop_i *forloopVal;
	struct stmt_t *stmtVal;
}

%token<numVal> 
	NUMBER "number"
%token<strVal>
	STRING "string" ID "identifier"
%token
	IF "if" ELSE "else" WHILE "while" FOR "for" FUNCTION "function" 
	RETURN "return" BREAK "break" CONTINUE "continue" AND "and" 
	NOT "not" OR "or" LOCAL "local" TRUE "true" FALSE "false" NIL "nil"
	COLON_COLON "::" DOT_DOT ".." EQUAL "==" NOT_EQUAL "!=" 
	PLUS_PLUS "++" MINUS_MINUS "--" GREATER_EQUAL ">=" LESS_EQUAL "<=" 	


%right '='
%left OR
%left AND
%nonassoc "==" "!="
%nonassoc '>' ">=" '<' "<=" 
%left '+' '-'
%left '*' '/' '%'
%right NOT "++" "--" UMINUS
%left '.' ".."
%left '[' ']'
%left '(' ')'

%nonassoc IF 
%nonassoc ELSE

%type<strVal> funcname
%type<numVal> funcbody ifprefix elseprefix whilestart whilecond N M W
%type<symbol> funcprefix funcdef
%type<exprNode> lvalue member primary assignexpr call term objectdef const expr elist
%type<Call> callsuffix normcall methodcall
%type<indexedVal> indexed indexedelem
%type<forloopVal> forprefix
%type<stmtVal> stmt stmts loopstmt break continue block ifstmt

%start program

%%

program	: /*empty*/		{}
		| stmts 		{free($1);}
		;

stmts :	stmt 			
			{
				$$ = make_stmt();
				$$->breaklist = $1->breaklist;
				$$->contlist = $1->contlist;
				free($1);
			}
	  |	stmts stmt		
			{
				$1->breaklist = mergelist($1->breaklist, $2->breaklist);
				$1->contlist = mergelist($1->contlist, $2->contlist);
				$$ = $1;
				free($2);
			}
	  ;	

stmt : expr ';'  		{do_backpatching($1); $$ = make_stmt();}
	 | ifstmt	 		{$$ = $1;}
	 | whilestmt 		{$$ = make_stmt();}
	 | forstmt 			{$$ = make_stmt();}
	 | returnstmt 		{$$ = make_stmt();}
	 | break 			{$$ = $1;}
	 | continue 	 	{$$ = $1;}
	 | block 			{$$ = $1;}
	 | funcdef 			{$$ = make_stmt();}
	 | ';' 				{resetTemp(); $$ = make_stmt();}
	 | error			{yyclearin; resetTemp(); $$ = make_stmt();}
	 ;

expr : assignexpr 		{$$ = $1;}
	 | expr '+' expr 	{$$ = do_arithop(add, $1, $3, "addition");}
	 | expr '-' expr  	{$$ = do_arithop(sub, $1, $3, "subtraction");}
	 | expr '*' expr 	{$$ = do_arithop(mul, $1, $3, "multiplication");}
	 | expr '%' expr 	{$$ = do_arithop(mod, $1, $3, "modulo");}
	 | expr '/' expr 	{$$ = do_arithop(division, $1, $3, "division");}
	 | expr '>' expr 	{$$ = do_relop(if_greater, $1, $3, "greater");}
	 | expr ">=" expr 	{$$ = do_relop(if_greatereq, $1, $3, "greater or equal");}
	 | expr '<' expr 	{$$ = do_relop(if_less, $1, $3, "less");}
	 | expr "<=" expr 	{$$ = do_relop(if_lesseq, $1, $3, "less or equal");}
	 | expr "==" {do_backpatching($1);} expr {do_backpatching($4); $$ = do_relop(if_eq, $1, $4, "");}
	 | expr "!=" {do_backpatching($1);} expr {do_backpatching($4); $$ = do_relop(if_noteq, $1, $4, "");}
	 | expr AND {emit_ifnotboolexpr($1);} W expr {$$ = do_boolop(and, $1, $5, $4);}
	 | expr OR  {emit_ifnotboolexpr($1);} W expr {$$ = do_boolop(or, $1, $5, $4);}
	 | term  {$$ = $1;}
	 ;
		
W :  {$$ = nextQuadLabel();};


term 	: '(' expr ')'	  {$$ = $2;}
	 	| primary 		  {$$ = $1;}
		| "++" lvalue	  {check_arith($2, "left increment");  $$ = do_prefixcalculation($2, add);}
	 	| "--" lvalue 	  {check_arith($2, "left decrement");  $$ = do_prefixcalculation($2, sub);}
	 	| lvalue "++" 	  {check_arith($1, "right increment"); $$ = do_postfixcalculation($1, add);}
	 	| lvalue "--" 	  {check_arith($1, "right decrement"); $$ = do_postfixcalculation($1, sub);}
	 	
		| '-' expr %prec UMINUS 	
	 		{
				check_arith($2, "unary operation");
				$$ = newexpr(arithexpr_e);
				$$->sym = istempexpr($2) ? $2->sym : newTemp();
				emit(mul, $$, $2, newexpr_constnum(-1), 0, yylineno);
			}

		| NOT expr 				
			{
				$$ = newexpr(boolexpr_e);
				$$->sym = istempexpr($2) ? $2->sym : newTemp();
				if($2->type != boolexpr_e) {
					$$->truelist = makeSceEntry(nextQuadLabel()+1);
					$$->falselist = makeSceEntry(nextQuadLabel());
					emit(if_eq, NULL, $2, newexpr_constbool(1), 0, yylineno);
					emit(jump, NULL, NULL, NULL, 0, yylineno);
				} else {
					$$->truelist = $2->falselist;
					$$->falselist = $2->truelist;
				}
			}
		;

assignexpr  : lvalue '=' expr 	
				{
					if($1 && ($1->type == programfunc_e || $1->type == libraryfunc_e)) 
						yyerror("syntax error, can't change value of function ", CYN, $1->sym->value.funcVal->name, RESET, ", functions are constant");
					do_backpatching($3);
					if($1->type == tableitem_e) {
						emit(tablesetelem, $1, $1->index, $3, 0, yylineno);
						$$ = emit_iftableitem($1);
						$$->type = assignexpr_e;
					} else {
						emit(assign, $1, $3, NULL, 0, yylineno);
						$$ = newexpr(assignexpr_e);
						$$->sym = newTemp();
						emit(assign, $$, $1, NULL, 0, yylineno);
					}
				}
		    ;	

primary : call  			{$$ = $1;}
		| objectdef 		{$$ = $1;}
		| const 			{$$ = $1;}
		| lvalue  			{$$ = emit_iftableitem($1);}
		| '(' funcdef ')' 	{$$ = newexpr(programfunc_e); $$->sym = $2;}
		;

lvalue  : member 	  {$$ = $1;}
	    | ID 	      {SymbolTableEntry *sym; $$ = (sym = symtable_ID($1)) ? lvalue_expr(sym) : newexpr(nil_e);}
	    | LOCAL ID 	  {SymbolTableEntry *sym; $$ = (sym = symtable_LOCAL_ID($2)) ? lvalue_expr(sym) : newexpr(nil_e);}
	    | "::" ID 	  {SymbolTableEntry *sym; $$ = (sym = symtable_GLOBAL_ID($2)) ? lvalue_expr(sym) : newexpr(nil_e);}
	    ;

member  : lvalue '.' ID		{$$ = member_item($1, $3);}
	    | call '.' ID		{$$ = member_item($1, $3);}
	    
		| lvalue '[' expr ']'	
			{
				do_backpatching($3);
				$1 = emit_iftableitem($1);
				$$ = newexpr(tableitem_e);
				$$->sym = $1->sym;
				$$->index = $3;
			}	 

	    | call '[' expr ']'      
	   		{
				do_backpatching($3);
				$1 = emit_iftableitem($1);
				$$ = newexpr(tableitem_e);
				$$->sym = $1->sym;
				$$->index = $3;
			}
	    ;

call 	: call '(' ')'			{$$ = make_call($1, NULL);}
	 	| call '(' elist ')'	{$$ = make_call($1, $3);}

	 	| lvalue callsuffix 				
	 		{
				$1 = emit_iftableitem($1);
				if($2->method) {
					expr* t = $1;
					$1 = emit_iftableitem(member_item(t, $2->name));
					if(!$2->elist)
						$2->elist = t;
					else {
						expr* e;
						for(e = $2->elist; e->next; e = e->next);
						e->next = t;
					}
				}
				$$ = make_call($1, $2->elist);
				free($2);
			}

		| '(' funcdef ')' '(' ')'			
		 	{
				expr* func = newexpr(programfunc_e);
				func->sym = $2;
				$$ = make_call(func, NULL);
			}
	
		| '(' funcdef ')' '(' elist ')'	
		 	{
				expr* func = newexpr(programfunc_e);
				func->sym = $2;
				$$ = make_call(func, $5);
			}
		;

callsuffix : normcall		{$$ = $1;}
		   | methodcall		{$$ = $1;}
		   ;

normcall   : '(' ')'		{$$ = new_call(NULL, 0, NULL);}
		   | '(' elist ')'	{$$ = new_call($2, 0, NULL);}
		   ;

methodcall : ".." ID '(' ')'		{$$ = new_call(NULL, 1, $2);}
		   | ".." ID '(' elist ')'	{$$ = new_call($4, 1, $2);}

elist : expr				{do_backpatching($1); $$ = $1;}
	  | elist ',' expr		{do_backpatching($3); $3->next = $1; $$ = $3;}
	  ;

objectdef 	: '[' ']'				
				{
					expr* t = newexpr(newtable_e);
					t->sym = newTemp();
					emit(tablecreate, t, NULL, NULL, 0, 0);
					$$ = t;
				}

		  	| '[' elist ']'		
		  		{
					expr* t = newexpr(newtable_e);
					t->sym = newTemp();
					emit(tablecreate, t, NULL, NULL, 0, 0);
					int len = -1;
					for(expr* e=$2; e; e=e->next) ++len;
					for(int i=len; $2; $2 = $2->next)
						emit(tablesetelem, t, newexpr_constnum(len--), $2, 0, 0);
					$$ = t;

				}

		  	| '[' indexed ']'		
		  		{
					expr* t = newexpr(newtable_e);
					t->sym = newTemp();
					emit(tablecreate, t, NULL, NULL, 0, 0);
					int count = 0;
					for(indexed_s *i = $2; i; i = i->next) {
						emit(tablesetelem, t, i->index, i->value, 0, 0);
					}
					$$ = t;
					free($2);
				}
		    ;

indexed : indexedelem				{$$ = $1;}
		| indexed ',' indexedelem	{$3->next = $1;$$ = $3;}
		;

indexedelem : '{' expr ':' expr '}'		
				{
					do_backpatching($2);
					do_backpatching($4);
					$$ = malloc(sizeof(indexed_s));
					$$->index = $2;
					$$->value = $4;
					$$->next = (indexed_s*) 0;
				}
            ;

block : '{' '}'						{hide(current_scope+1); $$=make_stmt();}
	  | '{' {SCOPEUP;} stmts '}'	{hide(current_scope); $$=$3; SCOPEDOWN;}
	  ;

funcname : ID 			{$$ = $1;}
		 | /*empty*/	{$$ = newTempFuncName();}
		 ;

funcprefix : FUNCTION funcname
				{
					emit(jump, NULL, NULL, NULL, 0, yylineno);
					pushFuncJump(nextQuadLabel());
					$$ = symtable_FUNC($2);
					$$->value.funcVal->iaddress = nextQuadLabel();
					expr* e = newexpr(programfunc_e);
					e->sym = $$;
					emit(funcstart, e, NULL, NULL, 0, yylineno);
					pushCurrScopeOffset();
					enterScopeSpace();
					resetFormalArgsOffset();
				}
		   ;

funcargs : '(' ')' 
			{
				enterScopeSpace();
				resetFunctionLocalsOffset();
				pushCurrLoopCounter(loopcounter);
			}
         | '(' idlist ')'
		 	{	
				enterScopeSpace();
				resetFunctionLocalsOffset();
				pushCurrLoopCounter(loopcounter);
			}
		 ;

funcbody : block 
			{
				$$ = currScopeOffset();
				exitScopeSpace();
				loopcounter = popLoopCounter();
			}
		 ;

funcdef : funcprefix funcargs {loopcounter = 0; FUNCUP;} funcbody {FUNCDOWN;}
			{
				exitScopeSpace();
				$1->value.funcVal->totalLocals = $4;
				int offset = popScopeOffset();
				restoreCurrScopeOffset(offset);
				expr* e = newexpr(programfunc_e);
				e->sym = $1;
				$$ = $1; 
				incomplFuncJumpsTop->end = nextQuadLabel();
				patchReturnJump();
				incomplFuncJumpsTop = incomplFuncJumpsTop->next;
				emit(funcend, e, NULL, NULL, 0, yylineno);
			}
        ;

const : NUMBER		{$$ = newexpr_constnum($1);}
	  | STRING 		{$$ = newexpr_conststring($1);}
	  | NIL 		{$$ = newexpr(nil_e);}
	  | TRUE 		{$$ = newexpr_constbool(1);}
	  | FALSE		{$$ = newexpr_constbool(0);}
	  ;

idlist : ID				{SCOPEUP; FUNCUP; symtable_IDLIST($1); SCOPEDOWN; FUNCDOWN;}
	   | idlist ',' ID	{SCOPEUP; FUNCUP; symtable_IDLIST($3); SCOPEDOWN; FUNCDOWN;}
	   ;

ifprefix : IF '(' expr ')'  	
			{
				do_backpatching($3);
				emit(if_eq, NULL, $3, newexpr_constbool(1), nextQuadLabel()+2, yylineno);
				$$ = nextQuadLabel();
				emit(jump, NULL, NULL, NULL, 0, yylineno);
			}
		 ;

elseprefix : ELSE	 {$$ = nextQuadLabel(); emit(jump, NULL, NULL, NULL, 0, yylineno);}
		   ;

ifstmt : ifprefix %prec IF stmt 	{patchLabel($1, nextQuadLabel()); $$ = ($2) ? $2 : make_stmt();}

	   | ifprefix %prec IF stmt elseprefix stmt		
	   		{
				patchLabel($1, $3+1);
				patchLabel($3, nextQuadLabel());
				$$ = make_stmt();
				if($2 && $4) {
					$$->breaklist = mergelist($2->breaklist, $4->breaklist);
					$$->contlist = mergelist($2->contlist, $4->contlist);
				}else if($2)
					$$ = $2;
				else if($4)
					$$ = $4;
			}
	   ;

whilestart : WHILE 		{$$ = nextQuadLabel();}
		   ;

whilecond : '(' expr ')'	
				{
					do_backpatching($2);
					emit(if_eq, NULL, $2, newexpr_constbool(1), nextQuadLabel()+2, yylineno);
					$$ = nextQuadLabel();
					emit(jump, NULL, NULL, NULL, 0, yylineno);
				}
		  ;

whilestmt : whilestart whilecond loopstmt		
				{
					emit(jump, NULL, NULL, NULL, $1, yylineno);
					patchLabel($2, nextQuadLabel());
					patchlist($3->breaklist, nextQuadLabel());
					patchlist($3->contlist, $1);
				}
		  ;

N : {$$ = nextQuadLabel(); emit(jump, NULL, NULL, NULL, 0, yylineno);};
M : {$$ = nextQuadLabel();};
E : /*empty*/ | elist ;

forprefix : FOR '(' E ';' M expr ';' 
				{
					do_backpatching($6);
					$$ = malloc(sizeof(forloop_i));
					$$->test = $5;
					$$->enter = nextQuadLabel();
					emit(if_eq, NULL, $6, newexpr_constbool(1), 0, yylineno);
				}
		  ;

forstmt   : forprefix N E ')' N loopstmt N	
				{
					patchLabel($1->enter, $5+1);
					patchLabel($2, nextQuadLabel());
					patchLabel($5, $1->test);
					patchLabel($7, $2+1);
					patchlist($6->breaklist, nextQuadLabel());
					patchlist($6->contlist, $2+1);
					free($1);
				}
          ;

returnstmt : RETURN ';'			
				{
					if(!func_scope)
						yyerror("return statement not within function");
					else 
						pushRetJump(nextQuadLabel()+1);

					emit(ret, NULL, NULL, NULL, 0, yylineno);
					emit(jump, NULL, NULL, NULL, 0, yylineno);
				}
           | RETURN expr ';'	
		   		{
					do_backpatching($2);
					if(!func_scope)
						yyerror("return statement not within function");
					else
						pushRetJump(nextQuadLabel()+1);

					emit(ret, $2, NULL, NULL, 0, yylineno);
					emit(jump, NULL, NULL, NULL, 0, yylineno);
				}

loopstmt : {++loopcounter;} stmt {--loopcounter; $$ = $2 ? $2 : make_stmt();}

break : BREAK ';' 
			{
				if(!loopcounter)
					yyerror("break statement not within loop");
				$$ = make_stmt();
				$$->breaklist = newlist(nextQuadLabel());
				emit(jump, NULL, NULL, NULL, 0, yylineno);
			}

continue : CONTINUE ';'
			{
				if(!loopcounter)
					yyerror("continue statement not within loop");
				$$ = make_stmt();
				$$->contlist = newlist(nextQuadLabel()); 
				emit(jump, NULL, NULL, NULL, 0, yylineno);
			}

%%          

void freeExpr(expr* e) {
	for(int i=0; e && i<currQuad; ++i) {
		if(quads[i].arg1 == e)
			quads[i].arg1 = NULL;
		if(quads[i].arg2 == e)
			quads[i].arg2 = NULL;
		if(quads[i].result == e)
			quads[i].result = NULL;
	}
	free(e);
}

void freeQuads(void) {
	for(int i=0; i<currQuad; ++i) {
		freeExpr(quads[i].arg1);
		quads[i].arg1 = NULL;
		freeExpr(quads[i].arg2);
		quads[i].arg2 = NULL;
		freeExpr(quads[i].result);
		quads[i].result = NULL;
	}
	free(quads);
}

void freeSymbols(void) {
	for(int i=0; i<scopeList->len; i++) {
		SymbolTableEntry *e = scopeList->scopes[i].head;
		while(e) {
			SymbolTableEntry *tmp;
			tmp = e->next_inScope;

			if(e->type == USERFUNC || e->type == LIBFUNC) { 
				free((void*)e->value.funcVal->name);
				free(e->value.funcVal);
			} else { 
				free((void*)e->value.varVal->name);
				free(e->value.varVal);
			}
			free(e);
			e = tmp;
		}
	}
}

void freeFuncJumps(void) {
	incomplFuncJumps_t* index = incomplFuncJumpsHead;

	while(index) {
		incomplFuncJumps_t* tmp = index->prev;
		if(index->retLabels)
			free(index->retLabels);
		free(index);
		index = tmp;
	}
}

void freeAll(void) {	
	freeQuads();
	free(anonymousFuncBuf);
	freetempname();
	freeStacks();
	freeFuncJumps();
	freeSymbols();
	free(scopeList->scopes);
	free(scopeList);
	free(funcScopeList->scopes);
	free(funcScopeList);
	free(symTable);
}

int main (int argc, char **argv) {
	
	yyin = stdin;
	
	for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (!strcmp(argv[i], "-o")) ++i;
            continue;
        }
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            fprintf(stderr, "error: cannot open '%s'\n", argv[i]);
            return 1;
        }
        yyin = f;
        break;
    }
	
	yyrestart(yyin);

	initSymTable();
	yyparse();
	
	if(warningcounter) {
		printf("\n\n");
		if(warningcounter>1)
			printf(MAGENTA BOLD"Compiled with %d warnings..."RESET, warningcounter);
		else 
			printf(MAGENTA BOLD"Compiled with 1 warning..."RESET);
	}
	if(errorcounter) {
		printf("\n\n");
		if(errorcounter>1)
			printf(RED BOLD"Compiled with %d errors..."RESET, errorcounter);
		else 
			printf(RED BOLD"Compiled with 1 error..."RESET);
		printf("\n\n");
		return 1;
	}
	if(!warningcounter) 
		printf(GREEN BOLD"\n\nCompiled successfully" RESET "\n\n");

	backpatchIncomplFuncJumps();
	generateTCode();

	int output_flag = 0;

	if(argc > 1) {
		for(int i=1; i<argc; ++i) {
			if(!strcmp(argv[i], "-s"))
				printScopeList(scopeList);
			if(!strcmp(argv[i], "-q"))
				printQuads();
			if(!strcmp(argv[i], "-o"))
				output_flag = i;
			if(!strcmp(argv[i], "-i")) {
				print_instructions();
				print_num_consts();
				print_string_consts();
				print_user_funcs();
				print_named_lib_funcs();
			}
		}
	}

	if(output_flag) {
		if(argc < output_flag + 1) {
			fprintf(stderr, RED BOLD"error: "RESET"-o flag was given but no name was specified");
			return -1;
		}
		generateBitCode(argv[output_flag+1]);
	}else {
		generateBitCode("alpha.abc");
	}

	freeAll();
	return 0;
}

void custom_yyerror(const char *str, ...) {
    va_list ap;

	fprintf(stderr, "\n"BOLD"%d"RESET": "BOLD RED"error"RESET":  ", yylineno);

    va_start(ap, str);
    while(str) {
        fprintf(stderr, "%s", str);
        str = va_arg(ap, const char*);
    }
    fprintf(stderr, "\n");
    va_end(ap);

	++errorcounter;
} 

void custom_warning(const char *str, ...) {
	va_list ap;

	fprintf(stderr, "\n"BOLD"%d"RESET": "BOLD MAGENTA"warning"RESET":  ", yylineno);

	va_start(ap, str);
	while(str) {
        fprintf(stderr, "%s", str);
        str = va_arg(ap, const char*);
    }
    fprintf(stderr, "\n");
    va_end(ap);

	++warningcounter;
}
