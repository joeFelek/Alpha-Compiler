%option noyywrap
%option yylineno
%option stack


KEYWORD_ (if|else|while|for|function|return|break|continue|and|not|or|local|true|false|nil)
OPERATOR_ (=|\+|-|\*|\/|%|==|!=|\+\+|--|>|<|<=|>=)
INTCONST_ (0[xX][0-9A-Fa-f]+)|([0-9]+)
REALCONST_ [0-9]+\.[0-9](e|E)?[0-9]*
PUNCTUATION_ (\{|\}|\[|\]|\(|\)|\;|\,|\:|\:\:|\.|\.\.)
ID_ [a-zA-Z_][a-zA-Z_0-9]*

%x STRING_ ESC_ COMMENT_


%{
    #include "parser.h"
    #include <stdarg.h>

    #define RESET   "\033[0m"
    #define BOLD    "\033[1m" 
    #define RED     "\033[31m"  
    #define GREEN   "\033[32m"    
    #define MAGENTA "\033[35m"      
    #define WHITE   "\033[37m"
    #define CYN     "\033[1m\033[36m"

    #define yyerror(...) print_yyerror(__VA_ARGS__, NULL)
    #define yywarning(...) print_warning(__VA_ARGS__, NULL)
    void print_yyerror(char const *str, ...);
    void print_warning(const char *str, ...);

    /* Tracks the tokens number */
    int tokenCount = 0;
    /* Buffer that keeps string tokens contents */
    char *content_temp_buf = NULL;
    /* Pointer to content_temp_buf for adding new chars */
    char *content_temp_ptr = NULL;
    /* Keeps block comment starting line */
    int block_comment_start;
    /* Stack that keeps nested comments starting lines */
    int* nested_comment_stack = NULL;
    int nested_comment_len = 0;

    int getKeywordValue(void);
    int getOperatorValue(void);
    int getPunctuationValue(void);
%}


%% 


[ \n\t\r]+ /* ignore whitespace */

{KEYWORD_}  {
    return getKeywordValue();
}  

{OPERATOR_} {
    return getOperatorValue();
}

{INTCONST_} {
    yylval.numVal = atoi(yytext);
    return NUMBER;
}

{REALCONST_} {
    yylval.numVal= atof(yytext);
    return NUMBER;
}

{PUNCTUATION_} {
    return getPunctuationValue();
}

{ID_} {
    yylval.strVal = strdup(yytext);    
    return ID;
}

\" { /* enter string condition */
    content_temp_buf = malloc(sizeof(char));
    content_temp_ptr = content_temp_buf;
    *content_temp_ptr = '\0';
    BEGIN(STRING_);
}

<STRING_>\" {
    BEGIN(INITIAL);
    yylval.strVal = strdup(content_temp_buf);
    free(content_temp_buf);
    return STRING;
}

<STRING_>\n { 
    BEGIN(INITIAL);
    yyerror("missing terminating \" character in string ", CYN, "\"", content_temp_buf, RESET);
    free(content_temp_buf);
    return YYerror;
}

<STRING_><<EOF>> { 
    BEGIN(INITIAL);
    yyerror("missing terminating \" character in string ", CYN, "\"", content_temp_buf, RESET);
    free(content_temp_buf);
    return YYerror;
}

<STRING_>\\  {
    content_temp_buf = realloc(content_temp_buf, strlen(content_temp_buf) + 2);
    content_temp_ptr = content_temp_buf + strlen(content_temp_buf);
    yy_push_state(ESC_);
}

<STRING_>[^\\\n\"\r]+ {
    char *yy = yytext;
    content_temp_buf = realloc(content_temp_buf, strlen(content_temp_buf) + strlen(yytext) + 1);
    content_temp_ptr = content_temp_buf + strlen(content_temp_buf);
    while (*yy) {
        *content_temp_ptr++ = *yy++;
    }
    *content_temp_ptr = '\0'; 
}

