

%{

#include "symtable.h"
#include "quad.h"

#define yyerror(...) custom_yyerror(__VA_ARGS__, NULL)
#define yywarning(...) custom_warning(__VA_ARGS__, NULL)
void custom_yyerror(char const *str, ...);
void custom_warning(const char *str, ...);

extern int yylex();
extern int yylineno;
extern char* yytext;

extern struct SymTable_S *symTable;
extern struct ScopeList *scopeList;
extern struct ScopeList  *funcScopeList;

unsigned loopcounter = 0;
unsigned errorcounter = 0;
unsigned warningcounter = 0;

incomplFuncJumps_t* incomplFuncJumpsTop = (incomplFuncJumps_t*) 0;
incomplFuncJumps_t* incomplFuncJumpsHead = (incomplFuncJumps_t*) 0;
incomplFuncJumps_t* incomplFuncJumpsTail = (incomplFuncJumps_t*) 0;

void pushFuncJump(unsigned label);
void pushRetJump(unsigned label);
void patchReturnJump(void);
void emit_ifnotboolexpr(expr* e);
void do_backpatching(expr* e);
int funcAsLvalue(expr *e);
int check_arith (expr* e, const char* context);
expr* do_postfixcalculation(expr* e, iopcode op);
expr* do_prefixcalculation(expr* e, iopcode op);
expr* do_arithop(iopcode op, expr* e1, expr* e2, char* context);
expr* do_relop(iopcode op, expr* e1, expr* e2, char* context);
expr* do_boolop(iopcode op, expr* e1, expr* e2, unsigned label);
SymbolTableEntry* symtable_ID(char* name); 
SymbolTableEntry* symtable_LOCAL_ID(char* name);
SymbolTableEntry* symtable_GLOBAL_ID(char* name);
SymbolTableEntry* symtable_FUNC(char* name);
SymbolTableEntry* symtable_IDLIST(char* name);

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

stmt : expr ';'  		{do_backpatching($1); resetTemp(); $$ = make_stmt();}
	 | ifstmt	 		{resetTemp(); $$ = $1;}
	 | whilestmt 		{resetTemp(); $$ = make_stmt();}
	 | forstmt 			{resetTemp(); $$ = make_stmt();}
	 | returnstmt 		{resetTemp(); $$ = make_stmt();}
	 | break 			{$$ = $1;}
	 | continue 	 	{$$ = $1;}
	 | block 			{resetTemp(); $$ = $1;}
	 | funcdef 			{resetTemp(); $$ = make_stmt();}
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
				emit(uminus, $$, $2, NULL, 0, yylineno);
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
					funcAsLvalue($1);
					do_backpatching($3);
					if($1->type == tableitem_e) {
						emit(tablesetelem, $3, $1, $1->index, 0, yylineno);
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

void pushFuncJump(unsigned label) {
	incomplFuncJumps_t* newj = malloc(sizeof(incomplFuncJumps_t));
	newj->retNum = 0;
	newj->retLabels = (unsigned*) 0;
	newj->start = label;
	newj->end = 0;
	newj->next = incomplFuncJumpsTop;
	incomplFuncJumpsTop = newj;
	if(!incomplFuncJumpsHead) {
		incomplFuncJumpsHead = newj;
		incomplFuncJumpsTail = newj;
		newj->prev = NULL;
	}else {
		incomplFuncJumpsTail->prev = newj;
		incomplFuncJumpsTail = newj;
	}
}

void pushRetJump(unsigned label) {
	if(!incomplFuncJumpsTop->retNum) 
		incomplFuncJumpsTop->retLabels = malloc(sizeof(unsigned));
	else 
		incomplFuncJumpsTop->retLabels = realloc(incomplFuncJumpsTop->retLabels, sizeof(unsigned)*(incomplFuncJumpsTop->retNum+1));
	
	incomplFuncJumpsTop->retLabels[incomplFuncJumpsTop->retNum++] = label;
}

void patchReturnJump(void) {
	for(int i=0; i<incomplFuncJumpsTop->retNum; i++) {
		patchLabel(incomplFuncJumpsTop->retLabels[i], incomplFuncJumpsTop->end);
	}
}

unsigned patchFuncJump(incomplFuncJumps_t *index) {
	int start = index->start;
	int end = index->end;

	if(end+1 == nextQuadLabel()) {
		patchLabel(start-1, end+1);
		return end+1;
	}

	if(quads[end+1].op != jump || quads[end+1].label != 0) {
		patchLabel(start-1, end+1);
		return end+1;
	}
	
	while(index && index->start < end) 
		index = index->prev;
	int patch = patchFuncJump(index);
	patchLabel(start-1, patch);	
	return patch;
}

void backpatchIncomplFuncJumps(void) {
	incomplFuncJumps_t* index = incomplFuncJumpsHead;

	while(index) {
		if(!quads[index->start-1].label) 
			patchFuncJump(index);
		index = index->prev;
	}
}



expr* do_prefixcalculation(expr* e, iopcode op) {
	expr* result;
	if(e->type == tableitem_e) {
		result = emit_iftableitem(e);
		emit(op, result, result, newexpr_constnum(1), 0, yylineno);
		emit(tablesetelem, result, e, e->index, 0, yylineno);
	}else {
		emit(op, e, e, newexpr_constnum(1), 0, yylineno);
		result = newexpr(arithexpr_e);
		result->sym = newTemp();
		emit(assign, result, e, NULL, 0, yylineno);
	}
	return result;
}

expr* do_postfixcalculation(expr* e, iopcode op) {
	expr* result = newexpr(var_e);
	result->sym = newTemp();

	if(e->type == tableitem_e) {
		expr* val = emit_iftableitem(e);
		emit(assign, result, val, NULL, 0, yylineno);
		emit(op, val, val, newexpr_constnum(1), 0, yylineno);
		emit(tablesetelem, val, e, e->index, 0, yylineno);
	}else {
		emit(assign, result, e, NULL, 0, yylineno);
		emit(op, e, e, newexpr_constnum(1), 0, yylineno);
	}
	return result;
}

void emit_ifnotboolexpr(expr* e) {
	
	if(e->type != boolexpr_e) {
		e->truelist = makeSceEntry(nextQuadLabel());
		e->falselist = makeSceEntry(nextQuadLabel()+1);
		emit(if_eq, NULL, e, newexpr_constbool(1), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
}

void do_backpatching(expr* e) {
	if(e->type == boolexpr_e) {
		backpatch(e->truelist, nextQuadLabel());
		backpatch(e->falselist, nextQuadLabel()+2);
		emit(assign, e, newexpr_constbool(1), NULL, 0, yylineno);
		emit(jump, NULL, NULL, NULL, nextQuadLabel()+2, yylineno);
		emit(assign, e, newexpr_constbool(0), NULL, 0, yylineno);
	}
}

int check_arith (expr* e, const char* context) {
	if ( e->type == constbool_e   ||
		 e->type == conststring_e ||
		 e->type == nil_e 		  ||
		 e->type == newtable_e 	  ||
		 e->type == programfunc_e ||
		 e->type == libraryfunc_e ||
		 e->type == boolexpr_e ) 
	{
		char *type;
		const char *name = "";
		switch(e->type) {
			case constbool_e:   
				type = "constant boolean"; 
				name = (e->val.boolConst) ? "'true'" : "'false'";	
				break;
			case conststring_e: 
				type = "constant string"; 
				name = e->val.strConst;
				break;
			case programfunc_e: 
				type = "function"; 
				name = e->sym->value.funcVal->name;
				break;
			case libraryfunc_e: 
				type = "library function"; 
				name = e->sym->value.funcVal->name;
				break;
			case nil_e: 	 	type = "nil"; break;
			case newtable_e: 	type = "table"; break;
			case boolexpr_e:  	type = "boolean expression"; break;
			default: assert(0);
		}
			
		yyerror("Invalid use of ", context, " on ", type, " ", CYN, name,RESET);
		return 1;
	}
	return 0;
}

expr* do_arithop(iopcode op, expr* e1, expr* e2, char* context) {
	expr* result;
	check_arith(e1, context); 
	check_arith(e2, context);
	if(e1->type == constnum_e && e2->type == constnum_e) {
		switch(op) {
			case add: 		result = newexpr_constnum(e1->val.numConst + e2->val.numConst); break;
			case sub: 		result = newexpr_constnum(e1->val.numConst - e2->val.numConst); break;
			case mul: 		result = newexpr_constnum(e1->val.numConst * e2->val.numConst); break;
			case division: 	result = newexpr_constnum(e1->val.numConst / e2->val.numConst); break;
			case mod: 		result = newexpr_constnum((int)e1->val.numConst % (int)e2->val.numConst); break;
			default: assert(0);
		}
	}else {
		result = newexpr(arithexpr_e);
		if(istempexpr(e1))
			result->sym = e1->sym;
		else if(istempexpr(e2))
			result->sym = e2->sym;
		else
			result->sym = newTemp();
		emit(op, result, e1, e2, 0, yylineno);
	}
	return result;
}

expr* do_relop(iopcode op, expr* e1, expr* e2, char* context) {
	expr* result = newexpr(boolexpr_e);
	if(op != if_eq && op != if_noteq) {
		check_arith(e1, context);
		check_arith(e2, context);
	}
	if(istempexpr(e1))
		result->sym = e1->sym;
	else if(istempexpr(e2))
		result->sym = e2->sym;
	else
		result->sym = newTemp();

	result->truelist = makeSceEntry(nextQuadLabel());
	result->falselist = makeSceEntry(nextQuadLabel()+1);
	shortcircuit_t *listtrue = mergeSceEntries(e1->truelist, e2->truelist);
	shortcircuit_t *listfalse = mergeSceEntries(e1->falselist, e2->falselist);
	result->truelist = mergeSceEntries(result->truelist, listtrue);
	result->falselist = mergeSceEntries(result->falselist, listfalse);
	
	emit(op, NULL, e1, e2, 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);

	return result;
}

expr* do_boolop(iopcode op, expr* e1, expr* e2, unsigned label) {
	expr* result = newexpr(boolexpr_e);
	if(istempexpr(e1))
		result->sym = e1->sym;
	else if(istempexpr(e2))
		result->sym = e2->sym;
	else
		result->sym = newTemp();
	
	emit_ifnotboolexpr(e2);

	if(op == or) {
		backpatch(e1->falselist, label);
		result->truelist = mergeSceEntries(e1->truelist, e2->truelist);
		result->falselist = e2->falselist;
	}else {
		backpatch(e1->truelist, label);
		result->truelist = e2->truelist;
		result->falselist = mergeSceEntries(e1->falselist, e2->falselist);
	}

	return result;
}

int funcAsLvalue(expr *e) {
	if(e && (e->type == programfunc_e || e->type == libraryfunc_e)) {
		yyerror("syntax error, can't change value of function ", CYN, e->sym->value.varVal->name, RESET, ", functions are constant");
		return 1;
	}
	return 0;
}

SymbolTableEntry* symtable_ID(char* name) {
	SymbolTableEntry *e;
	
	if(!(e = lookupAll(name, current_scope))) {
		e = insert(name, yylineno, LOCALVAR);
		e->space = currScopeSpace();
		e->offset = currScopeOffset();
		incCurrScopeOffset();
	}else if(e->value.varVal->scope && e->type != USERFUNC && !inFunc(e)) {
		char *line = malloc(sizeof(char)*(int)log10(e->value.varVal->line));
		sprintf(line, "%d", e->value.varVal->line);
		yyerror("syntax error, cannot access ", symbolTypeToString(e->type)," ", CYN, e->value.varVal->name, RESET ," declared at line ", line);
		free(line);
		return NULL;
	}
	return e;
}

SymbolTableEntry* symtable_LOCAL_ID(char* name) {
	SymbolTableEntry *e;
	
	if(current_scope != 0 && libraryCollision(name)) {
		yyerror("syntax error, collision with library function: ", CYN, name, RESET);
		return NULL;
	}

	if(!(e = lookup(name, current_scope))) {
		e = insert(name, yylineno, LOCALVAR);
		e->space = currScopeSpace();
		e->offset = currScopeOffset();
		incCurrScopeOffset();
	} else if(e->type == USERFUNC || e->type == LIBFUNC) {
		yywarning("keyword local useless on functions");
	}
	
	return e;
}

SymbolTableEntry* symtable_GLOBAL_ID(char* name) {
	SymbolTableEntry *e;
	if(!(e = lookup(name, 0))) {
		yyerror("syntax error, global variable ", CYN, name, RESET, " undeclared (first use here)");
		return NULL;
	}
	return e;
}

SymbolTableEntry* symtable_FUNC(char* name) {
	SymbolTableEntry *e;
	if(libraryCollision(name)) {
		yyerror("syntax error, collision with library function: ", CYN, name, RESET);
		return newSymEntry(name, yylineno, USERFUNC, current_scope, func_scope);
	}
	if(e = lookup(name, current_scope)) {
		char *line = malloc(sizeof(char)*(int)log10(e->value.varVal->line));
		sprintf(line, "%d", e->value.varVal->line);
		yyerror("syntax error, redeclared ", CYN, name, RESET, ", first declared as ", symbolTypeToString(e->type), " at line ", line);
		free(line);
		return newSymEntry(name, yylineno, USERFUNC, current_scope, func_scope);
	}
	return insert(name, yylineno, USERFUNC);
}

SymbolTableEntry* symtable_IDLIST(char* name) {
	if(libraryCollision(name)) {
		yyerror("syntax error, collision with library function: ", CYN, name, RESET);
		return NULL;
	}
	if(lookup(name, current_scope)) {
		yyerror("syntax error, formal argument ", CYN, name, RESET, " redeclared");
		return NULL;
	}
	SymbolTableEntry *e = insert(name, yylineno, FORMALVAR);
	e->space = currScopeSpace();
	e->offset = currScopeOffset();
	incCurrScopeOffset();
	return e;
}

void initSymTable(void) {
	symTable = SymTable_new();
	scopeList = newScopeList();
	funcScopeList = newScopeList();
	insertLibFuncs();
	anonymousFuncBuf = malloc(sizeof(char));
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

int main (void) {
	
	initSymTable();
	yyparse();
	
	if(warningcounter) {
		printf("\n\n");
		if(warningcounter>1)
			printf(MAGENTA BOLD"Parsing completed with %d warnings..."RESET, warningcounter);
		else 
			printf(MAGENTA BOLD"Parsing completed with 1 warning..."RESET);
	}
	if(errorcounter) {
		printf("\n\n");
		if(errorcounter>1)
			printf(RED BOLD"Parsing completed with %d errors..."RESET, errorcounter);
		else 
			printf(RED BOLD"Parsing completed with 1 error..."RESET);
		printf("\n\n");
		return 1;
	}
	if(!warningcounter)
		printf(GREEN BOLD"\n\nParsing completed successfully..." RESET);
	
	backpatchIncomplFuncJumps();
	printScopeList(scopeList);
	printQuads();

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
