/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 53 "parser.y"

	extern unsigned errorcounter;
	extern unsigned warningcounter;

#line 54 "parser.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* "number"  */
    STRING = 259,                  /* "string"  */
    ID = 260,                      /* "identifier"  */
    IF = 261,                      /* "if"  */
    ELSE = 262,                    /* "else"  */
    WHILE = 263,                   /* "while"  */
    FOR = 264,                     /* "for"  */
    FUNCTION = 265,                /* "function"  */
    RETURN = 266,                  /* "return"  */
    BREAK = 267,                   /* "break"  */
    CONTINUE = 268,                /* "continue"  */
    AND = 269,                     /* "and"  */
    NOT = 270,                     /* "not"  */
    OR = 271,                      /* "or"  */
    LOCAL = 272,                   /* "local"  */
    TRUE = 273,                    /* "true"  */
    FALSE = 274,                   /* "false"  */
    NIL = 275,                     /* "nil"  */
    COLON_COLON = 276,             /* "::"  */
    DOT_DOT = 277,                 /* ".."  */
    EQUAL = 278,                   /* "=="  */
    NOT_EQUAL = 279,               /* "!="  */
    PLUS_PLUS = 280,               /* "++"  */
    MINUS_MINUS = 281,             /* "--"  */
    GREATER_EQUAL = 282,           /* ">="  */
    LESS_EQUAL = 283,              /* "<="  */
    UMINUS = 284                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define NUMBER 258
#define STRING 259
#define ID 260
#define IF 261
#define ELSE 262
#define WHILE 263
#define FOR 264
#define FUNCTION 265
#define RETURN 266
#define BREAK 267
#define CONTINUE 268
#define AND 269
#define NOT 270
#define OR 271
#define LOCAL 272
#define TRUE 273
#define FALSE 274
#define NIL 275
#define COLON_COLON 276
#define DOT_DOT 277
#define EQUAL 278
#define NOT_EQUAL 279
#define PLUS_PLUS 280
#define MINUS_MINUS 281
#define GREATER_EQUAL 282
#define LESS_EQUAL 283
#define UMINUS 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 58 "parser.y"

	double numVal;
	char* strVal;
	struct SymbolTableEntry *symbol;
	struct expr *exprNode;
	struct call_s *Call;
	struct indexed_s *indexedVal;
	struct forloop_i *forloopVal;
	struct stmt_t *stmtVal;

#line 143 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