<ESC_>a              *content_temp_ptr++ = '\a';*content_temp_ptr = '\0';yy_pop_state();
<ESC_>b              *content_temp_ptr++ = '\b';*content_temp_ptr = '\0';yy_pop_state();
<ESC_>e              *content_temp_ptr++ = '\e';*content_temp_ptr = '\0';yy_pop_state();
<ESC_>f              *content_temp_ptr++ = '\f';*content_temp_ptr = '\0';yy_pop_state();
<ESC_>n              *content_temp_ptr++ = '\n';*content_temp_ptr = '\0';yy_pop_state();
<ESC_>r              *content_temp_ptr++ = '\r';*content_temp_ptr = '\0';yy_pop_state();
<ESC_>t              *content_temp_ptr++ = '\t';*content_temp_ptr = '\0';yy_pop_state();
<ESC_>v              *content_temp_ptr++ = '\v';*content_temp_ptr = '\0';yy_pop_state();
<ESC_>(\\|\'|\"|\?)  *content_temp_ptr++ = *yytext;*content_temp_ptr = '\0';yy_pop_state();

<ESC_>[xX][0-9A-Fa-f]{1,2} {
    long v = strtol(yytext + 1, NULL, 16);
    content_temp_buf = realloc(content_temp_buf, strlen(content_temp_buf) + 2);
    content_temp_ptr = content_temp_buf + strlen(content_temp_buf);
    *content_temp_ptr++ = (char)(v & 0xFF);
    *content_temp_ptr = '\0';
    yy_pop_state();
}

<ESC_>[xX] {
    yywarning("unknown escape sequence: ", CYN, "'\\x'", RESET, " in string ", CYN, "\"", content_temp_buf, "\"", RESET);
    content_temp_buf = realloc(content_temp_buf, strlen(content_temp_buf) + 2);
    content_temp_ptr = content_temp_buf + strlen(content_temp_buf);
    *content_temp_ptr++ = 'x';
    *content_temp_ptr = '\0';
    yy_pop_state();
}

<ESC_>[0-7]{1,3} {
    long v = strtol(yytext, NULL, 8);
    if (v == 0) {
        yywarning("escape produces NUL (\\0) which is unsupported in Alpha strings; ignoring");
        yy_pop_state();
    } else {
        content_temp_buf = realloc(content_temp_buf, strlen(content_temp_buf) + 2);
        content_temp_ptr = content_temp_buf + strlen(content_temp_buf);
        *content_temp_ptr++ = (char)(v & 0xFF);
        *content_temp_ptr = '\0';
        yy_pop_state();
    }
}

<ESC_>\n {
    BEGIN(INITIAL);
    yyerror("missing terminating \" character in string ", CYN, "\"", content_temp_buf,"\\", RESET);
    free(content_temp_buf);
    return YYerror;
}            

<ESC_><<EOF>> {
    BEGIN(INITIAL);
    yyerror("missing terminating \" character in string ", CYN, "\"", content_temp_buf,"\\", RESET);
    free(content_temp_buf);
    return YYerror;
}       

<ESC_>. {
    char* s = malloc(5);
    sprintf(s, "'\\%c'", *yytext);
    yywarning("unknown escape sequence: ", CYN, s, RESET, " in string ", CYN, "\"", content_temp_buf, "\"", RESET);
    free(s);
    yy_pop_state();
}   


"//".*  {  }

"/*"    { /* start block comment condition */
    block_comment_start = yylineno; 
    nested_comment_stack = malloc(sizeof(int));
    nested_comment_len = 0;
    yy_push_state(COMMENT_);
}    

<COMMENT_>"/"+"*" {
    nested_comment_len++;
    nested_comment_stack = realloc(nested_comment_stack, nested_comment_len*sizeof(int));
    *(nested_comment_stack+nested_comment_len-1) = yylineno;
    yy_push_state(COMMENT_);
}

<COMMENT_>"*"+"/" {  
    yy_pop_state();
}

<COMMENT_>\n|(.)    /* eat body */

<COMMENT_><<EOF>> {
    int line;
    if(yy_top_state() == INITIAL)
        line = block_comment_start;
    else 
        line = *(nested_comment_stack+nested_comment_len-1);
    yyerror("unterminated comment");
    free(nested_comment_stack);
    BEGIN(INITIAL);
    return YYerror;
}

.   {yywarning("found unknown character ", CYN, yytext, RESET);}


%%

int getKeywordValue() {
    if(!strcmp(yytext, "if")) return IF;
    if(!strcmp(yytext, "else")) return ELSE;
    if(!strcmp(yytext, "while")) return WHILE;
    if(!strcmp(yytext, "for")) return FOR;
    if(!strcmp(yytext, "function")) return FUNCTION;
    if(!strcmp(yytext, "return")) return RETURN;
    if(!strcmp(yytext, "break")) return BREAK;
    if(!strcmp(yytext, "continue")) return CONTINUE;
    if(!strcmp(yytext, "and")) return AND;
    if(!strcmp(yytext, "not")) return NOT;
    if(!strcmp(yytext, "or")) return OR;
    if(!strcmp(yytext, "local")) return LOCAL;
    if(!strcmp(yytext, "true")) return TRUE;
    if(!strcmp(yytext, "false")) return FALSE;
    if(!strcmp(yytext, "nil")) return NIL;
    return -1;
}

int getOperatorValue() {
    if(!strcmp(yytext, "==")) return EQUAL;
    if(!strcmp(yytext, "!=")) return NOT_EQUAL;
    if(!strcmp(yytext, "++")) return PLUS_PLUS;
    if(!strcmp(yytext, "--")) return MINUS_MINUS;
    if(!strcmp(yytext, ">=")) return GREATER_EQUAL;
    if(!strcmp(yytext, "<=")) return LESS_EQUAL;
    if(*yytext == '=') return '=';
    if(*yytext == '+') return '+';
    if(*yytext == '-') return '-';
    if(*yytext == '*') return '*';
    if(*yytext == '/') return '/';
    if(*yytext == '%') return '%';
    if(*yytext == '>') return '>';
    if(*yytext == '<') return '<';
    return -1;
}

int getPunctuationValue() {
    if(!strcmp(yytext, "::")) return COLON_COLON;
    if(!strcmp(yytext, "..")) return DOT_DOT;
    if(*yytext == '{') return '{';
    if(*yytext == '}') return '}';
    if(*yytext == '[') return '[';
    if(*yytext == ']') return ']';
    if(*yytext == '(') return '(';
    if(*yytext == ')') return ')';
    if(*yytext == ';') return ';';
    if(*yytext == ',') return ',';
    if(*yytext == ':') return ':';
    if(*yytext == '.') return '.';
    return -1;
}


void print_yyerror(const char *str, ...) {
    va_list ap;

	fprintf(stderr, "\n"BOLD"%d"RESET": "BOLD RED"error"RESET":  syntax error, ", yylineno);

    va_start(ap, str);
    while(str) {
        fprintf(stderr, "%s", str);
        str = va_arg(ap, const char*);
    }
    fprintf(stderr, "\n");
    va_end(ap);

	++errorcounter;
} 

void print_warning(const char *str, ...) {
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
