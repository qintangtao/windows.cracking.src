/***************************************************************************
        libbf - Free BrainFuck interpreter, optimizer, compiler library
                            -------------------
    copyright            : (C) 2005-2006 by Even Rouault
    email                : even 'dot' rouault 'at' mines-paris 'dot' org
***************************************************************************/

/*
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
 
#include "libbf_list.h"

#include "libbf_internal.h"

#define CONST const
 
static CONST char* c2bf_skip_comment(CONST char* code, int* numLine)
{
  CONST char* c = code;
  while(*c)
  {
    if (*c == '\n')
    {
      if (numLine) (*numLine) ++;
    }
    else if (*c == '*' && c[1] == '/')
    {
      return c + 2;
    }
    c++;
  }
  fprintf(stderr, "No ending C comment\n");
  return c;
}

typedef struct
{
  char* token;
  int   numLine;
} Token;

static Token* c2bf_create_token(CONST char* begin, CONST char* end, int numLine)
{
  Token* token = alloc0(Token, 1);
  token->token = alloc0(char, end - begin + 1);
  token->numLine = numLine;
  memcpy(token->token, begin, end - begin);
  return token;
}

static CONST char* oneCarTokens[] = {"(", ")", "{", "}", "[", "]", "=", "<", ">", "!", "+", "-", "*", "/", "%", "~", "^", "&", "|", ";", ",", "?", ":" };
static CONST char* twoCarTokens[] = {"<=", ">=", "!=", "==", "+=", "-=", "*=", "/=", "%=", "&=", "^=", "|=", "++", "--", "<<", ">>", "&&", "||" };
static CONST char* threeCarTokens[] = {"<<=", ">>="};

static CONST char* cKeywords[] = { "while", "if", "else", "return", "do", "for", "continue", "break", "goto", "int", "float", "double", "char", "long" };

static int c2bf_is_keyword(CONST char* token)
{
  unsigned int i;
  for(i=0;i<sizeof(cKeywords) / sizeof(char*); i++)
  {
    if (strcmp(cKeywords[i], token) == 0)
    {
      return TRUE;
    }
  }
  return FALSE; 
}

static CONST char*  c2bf_isKnownToken(CONST char* c)
{
  unsigned int i;
  if (c[1] != 0)
  {
    if (c[2] != 0)
    {
      for(i=0;i<sizeof(threeCarTokens) / sizeof(char*); i++)
      {
        if (c[0] == threeCarTokens[i][0] &&
            c[1] == threeCarTokens[i][1] &&
            c[2] == threeCarTokens[i][2])
          return threeCarTokens[i];
      }
    }
    for(i=0;i<sizeof(twoCarTokens) / sizeof(char*); i++)
    {
      if (c[0] == twoCarTokens[i][0] &&
          c[1] == twoCarTokens[i][1])
        return twoCarTokens[i];
    }
  }
  for(i=0;i<sizeof(oneCarTokens) / sizeof(char*); i++)
  {
    if (c[0] == oneCarTokens[i][0])
      return oneCarTokens[i];
  }
  return NULL; 
}

static List* c2bf_create_token_list(CONST char* code)
{
  CONST char* c = code;
  CONST char* beginToken = NULL;
  Token* token = NULL;
  CONST char* knownToken = NULL;
  List* tokenList = NULL;
  int numLine = 1;
  
  while(*c)
  {
    if (*c == '/' && c[1] == '*')
    {
      c = c2bf_skip_comment(c+2, &numLine);
      continue;
    }
    else if (*c == '/' && c[1] == '/')
    {
      while(*c) { if (*c == '\n') break; c++; }
      continue;
    }
    else if (*c == '"')
    {
      beginToken = c;
      c++;
      while(*c)
      {
        if (*c == '\\') c++;
        else if (*c == '"') {c++; break; }
        c++;
      }
      if (*c)
      {
        token = c2bf_create_token(beginToken, c, numLine);
        tokenList = list_append(tokenList, token);
        beginToken = NULL;
      }
      continue;
    }
    else if (*c == '\t' || *c == ' ' || *c == '\n')
    {
      if (beginToken != NULL)
      {
        token = c2bf_create_token(beginToken, c, numLine);
        tokenList = list_append(tokenList, token);
        beginToken = NULL;
      }
      if (*c == '\n') numLine++;
      c++;
      continue;
    }
    else if ((knownToken = c2bf_isKnownToken(c)) != NULL)
    {
      if (beginToken != NULL)
      {
        token = c2bf_create_token(beginToken, c, numLine);
        tokenList = list_append(tokenList, token);
        beginToken = NULL;
      }
      token = c2bf_create_token(c, c+strlen(knownToken), numLine);
      tokenList = list_append(tokenList, token);
      c+=strlen(knownToken);
    }
    else
    {
      if (beginToken == NULL)
      {
        beginToken = c;
      }
      c++;
      continue;
    }
  }
  
  return tokenList;
}


static void c2bf_free_token_list(List* tokenList)
{
  List* cur = tokenList;
  while(cur)
  {
    List* next = cur->next;
    Token* token = (Token*)cur->data;
    free(token->token);
    free(token);
    free(cur);
    cur = next;
  }
}

typedef enum
{
  ASSIGNMENT,
  ADD_ASSIGNMENT,
  SUB_ASSIGNMENT,
  MUL_ASSIGNMENT,
  DIV_ASSIGNMENT,
  MOD_ASSIGNMENT,
  BAND_ASSIGNMENT,
  BXOR_ASSIGNMENT,
  BOR_ASSIGNMENT,
  SHL_ASSIGNMENT,
  SHR_ASSIGNMENT,
  
  TERNARY_COND,
  
  LOR,
  
  LAND,
  
  BOR,
  
  BXOR,
  
  BAND,
  
  EQUAL,
  NOT_EQUAL,
  
  GREATER_EQUAL,
  LESSER_EQUAL,
  GREATER,
  LESSER,
  
  SHL,
  SHR,
  
  ADD,
  SUB,
  
  MUL,
  DIV,
  MOD,
  
  UNARY_PLUS,
  UNARY_NEG,
  UNARY_LNOT,
  UNARY_BNOT,
  UNARY_PRE_INCREMENT,
  UNARY_PRE_DECREMENT,
  
  UNARY_POST_INCREMENT,
  UNARY_POST_DECREMENT,
  
  
  FUNCTION_CALL,
  
  VARIABLE,
  
  CHARACTER,
  
  CONSTANT,
} CExprType;


typedef struct
{
  char*  function_name;
  List*  argument_list; /* list of expr */
} CFunctionCall;

typedef struct _CExpr CExpr;

struct _CExpr
{
  CExprType        type;
  int              numLine;
  union
  {
    CExpr*         ce_expr;
    struct
    {
      CExpr*       ce_expr_left;
      CExpr*       ce_expr_right;
    } s;
    struct
    {
      CExpr*       ce_expr_cond;
      CExpr*       ce_expr_then;
      CExpr*       ce_expr_else;
    } s2;
    CExpr*         ce_expr_tab[3];
    CFunctionCall* ce_function_call;
    struct
    {
      char*          ce_var;
      int            ce_var_is_static;
    } s3;
    int            ce_constant;
  } u;
};

#define ce_expr          u.ce_expr
#define ce_expr_left     u.s.ce_expr_left
#define ce_expr_right    u.s.ce_expr_right
#define ce_expr_cond     u.s2.ce_expr_cond
#define ce_expr_then     u.s2.ce_expr_then
#define ce_expr_else     u.s2.ce_expr_else
#define ce_expr_tab      u.ce_expr_tab
#define ce_function_call u.ce_function_call
#define ce_var           u.s3.ce_var
#define ce_var_is_static u.s3.ce_var_is_static
#define ce_constant      u.ce_constant

typedef enum
{
  UNARY = 1,
  BINARY = 2,
  TERNARY = 3,
} NAry;

typedef enum
{
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
} Associativity;

typedef struct
{
  char*         name;
  CExprType     type;
  NAry          n_ary;
  Associativity associativity;
  int           priority;
} COperator;
  
static CONST COperator operators[] = {
  {"=", ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"+=", ADD_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"-=", SUB_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"*=", MUL_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"/=", DIV_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"%=", MOD_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"&=", BAND_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"^=", BXOR_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"|=", BOR_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {"<<=", SHL_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  {">>=", SHR_ASSIGNMENT, BINARY, RIGHT_TO_LEFT, 0},
  
  { "?", TERNARY_COND, TERNARY, RIGHT_TO_LEFT, 1},
  { ":", TERNARY_COND, TERNARY, RIGHT_TO_LEFT, 1},
  
  {"||", LOR, BINARY, LEFT_TO_RIGHT, 2},
  
  {"&&", LAND, BINARY, LEFT_TO_RIGHT, 3},
  
  {"|", BOR, BINARY, LEFT_TO_RIGHT, 4},
  
  {"^", BXOR, BINARY, LEFT_TO_RIGHT, 5},
  
  {"&", BAND, BINARY, LEFT_TO_RIGHT, 6},
  
  {"==", EQUAL, BINARY, LEFT_TO_RIGHT, 7},
  {"!=", NOT_EQUAL, BINARY, LEFT_TO_RIGHT, 7},
  
  {">=", GREATER_EQUAL, BINARY, LEFT_TO_RIGHT, 8},
  {"<=", LESSER_EQUAL, BINARY, LEFT_TO_RIGHT, 8},
  {">", GREATER, BINARY, LEFT_TO_RIGHT, 8},
  {"<", LESSER, BINARY, LEFT_TO_RIGHT, 8},
  
  {">>", SHR, BINARY, LEFT_TO_RIGHT, 9},
  {"<<", SHL, BINARY, LEFT_TO_RIGHT, 9},
  
  {"+", ADD, BINARY, LEFT_TO_RIGHT, 10},
  {"-", SUB, BINARY, LEFT_TO_RIGHT, 10},
  
  {"*", MUL, BINARY, LEFT_TO_RIGHT, 11},
  {"%", MOD, BINARY, LEFT_TO_RIGHT, 11},
  {"/", DIV, BINARY, LEFT_TO_RIGHT, 11},
  
  {"+", UNARY_PLUS, UNARY, RIGHT_TO_LEFT, 12},
  {"-", UNARY_NEG, UNARY, RIGHT_TO_LEFT, 12},
  {"!", UNARY_LNOT, UNARY, RIGHT_TO_LEFT, 12},
  {"~", UNARY_BNOT, UNARY, RIGHT_TO_LEFT, 12},
  {"++", UNARY_PRE_INCREMENT, UNARY, RIGHT_TO_LEFT, 12},
  {"--", UNARY_PRE_DECREMENT, UNARY, RIGHT_TO_LEFT, 12},
  
  {"++", UNARY_POST_INCREMENT, UNARY, LEFT_TO_RIGHT, 13},
  {"--", UNARY_POST_DECREMENT, UNARY, LEFT_TO_RIGHT, 13},

  {")", (CExprType)-1, (NAry)-1, (Associativity)-1, -1},
  {"(", (CExprType)-1, (NAry)-1, (Associativity)-1, -1},

};

static CONST COperator* c2bf_is_token_operator(CONST char* token, int lookUnary)
{
  unsigned int i;
  for(i=0;i<sizeof(operators)/sizeof(COperator);i++)
  {
    if (strcmp(operators[i].name, token) == 0 && (!lookUnary || operators[i].n_ary == UNARY))
      return &operators[i];
  }
  return NULL;
}

static int c2bf_is_operator(CExprType type)
{
  switch(type)
  {
    case VARIABLE:
    case CONSTANT:
    case CHARACTER:
    case FUNCTION_CALL:
      return 0;

    default:
      return 1;
  }
}

static CONST COperator* c2bf_find_operator_of_expr_type(CExprType type)
{
  unsigned int i;
  for(i=0;i<sizeof(operators)/sizeof(COperator);i++)
  {
    if (operators[i].type == type)
    {
      return &operators[i];
    }
  }
  SHOULDNT_HAPPEN();
  return NULL;
}


static CExpr* c2bf_new_expr_constant(int constant, int numLine)
{
  CExpr* expr = alloc0(CExpr, 1);
  expr->type = CONSTANT;
  expr->numLine = numLine;
  expr->ce_constant = constant;
  return expr;
}

static CExpr* c2bf_new_expr_var(CONST char* varName, int numLine)
{
  CExpr* expr = alloc0(CExpr, 1);
  expr->type = VARIABLE;
  expr->numLine = numLine;
  if (strncmp(varName, "__bf_tmp", 8) == 0)
  {
    expr->ce_var = (char*) varName;
    expr->ce_var_is_static = 1;
  }
  else
    expr->ce_var = strdup(varName);
  return expr;
}

static CExpr* c2bf_new_expr_assign(CExpr* left, CExpr* right)
{
  CExpr* expr = alloc0(CExpr, 1);
  expr->type = ASSIGNMENT;
  expr->numLine = left->numLine;
  expr->ce_expr_left = left;
  expr->ce_expr_right = right;
  return expr;
}

static CExpr* c2bf_new_expr_assign_var(CONST char* varName, CONST char* varNameRight, int numLine)
{
  return c2bf_new_expr_assign(c2bf_new_expr_var(varName, numLine), c2bf_new_expr_var(varNameRight, numLine));
}

static CExpr* c2bf_new_expr_assign_constant(CONST char* varName, int constant, int numLine)
{
  return c2bf_new_expr_assign(c2bf_new_expr_var(varName, numLine), c2bf_new_expr_constant(constant, numLine));
}

static CExpr* c2bf_new_expr_unary(CExprType type, CExpr* unary_expr)
{
  CExpr* expr = alloc0(CExpr, 1);
  expr->type = type;
  expr->numLine = unary_expr->numLine;
  expr->ce_expr = unary_expr;
  return expr;
}


static CExpr* c2bf_new_expr_decrement(CONST char* varName, int numLine)
{
  return c2bf_new_expr_unary(UNARY_POST_DECREMENT, c2bf_new_expr_var(varName, numLine));
}

static CExpr* c2bf_new_expr_increment(CONST char* varName, int numLine)
{
  return c2bf_new_expr_unary(UNARY_POST_INCREMENT, c2bf_new_expr_var(varName, numLine));
}


static CExpr* c2bf_new_expr_binary(CExprType type, CExpr* left, CExpr* right)
{
  CExpr* expr = alloc0(CExpr, 1);
  expr->type = type;
  expr->numLine = left->numLine;
  expr->ce_expr_left = left;
  expr->ce_expr_right = right;
  return expr;
}


static CExpr* c2bf_dup_c_expr(CONST CExpr* expr);
static void c2bf_dump_c_expr(FILE* f, CONST CExpr* expr);
static void c2bf_free_expr(CExpr* expr);

static CFunctionCall* c2bf_dup_function_call(CONST CFunctionCall* function_call)
{
  CFunctionCall* new_function_call = alloc0(CFunctionCall, 1);
  List* cur = function_call->argument_list;
  new_function_call->function_name = strdup(function_call->function_name);
  new_function_call->argument_list = NULL;
  while(cur)
  {
    new_function_call->argument_list = 
        list_append(new_function_call->argument_list, c2bf_dup_c_expr((CExpr*)cur->data));
    cur = cur->next;
  }
  return new_function_call;
}


static void c2bf_dump_function_call(FILE* f, CONST CFunctionCall* function_call)
{
  List* cur = function_call->argument_list;
  fprintf(f, "%s(", function_call->function_name);
  while(cur)
  {
    c2bf_dump_c_expr(f, (CExpr*)cur->data);
    cur = cur->next;
    if (cur)
      fprintf(f, ", ");
  }
  fprintf(f, ")");
}

static void c2bf_free_function_call(CFunctionCall* function_call)
{
  List* cur = function_call->argument_list;
  free(function_call->function_name);
  while(cur)
  {
    List* next = cur->next;
    c2bf_free_expr((CExpr*)cur->data);
    free(cur);
    cur = next;
  }
  free(function_call);
}

static CExpr* c2bf_dup_c_expr(CONST CExpr* expr)
{
  CExpr* newExpr = alloc0(CExpr, 1);
  newExpr->type = expr->type;
  newExpr->numLine = expr->numLine;
  switch(expr->type)
  {
    case FUNCTION_CALL:
      newExpr->ce_function_call = c2bf_dup_function_call(expr->ce_function_call);
      break;
      
    case VARIABLE:
    {
      if (expr->ce_var_is_static)
      {
        newExpr->ce_var_is_static = 1;
        newExpr->ce_var = expr->ce_var;
      }
      else
        newExpr->ce_var = strdup(expr->ce_var);
      break;
    }
    
    case CHARACTER:
    case CONSTANT:
      newExpr->ce_constant = expr->ce_constant;
      break;
    
    default:
    {
      CONST COperator* coperator = c2bf_find_operator_of_expr_type(expr->type);
      int i;
      for(i=0;i<coperator->n_ary;i++)
        newExpr->ce_expr_tab[i] = c2bf_dup_c_expr(expr->ce_expr_tab[i]);
    }
  }
  return newExpr;
}

static void c2bf_dump_c_expr(FILE* f, CONST CExpr* expr)
{
  switch(expr->type)
  {
    case FUNCTION_CALL:
      c2bf_dump_function_call(f, expr->ce_function_call);
      break;
      
    case VARIABLE:
      fputs(expr->ce_var, f);
      break;
    
    case CONSTANT:
      fprintf(f, "%d", expr->ce_constant);
      break;
      
    case CHARACTER:
      if (expr->ce_constant == '\n')
        fputs("'\\n'", f);
      else if (expr->ce_constant == '\t')
        fputs("'\\t'", f);
      else
        fprintf(f, "'%c'", expr->ce_constant);
      break;
    
    default:
    {
      CONST COperator* coperator = c2bf_find_operator_of_expr_type(expr->type);
      switch (coperator->n_ary)
      {
        case UNARY:
        {
          if (expr->type == UNARY_POST_INCREMENT ||
              expr->type == UNARY_POST_DECREMENT)
          {
            fprintf(f, "(");
            c2bf_dump_c_expr(f, expr->ce_expr);
            fprintf(f, "%s)", coperator->name);
          }
          else
          {
            fprintf(f, "(");
            fprintf(f, "%s", coperator->name);
            c2bf_dump_c_expr(f, expr->ce_expr);
            fprintf(f, ")");
          }
          break;
        }

        case BINARY:
        {
          fprintf(f, "(");
          c2bf_dump_c_expr(f, expr->ce_expr_left);
          fprintf(f, "%s", coperator->name);
          c2bf_dump_c_expr(f, expr->ce_expr_right);
          fprintf(f, ")");
          break;
        }
        
        case TERNARY:
        {
          fprintf(f, "(");
          c2bf_dump_c_expr(f, expr->ce_expr_cond);
          fprintf(f, "?");
          c2bf_dump_c_expr(f, expr->ce_expr_then);
          fprintf(f, ":");
          c2bf_dump_c_expr(f, expr->ce_expr_else);
          fprintf(f, ")");
          break;
        }
        
        default: SHOULDNT_HAPPEN();
      }
    }
  }
}


static void c2bf_free_expr(CExpr* expr)
{
  switch(expr->type)
  {
    case FUNCTION_CALL:
      c2bf_free_function_call(expr->ce_function_call);
      break;
      
    case VARIABLE:
      if (!expr->ce_var_is_static) free(expr->ce_var);
      break;
      
    case CHARACTER:
    case CONSTANT:
      break;
      
    default:
    {
      CONST COperator* coperator = c2bf_find_operator_of_expr_type(expr->type);
      int i;
      for(i=0;i<coperator->n_ary;i++)
        c2bf_free_expr(expr->ce_expr_tab[i]);
    }
  }
  free(expr);
}

typedef enum
{
  NOTHING_STATEMENT,
  LIST,
  DECLARATION,
  AFFECT,
  EXPRESSION,
  WHILE,
  DO_WHILE,
  IF,
  FOR,
  BREAK,
  CONTINUE
} CCodeType;

typedef struct _CCode CCode;

typedef struct
{
  char*  var;
} CDeclaration;

typedef struct
{
  char*  var;
  CExpr* expr;
} CAffect;

typedef struct
{
  CExpr* cond;
  CCode* block;
} CWhile;

typedef struct
{
  CExpr* cond;
  CCode* block;
} CDoWhile;

typedef struct
{
  CExpr* cond;
  CCode* block;
  CCode* else_block;
} CIf;

typedef struct
{
  CExpr* cond;
  CCode* init_block;
  CCode* update_block;
  CCode* block;
} CFor;

struct _CCode
{
  CCodeType type;
  int       numLine;
  union
  {
    List*          c_list;
    CDeclaration*  c_decl;
    CExpr*         c_expr;
    CWhile*        c_while;
    CDoWhile*      c_do_while;
    CIf*           c_if;
    CFor*          c_for;
  } u;
};

#define c_list             u.c_list
#define c_decl             u.c_decl
#define c_expr             u.c_expr
#define c_while            u.c_while
#define c_do_while         u.c_do_while
#define c_if               u.c_if
#define c_for              u.c_for

static CCode* c2bf_dup_c_code(CONST CCode* ccode);
static void c2bf_free_c_code(CCode* ccode);

static List* c2bf_dup_c_code_list(CONST List* list)
{
  CONST List* cur = list;
  List* new_list = NULL;
  while(cur)
  {
    new_list = list_append(new_list, c2bf_dup_c_code((CCode*)cur->data));
    cur = cur->next;
  }
  return new_list;
}

static void c2bf_free_c_code_list(List* list)
{
  List* cur = list;
  while(cur)
  {
    List* next = cur->next;
    c2bf_free_c_code((CCode*)cur->data);
    free(cur);
    cur = next;
  }
}

static CDeclaration* c2bf_dup_c_declaration(CONST CDeclaration* ccode_decl)
{
  CDeclaration* new_decl = alloc0(CDeclaration, 1);
  new_decl->var = strdup(ccode_decl->var);
  return new_decl;
}

static void c2bf_free_c_declaration(CDeclaration* ccode_decl)
{
  free(ccode_decl->var);
  free(ccode_decl);
}

static CWhile* c2bf_dup_while(CONST CWhile* ccode_while)
{
  CWhile* new_while = alloc0(CWhile, 1);
  new_while->cond = c2bf_dup_c_expr(ccode_while->cond);
  new_while->block = c2bf_dup_c_code(ccode_while->block);
  return new_while;
}

static void c2bf_free_while(CWhile* ccode_while)
{
  c2bf_free_expr(ccode_while->cond);
  c2bf_free_c_code(ccode_while->block);
  free(ccode_while);
}

static CDoWhile* c2bf_dup_do_while(CONST CDoWhile* ccode_do_while)
{
  CDoWhile* new_do_while = alloc0(CDoWhile, 1);
  new_do_while->cond = c2bf_dup_c_expr(ccode_do_while->cond);
  new_do_while->block = c2bf_dup_c_code(ccode_do_while->block);
  return new_do_while;
}

static void c2bf_free_do_while(CDoWhile* ccode_do_while)
{
  c2bf_free_expr(ccode_do_while->cond);
  c2bf_free_c_code(ccode_do_while->block);
  free(ccode_do_while);
}

static CIf* c2bf_dup_if(CONST CIf* ccode_if)
{
  CIf* new_if = alloc0(CIf, 1);
  new_if->cond = c2bf_dup_c_expr(ccode_if->cond);
  new_if->block = c2bf_dup_c_code(ccode_if->block);
  if(ccode_if->else_block)
    new_if->else_block = c2bf_dup_c_code(ccode_if->else_block);
  return new_if;
}

static void c2bf_free_if(CIf* ccode_if)
{
  c2bf_free_expr(ccode_if->cond);
  c2bf_free_c_code(ccode_if->block);
  if(ccode_if->else_block)
    c2bf_free_c_code(ccode_if->else_block);
  free(ccode_if);
}

static CFor* c2bf_dup_for(CONST CFor* ccode_for)
{
  CFor* new_for = alloc0(CFor, 1);
  new_for->cond = c2bf_dup_c_expr(ccode_for->cond);
  new_for->block = c2bf_dup_c_code(ccode_for->block);
  new_for->init_block = c2bf_dup_c_code(ccode_for->init_block);
  new_for->update_block = c2bf_dup_c_code(ccode_for->update_block);
  return new_for;
}

static void c2bf_free_for(CFor* ccode_for)
{
  c2bf_free_expr(ccode_for->cond);
  c2bf_free_c_code(ccode_for->init_block);
  c2bf_free_c_code(ccode_for->update_block);
  c2bf_free_c_code(ccode_for->block);
  free(ccode_for);
}

static CCode* c2bf_dup_c_code(CONST CCode* ccode)
{
  CCode* new_code = alloc0(CCode, 1);
  new_code->numLine = ccode->numLine;
  new_code->type = ccode->type;
  switch(ccode->type)
  {
    case LIST:
      new_code->c_list = c2bf_dup_c_code_list(ccode->c_list);
      break;
      
    case DECLARATION:
      new_code->c_decl = c2bf_dup_c_declaration(ccode->c_decl);
      break;
      
    case EXPRESSION:
      new_code->c_expr = c2bf_dup_c_expr(ccode->c_expr);
      break;
      
    case WHILE:
      new_code->c_while = c2bf_dup_while(ccode->c_while);
      break;
      
    case DO_WHILE:
      new_code->c_do_while = c2bf_dup_do_while(ccode->c_do_while);
      break;
      
    case IF:
      new_code->c_if = c2bf_dup_if(ccode->c_if);
      break;
      
    case FOR:
      new_code->c_for = c2bf_dup_for(ccode->c_for);
      break;
      
    case BREAK:
    case CONTINUE:
    case NOTHING_STATEMENT:
      break;
      
    default:
      SHOULDNT_HAPPEN();
  }
  return new_code;
}

static void c2bf_free_c_code(CCode* ccode)
{
  switch(ccode->type)
  {
    case LIST:
      c2bf_free_c_code_list(ccode->c_list);
      break;
      
    case DECLARATION:
      c2bf_free_c_declaration(ccode->c_decl);
      break;
      
    case EXPRESSION:
      c2bf_free_expr(ccode->c_expr);
      break;
      
    case WHILE:
      c2bf_free_while(ccode->c_while);
      break;
      
    case DO_WHILE:
      c2bf_free_do_while(ccode->c_do_while);
      break;
      
    case IF:
      c2bf_free_if(ccode->c_if);
      break;
      
    case FOR:
      c2bf_free_for(ccode->c_for);
      break;
      
    case BREAK:
    case CONTINUE:
    case NOTHING_STATEMENT:
      break;
      
    default:
      SHOULDNT_HAPPEN();
  }
  
  free(ccode);
}

#define TOKEN(cur)           (((cur) != NULL) ? ((Token*)(cur)->data)->token : "")
#define TOKEN_LINE(cur)      (((cur) != NULL) ? ((Token*)(cur)->data)->numLine : 0)
#define NEXT_TOKEN(cur)      (((cur) != NULL) ? TOKEN((cur)->next) : "")
#define NEXT_TOKEN_LINE(cur) (((cur) != NULL) ? TOKEN_LINE((cur)->next) : 0)
#define ASSERT_TOKEN(list, str) do { if(strcmp(TOKEN(list), str) != 0) fatal("Expected " str " at line %d of source file\n", TOKEN_LINE(list)); } while(0)
#define ASSERT_NEXT_TOKEN(list, str) do { if(strcmp(NEXT_TOKEN(list), str) != 0) fatal("Expected " str " at line %d", NEXT_TOKEN_LINE(list)); } while(0)

#define ASSERT(cond) do { if (!(cond)) fatal("cond \"" #cond "\" failed\n"); } while(0)

#define ASSERT_WITH_LINE(line, cond) do { if (!(cond)) fatal("cond \"" #cond "\" failed at line %d of source file\n", line); } while(0)

/*   1 * 3 + 2 * 4   */ 
/*   1 + 3 * 2 + 4   */

typedef struct
{
  List* operandList;
  List* operatorList;
  int   numLine;
} ExprBuilding;

static CExpr* c2bf_unstack_last_operand(ExprBuilding* builder)
{
  CExpr* expr;
  ASSERT_WITH_LINE(builder->numLine, builder->operandList != NULL);
  expr = (CExpr*)builder->operandList->data;
  builder->operandList = list_remove_head_link(builder->operandList);
  return expr;
}

static void c2bf_unstack_last_operator(ExprBuilding* builder)
{
  COperator* lastOperator;
  int i;
  CExpr* expr;
  ASSERT_WITH_LINE(builder->numLine, builder->operatorList != NULL);
  lastOperator = (COperator*)builder->operatorList->data;
  
  expr = alloc0(CExpr, 1);
  expr->type = lastOperator->type;
  for(i=lastOperator->n_ary-1;i>=0;i--)
  {
    expr->ce_expr_tab[i] = c2bf_unstack_last_operand(builder);
    expr->numLine = expr->ce_expr_tab[i]->numLine;
  }
  builder->operandList = list_prepend(builder->operandList, expr);
  
  builder->operatorList = list_remove_head_link(builder->operatorList);
  if (lastOperator->n_ary == TERNARY)
    builder->operatorList = list_remove_head_link(builder->operatorList);
}

static CExpr* c2bf_build_c_expr(CONST List* tokenList, CONST List** p_endTokenList)
{
  ExprBuilding builder;
  CONST List* cur = tokenList;
  int balanceParenthesis = 0;
  int endNow = FALSE;
  int lastTokenWasOperator = TRUE;
  builder.operandList = NULL;
  builder.operatorList = NULL;
  while(cur)
  {
    CONST char* token = TOKEN(cur);
    
    CONST COperator* coperator = c2bf_is_token_operator(token, FALSE);
    
    builder.numLine = TOKEN_LINE(cur);
    
    if (strcmp(token, "(") == 0)
    {
      balanceParenthesis++;
      lastTokenWasOperator = TRUE;
    }
    else if (strcmp(token, ")") == 0)
    {
      balanceParenthesis--;
      if (balanceParenthesis < 0)
      {
        endNow = TRUE;
      }
      lastTokenWasOperator = FALSE;
    }
    else if (strcmp(token, "++") == 0 || (strcmp(token, "--") == 0))
    {
      if (lastTokenWasOperator)
      {
        CExpr* expr = alloc0(CExpr, 1);
        int wrapVar = FALSE;
        expr->type = (strcmp(token, "++") == 0) ? UNARY_PRE_INCREMENT : UNARY_PRE_DECREMENT;
        expr->numLine = TOKEN_LINE(cur);
        cur = cur->next;
        if (strcmp(TOKEN(cur), "(") == 0) { wrapVar= TRUE; cur = cur->next; }
        ASSERT_WITH_LINE(TOKEN_LINE(cur), !c2bf_is_keyword(TOKEN(cur)));
        expr->ce_expr = c2bf_new_expr_var(TOKEN(cur), expr->numLine);
        cur = cur->next;
        if (strcmp(TOKEN(cur), ")") == 0 && wrapVar) cur = cur->next;
        builder.operandList = list_prepend(builder.operandList, expr);
        lastTokenWasOperator = FALSE;
        continue;
      }
      else
      {
        CExpr* expr = alloc0(CExpr, 1);
        CExpr* lastOperandExpr;
        expr->numLine = TOKEN_LINE(cur);
        lastOperandExpr = (CExpr*)builder.operandList->data;
        builder.operandList = list_remove_head_link(builder.operandList);
        ASSERT_WITH_LINE(TOKEN_LINE(cur), lastOperandExpr->type == VARIABLE);
        expr->type = (strcmp(token, "++") == 0) ? UNARY_POST_INCREMENT : UNARY_POST_DECREMENT;
        expr->ce_expr = lastOperandExpr;
        cur = cur->next;
        builder.operandList = list_prepend(builder.operandList, expr);
        lastTokenWasOperator = FALSE;
        continue;
      }
    }
    else
    {
      if (coperator && lastTokenWasOperator)
      {
        coperator = c2bf_is_token_operator(token, TRUE);
        ASSERT_WITH_LINE(TOKEN_LINE(cur), coperator != NULL);
      }
      lastTokenWasOperator = coperator != NULL;
    }
    if (endNow || strcmp(token, ";") == 0 || strcmp(token, ",") == 0)
    {
      CExpr* expr;
      if (p_endTokenList) *p_endTokenList = cur;
      
      while(builder.operatorList)
      {
        c2bf_unstack_last_operator(&builder);
      }
      ASSERT_WITH_LINE(builder.numLine, builder.operandList != NULL && builder.operandList->next == NULL);
      expr = (CExpr*)builder.operandList->data;
      free(builder.operandList);
      return expr;
    }
    else if (coperator)
    {
      if (strcmp(token, "(") != 0)
      {
        while (builder.operatorList)
        {
          COperator* lastOperator = (COperator*)builder.operatorList->data;
          if (strcmp(lastOperator->name, "(") == 0)
            break;
          if (coperator->priority < lastOperator->priority)
          {
            c2bf_unstack_last_operator(&builder);
          }
          else if (coperator->n_ary != TERNARY &&
                   coperator->priority == lastOperator->priority &&
                   coperator->associativity == LEFT_TO_RIGHT )
          {
            c2bf_unstack_last_operator(&builder);
          }
          else
          {
            break;
          }
        }
      }
      if (strcmp(token, ")") == 0)
      {
        while(1)
        {
          COperator* lastOperator = (COperator*)builder.operatorList->data;
          if (strcmp(lastOperator->name, "(") == 0)
          {
            builder.operatorList = list_remove_head_link(builder.operatorList);
            break;
          }
          else
          {
            c2bf_unstack_last_operator(&builder);
          }
        }
      }
      else
      {
        builder.operatorList = list_prepend(builder.operatorList, (COperator*)coperator);
      }
    }
    else
    {
      CExpr* operandExpr = alloc0(CExpr, 1);
      operandExpr->numLine = TOKEN_LINE(cur);
      builder.operandList = list_prepend(builder.operandList, operandExpr);
      if (token[0] >= '0' && token[0] <= '9')
      {
        operandExpr->type = CONSTANT;
        operandExpr->ce_constant = atoi(token);
      }
      else if (token[0] == '\'')
      {
        ASSERT_WITH_LINE(builder.numLine, token[strlen(token)-1] == '\'');
        operandExpr->type = CHARACTER;
        if (token[1] != '\\')
          operandExpr->ce_constant = token[1];
        else if (token[2] == 'n')
          operandExpr->ce_constant = '\n';
        else if (token[2] == 't')
          operandExpr->ce_constant = '\t';
        else if (token[2] == '\\')
          operandExpr->ce_constant = '\\';
        else
          NOT_IMPLEMENTED();
      }
      else if (strcmp(NEXT_TOKEN(cur), "(") == 0)
      {
        ASSERT_WITH_LINE(TOKEN_LINE(cur), !c2bf_is_keyword(token));
        operandExpr->type = FUNCTION_CALL;
        operandExpr->ce_function_call = alloc0(CFunctionCall, 1);
        operandExpr->ce_function_call->function_name = strdup(token);
        cur = cur->next;
        if (strcmp(NEXT_TOKEN(cur), ")") != 0)
        {
          do
          {
            cur = cur->next;
            operandExpr->ce_function_call->argument_list =
                list_append(operandExpr->ce_function_call->argument_list,
                            c2bf_build_c_expr(cur, &cur));
          } while (strcmp(TOKEN(cur), ",") == 0);
        }
        else
        {
          cur = cur->next;
        }
        ASSERT_TOKEN(cur, ")");
      }
      else
      {
        ASSERT_WITH_LINE(TOKEN_LINE(cur), !c2bf_is_keyword(token));
        operandExpr->type = VARIABLE;
        operandExpr->ce_var = strdup(token);
      }
    }
    cur = cur->next;
  }
  SHOULDNT_HAPPEN();
  return NULL;
}

static List* c2bf_build_c_decl(List* ccodeList, CONST List* tokenList, CONST List** p_endTokenList)
{
  CONST List* cur = tokenList;
  const char* varName;
  CCode* ccode;
  ASSERT_WITH_LINE(TOKEN_LINE(cur), strcmp(TOKEN(cur), "int") == 0 || strcmp(TOKEN(cur), ",") == 0);
  cur = cur->next;
      
  varName = TOKEN(cur);
      
  ccode = alloc0(CCode, 1);
  ccode->numLine = TOKEN_LINE(cur);
  ccode->type = DECLARATION;
  ccode->c_decl = alloc0(CDeclaration, 1);
  ccode->c_decl->var = strdup(varName);
  ccodeList = list_append(ccodeList, ccode);
      
  cur = cur->next;
  if (strcmp(TOKEN(cur), ";") == 0)
  {
    if (p_endTokenList) *p_endTokenList = cur;
    return ccodeList;
  }
  else if (strcmp(TOKEN(cur), "=") == 0)
  {
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = EXPRESSION;
    ccode->c_expr = alloc0(CExpr, 1);
    ccode->c_expr->numLine = TOKEN_LINE(cur);
    ccode->c_expr->type = ASSIGNMENT;
    ccode->c_expr->ce_expr_left = c2bf_new_expr_var(varName, ccode->c_expr->numLine);
    ccode->c_expr->ce_expr_right = c2bf_build_c_expr(cur->next, &cur);
    ccodeList = list_append(ccodeList, ccode);
    
    if (strcmp(TOKEN(cur), ",") == 0)
    {
      return c2bf_build_c_decl(ccodeList, cur, p_endTokenList);
    }
    else if (strcmp(TOKEN(cur), ";") == 0)
    {
      if (p_endTokenList) *p_endTokenList = cur;
      return ccodeList;
    }
    else SHOULDNT_HAPPEN();
  }
  else if (strcmp(TOKEN(cur), ",") == 0)
  {
    return c2bf_build_c_decl(ccodeList, cur, p_endTokenList);
  }
  
  fatal("Invalid declaration at line %d of source file\n", TOKEN_LINE(cur));
  return NULL;
}

static CCode* c2bf_build_c_statement(CONST List* tokenList, CONST List** p_endTokenList);

static CCode* c2bf_build_c_statement_list(CONST List* tokenList, CONST List** p_endTokenList)
{
  CONST List* cur = tokenList;
  CCode* ccode;
  ccode = alloc0(CCode, 1);
  ccode->numLine = TOKEN_LINE(cur);
  ccode->type = LIST;
  ccode->c_list = NULL;
  ASSERT_TOKEN(cur, "{");
  
  cur = cur->next;
  while(cur)
  {
    if (strcmp(TOKEN(cur), "int") == 0)
    {
      ccode->c_list = c2bf_build_c_decl(ccode->c_list, cur, &cur);
      ASSERT_TOKEN(cur, ";");
      cur = cur->next;
      continue;
    }
    else if (strcmp(TOKEN(cur), "}") == 0)
    {
      if (p_endTokenList) *p_endTokenList = cur;
      return ccode;
    }
    else
    {
      ccode->c_list = list_append(ccode->c_list, c2bf_build_c_statement(cur, &cur));
      ASSERT_WITH_LINE(TOKEN_LINE(cur), strcmp(TOKEN(cur), ";") == 0 || strcmp(TOKEN(cur), "}") == 0);
      cur = cur->next;
      continue;
    }
  }
  
  fatal("Missing } in source file\n");
  return NULL;
}

static CCode* c2bf_build_c_statement(CONST List* tokenList, CONST List** p_endTokenList)
{
  CONST List* cur = tokenList;
  if (strcmp(TOKEN(cur), "{") == 0)
  {
    return c2bf_build_c_statement_list(cur, p_endTokenList);
  }
  else if (strcmp(TOKEN(cur), ";") == 0)
  {
    CCode* ccode;
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = NOTHING_STATEMENT;
    if (p_endTokenList) *p_endTokenList = cur;
    return ccode;
  }
  else if (strcmp(TOKEN(cur), "break") == 0)
  {
    CCode* ccode;
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = BREAK;
    
    cur = cur->next;
    ASSERT_TOKEN(cur, ";");
    
    if (p_endTokenList) *p_endTokenList = cur;
    return ccode;
  }
  else if (strcmp(TOKEN(cur), "continue") == 0)
  {
    CCode* ccode;
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = CONTINUE;
    
    cur = cur->next;
    ASSERT_TOKEN(cur, ";");
    
    if (p_endTokenList) *p_endTokenList = cur;
    return ccode;
  }
  else if (strcmp(TOKEN(cur), "while") == 0)
  {
    CCode* ccode;
    
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = WHILE;
    ccode->c_while = alloc0(CWhile, 1);
    
    cur = cur->next;
    ASSERT_TOKEN(cur, "(");
    ccode->c_while->cond = c2bf_build_c_expr(cur->next, &cur);
    
    ASSERT_TOKEN(cur, ")");
    cur = cur->next;
    ccode->c_while->block = c2bf_build_c_statement(cur, p_endTokenList);
    
    return ccode;
  }
  else if (strcmp(TOKEN(cur), "do") == 0)
  {
    CCode* ccode;
    
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = DO_WHILE;
    ccode->c_do_while = alloc0(CDoWhile, 1);
    
    cur = cur->next;
    ccode->c_do_while->block = c2bf_build_c_statement(cur, &cur);
    ASSERT_WITH_LINE(TOKEN_LINE(cur), strcmp(TOKEN(cur), ";") == 0 || strcmp(TOKEN(cur), "}") == 0);
    
    cur = cur->next;
    ASSERT_TOKEN(cur, "while");
    
    cur = cur->next;
    ASSERT_TOKEN(cur, "(");
    
    ccode->c_while->cond = c2bf_build_c_expr(cur->next, &cur);
    
    ASSERT_TOKEN(cur, ")");
    cur = cur->next;
    
    ASSERT_TOKEN(cur, ";");
    if (p_endTokenList) *p_endTokenList = cur;
    
    return ccode;
  }
  else if (strcmp(TOKEN(cur), "if") == 0)
  {
    CCode* ccode;
    
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = IF;
    ccode->c_if = alloc0(CIf, 1);
    
    cur = cur->next;
    ASSERT_TOKEN(cur, "(");
    ccode->c_if->cond = c2bf_build_c_expr(cur->next, &cur);
    
    ASSERT_TOKEN(cur, ")");
    cur = cur->next;
    ccode->c_if->block = c2bf_build_c_statement(cur, &cur);
    
    ASSERT_WITH_LINE(TOKEN_LINE(cur), strcmp(TOKEN(cur), ";") == 0 || strcmp(TOKEN(cur), "}") == 0);
    
    if (strcmp(NEXT_TOKEN(cur), "else") == 0)
    {
      cur = cur->next->next;
      ccode->c_if->else_block = c2bf_build_c_statement(cur, p_endTokenList);
    }
    else
    {
      if (p_endTokenList) *p_endTokenList = cur;
    }
    
    return ccode;
  } 
  else if (strcmp(TOKEN(cur), "for") == 0)
  {
    CCode* ccode;
    
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = FOR;
    ccode->c_for = alloc0(CFor, 1);
    
    ccode->c_for->init_block = alloc0(CCode, 1);
    ccode->c_for->init_block->numLine = TOKEN_LINE(cur);
    ccode->c_for->init_block->type = LIST;
    ccode->c_for->init_block->c_list = NULL;
    
    cur = cur->next;
    ASSERT_TOKEN(cur, "(");
    
    if (strcmp(TOKEN(cur->next), ";") != 0)
    {
      do
      {
        CCode* codeExpr = alloc0(CCode, 1);
        codeExpr->numLine = TOKEN_LINE(cur);
        codeExpr->type = EXPRESSION;
        cur = cur->next;
        codeExpr->c_expr = c2bf_build_c_expr(cur, &cur);
        ccode->c_for->init_block->c_list = list_append(ccode->c_for->init_block->c_list, codeExpr);
      } while(strcmp(TOKEN(cur), ";") != 0);
    }
    else
    {
      cur = cur->next;
    }
    cur = cur->next;
    
    if (strcmp(TOKEN(cur), ";") != 0)
    {
      ccode->c_for->cond = c2bf_build_c_expr(cur, &cur);
    }
    else
    {
      ccode->c_for->cond = c2bf_new_expr_constant(1, TOKEN_LINE(cur));
    }
    ASSERT_TOKEN(cur, ";");
    
    ccode->c_for->update_block = alloc0(CCode, 1);
    ccode->c_for->update_block->numLine = TOKEN_LINE(cur);
    ccode->c_for->update_block->type = LIST;
    ccode->c_for->update_block->c_list = NULL;
    
    if (strcmp(TOKEN(cur->next), ")") != 0)
    {
      do
      {
        CCode* codeExpr = alloc0(CCode, 1);
        codeExpr->numLine = TOKEN_LINE(cur);
        codeExpr->type = EXPRESSION;
        cur = cur->next;
        codeExpr->c_expr = c2bf_build_c_expr(cur, &cur);
        ccode->c_for->update_block->c_list = list_append(ccode->c_for->update_block->c_list, codeExpr);
      } while(strcmp(TOKEN(cur), ")") != 0);
    }
    else
    {
      cur = cur->next;
    }
    
    cur = cur->next;
    
    ccode->c_for->block = c2bf_build_c_statement(cur, p_endTokenList);
    
    return ccode;
  }
  else if (strcmp(TOKEN(cur), "int") == 0)
  {
    fatal("Invalid type '%s' at line %d of source file\n", TOKEN(cur), TOKEN_LINE(cur));
    SHOULDNT_HAPPEN();
  }
  else
  {
    CCode* ccode;
    ASSERT_WITH_LINE(TOKEN_LINE(cur), !c2bf_is_keyword(TOKEN(cur)));
    ccode = alloc0(CCode, 1);
    ccode->numLine = TOKEN_LINE(cur);
    ccode->type = EXPRESSION;
    ccode->c_expr = c2bf_build_c_expr(cur, &cur);
    ASSERT_TOKEN(cur, ";");
    if (p_endTokenList) *p_endTokenList = cur;
    return ccode;
  }
}

static CONST List* c2bf_ignore_main_params(CONST List* cur)
{
  ASSERT_TOKEN(cur, "(");
  cur = cur->next;
  while(cur)
  {
    if (strcmp(TOKEN(cur), ")") == 0)
    {
      cur = cur->next;
      ASSERT_TOKEN(cur, "{");
      return cur;
    }
    cur = cur->next;
  }
  return NULL;
}

static CONST List* c2bf_look_for_main(CONST List* cur)
{
  while(cur)
  {
    if (strcmp(TOKEN(cur), "int") == 0 &&
        strcmp(NEXT_TOKEN(cur), "main") == 0)
    {
      return c2bf_ignore_main_params(cur->next->next);
    }
    else if (strcmp(TOKEN(cur), "main") == 0)
    {
      return c2bf_ignore_main_params(cur->next);
    }
    cur = cur->next;
  }
  return NULL;
}

static CCode* c2bf_build_c_code_main(CONST List* tokenList)
{
  CONST List* beginMain = c2bf_look_for_main(tokenList);
  
  return c2bf_build_c_statement_list(beginMain, NULL);
}


static void c2bf_dump_indent(FILE* f, int indent)
{
  int i;
  for(i=0;i<indent;i++) fprintf(f, "  ");
}

static void c2bf_dump_c_code(FILE* f, CONST CCode* ccode, int indent);

static void c2bf_dump_c_list(FILE* f, CONST List* list, int indent)
{
  while(list)
  {
    c2bf_dump_c_code(f, (const CCode*)list->data, indent);
    list = list->next;
  }
}

static int c2bf_add_optional_left_bracket(FILE* f, CONST CCode* ccode, int indent)
{
  if (ccode->type == IF ||
      ccode->type == WHILE ||
      ccode->type == DO_WHILE ||
      ccode->type == FOR)
  {
    c2bf_dump_indent(f, indent);
    fprintf(f, "{\n");
  }
  if (ccode->type != LIST)
    indent ++;
  return indent;
}

static int c2bf_add_optional_right_bracket(FILE* f, CONST CCode* ccode, int indent)
{
  if (ccode->type != LIST)
    indent --;
  if (ccode->type == IF ||
      ccode->type == WHILE ||
      ccode->type == DO_WHILE ||
      ccode->type == FOR)
  {
    c2bf_dump_indent(f, indent);
    fprintf(f, "}\n");
  }
  return indent;
}

static void c2bf_dump_c_code(FILE* f, CONST CCode* ccode, int indent)
{
  if (indent == 0)
  {
    fprintf(f, "int main()\n");
  }
  switch(ccode->type)
  {
    case LIST:
      c2bf_dump_indent(f, indent);
      fprintf(f, "{\n");
      indent++;
      c2bf_dump_c_list(f, ccode->c_list, indent);
      indent--;
      c2bf_dump_indent(f, indent);
      fprintf(f, "}\n");
      break;
      
    case DECLARATION:
      c2bf_dump_indent(f, indent);
      fprintf(f, "int %s = 0;\n", ccode->c_decl->var);
      break;
      
    case EXPRESSION:
      c2bf_dump_indent(f, indent);
      c2bf_dump_c_expr(f, ccode->c_expr);
      fprintf(f, ";\n");
      break;
      
    case WHILE:
      c2bf_dump_indent(f, indent);
      fprintf(f, "while(");
      c2bf_dump_c_expr(f, ccode->c_while->cond);
      fprintf(f, ")\n");
      indent = c2bf_add_optional_left_bracket(f, ccode->c_while->block, indent);
      c2bf_dump_c_code(f, ccode->c_while->block, indent);
      indent = c2bf_add_optional_right_bracket(f, ccode->c_while->block, indent);
      break;
      
     
    case DO_WHILE:
      c2bf_dump_indent(f, indent);
      fprintf(f, "do\n");
      indent = c2bf_add_optional_left_bracket(f, ccode->c_do_while->block, indent);
      c2bf_dump_c_code(f, ccode->c_do_while->block, indent);
      indent = c2bf_add_optional_right_bracket(f, ccode->c_do_while->block, indent);
      c2bf_dump_indent(f, indent);
      fprintf(f, "while(");
      c2bf_dump_c_expr(f, ccode->c_do_while->cond);
      fprintf(f, ");\n");
      break;
      
    case IF:
    {
      int cont;
      c2bf_dump_indent(f, indent);
      fprintf(f, "if(");
      c2bf_dump_c_expr(f, ccode->c_if->cond);
      fprintf(f, ")\n");
      do
      {
        cont = FALSE;
        indent = c2bf_add_optional_left_bracket(f, ccode->c_if->block, indent);
        c2bf_dump_c_code(f, ccode->c_if->block, indent);
        indent = c2bf_add_optional_right_bracket(f, ccode->c_if->block, indent);
        if (ccode->c_if->else_block)
        {
          if (ccode->c_if->else_block->type == IF)
          {
            c2bf_dump_indent(f, indent);
            fprintf(f, "else if(");
            c2bf_dump_c_expr(f, ccode->c_if->else_block->c_if->cond);
            fprintf(f, ")\n");
            ccode = ccode->c_if->else_block;
            cont = TRUE;
          }
          else
          {
            c2bf_dump_indent(f, indent);
            fprintf(f, "else\n");
            indent = c2bf_add_optional_left_bracket(f, ccode->c_if->else_block, indent);
            c2bf_dump_c_code(f, ccode->c_if->else_block, indent);
            indent = c2bf_add_optional_right_bracket(f, ccode->c_if->else_block, indent);
          }
        }
      } while(cont);
      break;
    }
    
    case FOR:
    {
      c2bf_dump_c_list(f, ccode->c_for->init_block->c_list, indent);
      c2bf_dump_indent(f, indent);
      fprintf(f, "for(;");
      c2bf_dump_c_expr(f, ccode->c_for->cond);
      fprintf(f, ";)\n");
      c2bf_dump_indent(f, indent);
      fprintf(f, "{\n");
      indent++;
      c2bf_dump_c_code(f, ccode->c_for->block, indent);
      c2bf_dump_c_list(f, ccode->c_for->update_block->c_list, indent);
      indent--;
      c2bf_dump_indent(f, indent);
      fprintf(f, "}\n");
      break;
    }
    
    case BREAK:
      c2bf_dump_indent(f, indent);
      fprintf(f, "break;\n");
      break;
      
    case CONTINUE:
      c2bf_dump_indent(f, indent);
      fprintf(f, "continue;\n");
      break;
    
    case NOTHING_STATEMENT:
      c2bf_dump_indent(f, indent);
      fprintf(f, ";\n");
      break;
      
    default:
      SHOULDNT_HAPPEN();
  }
}


typedef struct
{
  int   exprTmpVarNum;
  int   exprTmpVarMax;
} SimplifyVarAllocator;


static CCode* c2bf_simplify_code_internal(CONST CCode* ccode, int* has_break, int* has_continue, SimplifyVarAllocator* allocator);



/* ccode must not be used after that call */
static List* c2bf_list_append_code_and_merge_if_list(List* list, CCode* ccode)
{
  if (ccode->type == LIST)
  {
    list = list_merge(list, ccode->c_list);
    free(ccode);
  }
  else
  {
    list = list_append(list, ccode);
  }
  return list;
}


static CCode* c2bf_new_code_if(CExpr* cond, CCode* block, CCode* else_block)
{
  CCode* new_code = alloc0(CCode, 1);
  new_code->numLine = cond->numLine;
  new_code->type = IF;
  new_code->c_if = alloc0(CIf, 1);
  new_code->c_if->cond = cond;
  new_code->c_if->block = block;
  new_code->c_if->else_block = else_block;
  return new_code;
}

static CCode* c2bf_simplify_list(CONST CCode* ccode, int* has_break, int* has_continue, SimplifyVarAllocator* allocator)
{
  CCode* new_code = alloc0(CCode, 1);
  List* cur = ccode->c_list;
  int has_break_local = FALSE;
  int has_continue_local = FALSE;
  new_code->numLine = ccode->numLine;
  new_code->type = ccode->type;
  
  if (has_break == NULL) has_break = &has_break_local;
  if (has_continue == NULL) has_continue = &has_continue_local;
  
  while(cur)
  {
    CONST CCode* oldSubCode = (CONST CCode*)cur->data;
    CCode* newSubCode;
    
    newSubCode = c2bf_simplify_code_internal(oldSubCode, has_break, has_continue, allocator);
    new_code->c_list = c2bf_list_append_code_and_merge_if_list(new_code->c_list, newSubCode);
    cur = cur->next;
    
    if (*has_break == TRUE || *has_continue == TRUE)
    {
      int is_has_break = *has_break;
      CCode* if_block;
      CCode* new_block = alloc0(CCode, 1);
      CExpr* cond = c2bf_new_expr_var(is_has_break ? "__bf_tmp_no_break" : "__bf_tmp_no_continue", 0);
      CCode* simplified;
      new_block->numLine = 0;
      new_block->type = LIST;
      new_block->c_list = NULL;
      while (cur)
      {
        new_block->c_list = list_append(new_block->c_list, c2bf_dup_c_code((CCode*)cur->data));
        cur = cur->next;
      }
      if_block = c2bf_new_code_if(cond, new_block, NULL);
      *has_break = FALSE;
      *has_continue = FALSE;
      simplified = c2bf_simplify_code_internal(if_block, has_break, has_continue, allocator);
      if (is_has_break) *has_break = TRUE; else *has_continue = TRUE;
      c2bf_free_c_code(if_block);
      new_code->c_list = c2bf_list_append_code_and_merge_if_list(new_code->c_list, simplified);
      break;
    }
  }
  return new_code;
}

static CCode* c2bf_new_code_while(CExpr* cond, CCode* block)
{
  CCode* new_code = alloc0(CCode, 1);
  new_code->numLine = cond->numLine;
  new_code->type = WHILE;
  new_code->c_while = alloc0(CWhile, 1);
  new_code->c_while->cond = cond;
  new_code->c_while->block = block;
  return new_code;
}

static CCode* c2bf_new_code_list(CCode* internal_code, ...)
{
  CCode* new_code = alloc0(CCode, 1);
  va_list vlist;
  
  if (internal_code)
    new_code->numLine = internal_code->numLine;
  new_code->type = LIST;
  va_start(vlist, internal_code);
  while(internal_code)
  {
    new_code->c_list = list_append(new_code->c_list, internal_code);
    internal_code = va_arg(vlist, CCode*);
  }
  va_end(vlist);
  return new_code;
}

static CCode* c2bf_new_code_from_expr(CExpr* expr)
{
  CCode* new_code = alloc0(CCode, 1);
  new_code->numLine = expr->numLine;
  new_code->type = EXPRESSION;
  new_code->c_expr = expr;
  return new_code;
}

typedef struct
{
  List* listPre;
  List* list;
  List* listPost;
} SimplifyExprBuilder;


static void c2bf_simplify_c_expr(SimplifyExprBuilder* builder, SimplifyVarAllocator* allocator, CONST CExpr* expr)
{
  CExpr* exprTmp;
  char tmpName[512];

  switch(expr->type)
  {
    case CONSTANT:
    case CHARACTER:
    case VARIABLE:
    {
      builder->list = list_append(builder->list,
                                  c2bf_new_code_from_expr(c2bf_dup_c_expr(expr)));
      break;
    }
    
    case FUNCTION_CALL:
    {
      CExpr* newExpr = alloc0(CExpr, 1);
      List* cur = expr->ce_function_call->argument_list;
      newExpr->numLine = expr->numLine;
      newExpr->type = expr->type;
      newExpr->ce_function_call = alloc0(CFunctionCall, 1);
      newExpr->ce_function_call->function_name = strdup(expr->ce_function_call->function_name);
      newExpr->ce_function_call->argument_list = NULL;
      
      while(cur)
      {
        List* saved_builder_list;
        CCode* ret_ccode;
        CExpr* expr2;
        
        sprintf(tmpName, "__bf_expr_tmp_%d", allocator->exprTmpVarNum);
        allocator->exprTmpVarNum++;
        if (allocator->exprTmpVarNum > allocator->exprTmpVarMax)
          allocator->exprTmpVarMax = allocator->exprTmpVarNum;
        
        exprTmp = c2bf_new_expr_assign(
            c2bf_new_expr_var(tmpName, expr->numLine),
            c2bf_dup_c_expr((CExpr*)cur->data));
        
        saved_builder_list = builder->list;
        builder->list = NULL;
        c2bf_simplify_c_expr(builder, allocator, exprTmp);
        while(builder->list && builder->list->next)
        {
          List* next = builder->list->next;
          builder->listPre = list_append(builder->listPre, builder->list->data);
          free(builder->list);
          builder->list = next;
        }
        ret_ccode = (CCode*)builder->list->data;
        ASSERT(ret_ccode->type == EXPRESSION);
        expr2 = ret_ccode->c_expr;
        free(ret_ccode);
        free(builder->list);
        builder->list = saved_builder_list;
        
        if (expr2->type == ASSIGNMENT &&
            expr2->ce_expr_left->type == VARIABLE &&
            strcmp(expr2->ce_expr_left->ce_var, tmpName) == 0)
        {
          newExpr->ce_function_call->argument_list =
              list_append(newExpr->ce_function_call->argument_list,
                          c2bf_dup_c_expr(expr2->ce_expr_right));
          
          c2bf_free_expr(expr2);
        }
        else
        {
          builder->listPre = list_append(builder->listPre, c2bf_new_code_from_expr(expr2));
                
          newExpr->ce_function_call->argument_list =
              list_append(newExpr->ce_function_call->argument_list,
                          c2bf_dup_c_expr(exprTmp->ce_expr_left));
        }
        c2bf_free_expr(exprTmp);
        
        cur = cur->next;
      }
      
      builder->list = list_append(builder->list, c2bf_new_code_from_expr(newExpr));
      
      break;
    }
    
    case LOR:
    {
      /* a || b 
      -->
      if (a) __tmp = 1;
      else if (b) __tmp = 1;
      else __tmp = 0;
      __tmp;
      */
    
      CCode* tmp_if_code;
      CCode* while_code;
      
      tmp_if_code = c2bf_new_code_if(
        c2bf_dup_c_expr(expr->ce_expr_left),
        c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_lor", 1, expr->numLine)),
        c2bf_new_code_if(
          c2bf_dup_c_expr(expr->ce_expr_right),
          c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_lor", 1, expr->numLine)),
          c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_lor", 0, expr->numLine))
        )
      );
      
      while_code = c2bf_simplify_code_internal(tmp_if_code, NULL, NULL, allocator);
      c2bf_free_c_code(tmp_if_code);
      
      builder->list = c2bf_list_append_code_and_merge_if_list(builder->list, while_code);
      builder->list = list_append(builder->list, c2bf_new_code_from_expr(c2bf_new_expr_var("__bf_tmp_lor", expr->numLine)));
      
      break;
    }
    
    case LAND:
    {
      /* a && b 
      -->
      __tmp = 0;
      if (a)
      {
        if (b) __tmp = 1;
      };
      __tmp;
      */
      
      CCode* tmp_if_code;
      CCode* while_code;
      
      tmp_if_code = c2bf_new_code_if(
       c2bf_dup_c_expr(expr->ce_expr_left),
       c2bf_new_code_if(
          c2bf_dup_c_expr(expr->ce_expr_right),
            c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_land", 1, expr->numLine)),
          NULL),
       NULL
      );  
      
      while_code = c2bf_simplify_code_internal(tmp_if_code, NULL, NULL, allocator);
      c2bf_free_c_code(tmp_if_code);
      
      builder->list = list_append(builder->list,
                                  c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_land", 0, expr->numLine)));
      builder->list = c2bf_list_append_code_and_merge_if_list(builder->list, while_code);
      builder->list = list_append(builder->list, c2bf_new_code_from_expr(c2bf_new_expr_var("__bf_tmp_land", expr->numLine)));
      
      break;
    }
    
    case TERNARY_COND:
    {
      /* (a) ? b : c
         -->
         if (a)
           __ternary = b;
         else
           __ternary = c;
         __ternary;
      */
      
      CCode* tmp_if_code;
      CCode* while_code;
      
      tmp_if_code = c2bf_new_code_if(
        c2bf_dup_c_expr(expr->ce_expr_cond),
        c2bf_new_code_from_expr(c2bf_new_expr_assign(c2bf_new_expr_var("__bf_tmp_ternary", expr->numLine),
                                                           c2bf_dup_c_expr(expr->ce_expr_then))),
        c2bf_new_code_from_expr(c2bf_new_expr_assign(c2bf_new_expr_var("__bf_tmp_ternary", expr->numLine),
                                                           c2bf_dup_c_expr(expr->ce_expr_else)))
      );
      
      while_code = c2bf_simplify_code_internal(tmp_if_code, NULL, NULL, allocator);
      c2bf_free_c_code(tmp_if_code);
      
      builder->list = c2bf_list_append_code_and_merge_if_list(builder->list, while_code);
      builder->list = list_append(builder->list, c2bf_new_code_from_expr(c2bf_new_expr_var("__bf_tmp_ternary", expr->numLine)));
      
      break;
    }
    
    case NOT_EQUAL:
    {
      CCode* tmp_code;
      CCode* simplified_code;
      
      tmp_code = c2bf_new_code_from_expr(c2bf_new_expr_unary(
          UNARY_LNOT,
          c2bf_new_expr_binary(EQUAL, c2bf_dup_c_expr(expr->ce_expr_left), c2bf_dup_c_expr(expr->ce_expr_right))));
      
      simplified_code = c2bf_simplify_code_internal(tmp_code, NULL, NULL, allocator);
      c2bf_free_c_code(tmp_code);
      
      builder->list = c2bf_list_append_code_and_merge_if_list(builder->list, simplified_code);
      
      break;
    }

    case ADD_ASSIGNMENT:
    case SUB_ASSIGNMENT:
    case MUL_ASSIGNMENT:
    case DIV_ASSIGNMENT:
    case MOD_ASSIGNMENT:
    case BAND_ASSIGNMENT:
    case BXOR_ASSIGNMENT:
    case BOR_ASSIGNMENT:
    case SHL_ASSIGNMENT:
    case SHR_ASSIGNMENT:
    {
      CExpr* newExpr = alloc0(CExpr, 1);
      newExpr->type = ASSIGNMENT;
      newExpr->numLine = expr->numLine;
      newExpr->ce_expr_left = c2bf_dup_c_expr(expr->ce_expr_left);
      newExpr->ce_expr_right = alloc0(CExpr, 1);
      switch(expr->type)
      {
        case ADD_ASSIGNMENT:
          newExpr->ce_expr_right->type = ADD;
          break;
        case SUB_ASSIGNMENT:
          newExpr->ce_expr_right->type = SUB;
          break;
        case MUL_ASSIGNMENT:
          newExpr->ce_expr_right->type = MUL;
          break;
        case DIV_ASSIGNMENT:
          newExpr->ce_expr_right->type = DIV;
          break;
        case MOD_ASSIGNMENT:
          newExpr->ce_expr_right->type = MOD;
          break;
        case BAND_ASSIGNMENT:
          newExpr->ce_expr_right->type = BAND;
          break;
        case BXOR_ASSIGNMENT:
          newExpr->ce_expr_right->type = BXOR;
          break;
        case BOR_ASSIGNMENT:
          newExpr->ce_expr_right->type = BOR;
          break;
        case SHL_ASSIGNMENT:
          newExpr->ce_expr_right->type = SHL;
          break;
        case SHR_ASSIGNMENT:
          newExpr->ce_expr_right->type = SHR;
          break;
        default:
          SHOULDNT_HAPPEN();
      }
      newExpr->ce_expr_right->numLine = expr->numLine;
      newExpr->ce_expr_right->ce_expr_left = c2bf_dup_c_expr(expr->ce_expr_left);
      newExpr->ce_expr_right->ce_expr_right = c2bf_dup_c_expr(expr->ce_expr_right);
      
      c2bf_simplify_c_expr(builder, allocator, newExpr);
      c2bf_free_expr(newExpr);
      
      break;
    }

    default:
    {
      CONST COperator* coperator = c2bf_find_operator_of_expr_type(expr->type);
      int i;
      CExpr* newExpr = alloc0(CExpr, 1);
      newExpr->type = expr->type;
      newExpr->numLine = expr->numLine;
      for(i=0; i < coperator->n_ary; i++)
      {
        switch (expr->ce_expr_tab[i]->type)
        {
          case ASSIGNMENT:
          case ADD_ASSIGNMENT:
          case SUB_ASSIGNMENT:
          case MUL_ASSIGNMENT:
          case DIV_ASSIGNMENT:
          case MOD_ASSIGNMENT:
          case BAND_ASSIGNMENT:
          case BXOR_ASSIGNMENT:
          case BOR_ASSIGNMENT:
          case SHL_ASSIGNMENT:
          case SHR_ASSIGNMENT:
          {
            List* saved_builder_list = builder->list;
            builder->list = NULL;
            c2bf_simplify_c_expr(builder, allocator, expr->ce_expr_tab[i]);
            builder->listPre = list_append(builder->listPre, builder->list->data);
            free(builder->list);
            builder->list = saved_builder_list;
            
            newExpr->ce_expr_tab[i] = c2bf_dup_c_expr(expr->ce_expr_tab[i]->ce_expr_left);
            break;
          }

          case UNARY_PRE_INCREMENT:
          case UNARY_PRE_DECREMENT:
          {
            builder->listPre = list_prepend(builder->listPre,
                                            c2bf_new_code_from_expr(c2bf_dup_c_expr(expr->ce_expr_tab[i])));
            
            newExpr->ce_expr_tab[i] = c2bf_dup_c_expr(expr->ce_expr_tab[i]->ce_expr);
            break;
          }
          
          case UNARY_POST_INCREMENT:
          case UNARY_POST_DECREMENT:
          {
            builder->listPost = list_append(builder->listPost,
                                            c2bf_new_code_from_expr(c2bf_dup_c_expr(expr->ce_expr_tab[i])));
            
            newExpr->ce_expr_tab[i] = c2bf_dup_c_expr(expr->ce_expr_tab[i]->ce_expr);
            break;
          }
          
          default:
          {
            List* saved_builder_list = builder->list;
            CCode* ret_ccode;
            builder->list = NULL;
            c2bf_simplify_c_expr(builder, allocator, expr->ce_expr_tab[i]);
            while(builder->list && builder->list->next)
            {
              List* next = builder->list->next;
              builder->listPre = list_append(builder->listPre, builder->list->data);
              free(builder->list);
              builder->list = next;
            }
            ret_ccode = (CCode*)builder->list->data;
            ASSERT(ret_ccode->type == EXPRESSION);
            newExpr->ce_expr_tab[i] = ret_ccode->c_expr;
            free(ret_ccode);
            free(builder->list);
            builder->list = saved_builder_list;
                        
            if (newExpr->ce_expr_tab[i]->type != VARIABLE)
            {
              sprintf(tmpName, "__bf_expr_tmp_%d", allocator->exprTmpVarNum);
              allocator->exprTmpVarNum++;
              if (allocator->exprTmpVarNum > allocator->exprTmpVarMax)
                allocator->exprTmpVarMax = allocator->exprTmpVarNum;
              exprTmp = c2bf_new_expr_assign(
                  c2bf_new_expr_var(tmpName, expr->numLine),
                  newExpr->ce_expr_tab[i]);
              builder->listPre = list_append(builder->listPre, c2bf_new_code_from_expr(exprTmp));
              
              newExpr->ce_expr_tab[i] = c2bf_dup_c_expr(exprTmp->ce_expr_left);
            }
          }
        }
      }

      builder->list = list_append(builder->list, c2bf_new_code_from_expr(newExpr));
    }
  }
}

static CCode* c2bf_simplify_c_expr_to_code(CONST CExpr* expr, SimplifyVarAllocator* allocator)
{
  CCode* new_code = alloc0(CCode, 1);
  SimplifyExprBuilder builder;
  
  builder.listPre = NULL;
  builder.listPost = NULL;
  builder.list = NULL;
  
  allocator->exprTmpVarNum = 0;
      
  c2bf_simplify_c_expr(&builder, allocator, expr);
  
  new_code->numLine = expr->numLine;
  new_code->type = LIST;
  new_code->c_list = NULL;
  new_code->c_list = list_merge(new_code->c_list, builder.listPre);
  new_code->c_list = list_merge(new_code->c_list, builder.list);
  new_code->c_list = list_merge(new_code->c_list, builder.listPost);
  
  return new_code;
}

static void c2bf_deal_with_special_last_expr_for_while_cond(CCode* block, SimplifyVarAllocator* allocator, CONST char* varName)
{
  CCode* last_expr_of_cond_code = (CCode*)(list_end(block->c_list)->data);

  CCode* simplified_last_expr_of_cond_code = NULL;
  ASSERT(last_expr_of_cond_code->type == EXPRESSION);
  last_expr_of_cond_code->c_expr =
      c2bf_new_expr_assign(c2bf_new_expr_var(varName, last_expr_of_cond_code->numLine),
                               last_expr_of_cond_code->c_expr);
  simplified_last_expr_of_cond_code = c2bf_simplify_code_internal(last_expr_of_cond_code, NULL, NULL, allocator);
  c2bf_free_c_code(last_expr_of_cond_code);
  if (block->c_list == list_end(block->c_list))
  {
    list_free_elt(block->c_list, NULL);
    block->c_list = NULL;
  }
  else
    list_free_elt(list_end(block->c_list), NULL);
  block->c_list = c2bf_list_append_code_and_merge_if_list(block->c_list, simplified_last_expr_of_cond_code);
}

static int c2bf_has_loop_break_or_continue(CONST CCode* ccode, CCodeType statement)
{
  List* cur;
  if (ccode->type == statement)
    return TRUE;
  else if (ccode->type == LIST)
  {
    cur = ccode->c_list;
    while(cur)
    {
      CONST CCode* subCode = (CONST CCode*)cur->data;
      if (subCode->type == statement)
      {
        return TRUE;
      }
      
      switch (subCode->type)
      {
        case IF:
          if (c2bf_has_loop_break_or_continue(subCode->c_if->block, statement))
            return TRUE;
          break;
          
        case LIST:
          if (c2bf_has_loop_break_or_continue(subCode, statement))
            return TRUE;
          break;
        
        default:
          break;
      }
      cur = cur->next;
    }
  }
  return FALSE;
}

static int c2bf_has_loop_break(CONST CCode* ccode)
{
  return c2bf_has_loop_break_or_continue(ccode, BREAK);
}

static int c2bf_has_loop_continue(CONST CCode* ccode)
{
  return c2bf_has_loop_break_or_continue(ccode, CONTINUE);
}

static CCode* c2bf_simplify_code_for_or_while(CCode* src_init_block,
                                              CExpr* src_cond,
                                              CCode* src_block,
                                              CCode* src_update_block,
                                              int numLine,
                                              SimplifyVarAllocator* allocator)
{
  CCode* new_wrapping_code = NULL;
  CCode* new_code = NULL;
  int has_break = c2bf_has_loop_break(src_block);
  int has_continue = c2bf_has_loop_continue(src_block);
  CCode* init_block = (src_init_block) ? c2bf_simplify_code_internal(src_init_block, NULL, NULL, allocator) : NULL;
  CCode* cond_code;
  CCode* dup_cond_code;
  CCode* block;
  CCode* update_block = (src_update_block) ? c2bf_simplify_code_internal(src_update_block, NULL, NULL, allocator) : NULL;
  CCode* last_expr_of_cond_code = NULL;
  
  if (has_break)
  {
    CExpr* new_cond = c2bf_new_expr_binary(LAND, c2bf_new_expr_var("__bf_tmp_no_break", src_cond->numLine), c2bf_dup_c_expr(src_cond));
    cond_code = c2bf_simplify_c_expr_to_code(new_cond, allocator);
    c2bf_free_expr(new_cond);
  }
  else
  {
    cond_code = c2bf_simplify_c_expr_to_code(src_cond, allocator);
  }
  dup_cond_code = c2bf_dup_c_code(cond_code);
  
  new_wrapping_code = alloc0(CCode, 1);
  new_wrapping_code->numLine = numLine;
  new_wrapping_code->type = LIST;
  new_wrapping_code->c_list = NULL;
  if (has_break)
  {
    new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list,
      c2bf_new_code_from_expr(
        c2bf_new_expr_assign_constant("__bf_tmp_no_break", 1, 0)));
  }
  if (has_continue)
  {
    new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list,
        c2bf_new_code_from_expr(
            c2bf_new_expr_assign_constant("__bf_tmp_no_continue", 1, 0)));
  }
  if (init_block)
  {
    new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list, init_block);
  }
  new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list, cond_code);
      
  new_code = alloc0(CCode, 1);
  new_code->numLine = numLine;
  new_code->type = WHILE;
  new_code->c_while = alloc0(CWhile, 1);
            
      
  if (has_continue)
  {
    CCode* new_block;
    new_block = alloc0(CCode, 1);
    new_block->type = LIST;
    new_block->numLine = src_block->numLine;
    new_block->c_list = NULL;
    new_block->c_list = c2bf_list_append_code_and_merge_if_list(new_block->c_list,
        c2bf_new_code_from_expr(
            c2bf_new_expr_assign_constant("__bf_tmp_no_continue", 1, 0)));
    new_block->c_list = c2bf_list_append_code_and_merge_if_list(new_block->c_list,
        c2bf_simplify_code_internal(src_block, NULL, NULL, allocator));
    block = new_block;
  }
  else
  {
    block = c2bf_simplify_code_internal(src_block, NULL, NULL, allocator);
  }
  
  if (block->type != LIST)
  {
    CCode* trueblock = block;
    block = alloc0(CCode, 1);
    block->type = LIST;
    block->numLine = trueblock->numLine;
    block->c_list = list_append(NULL, trueblock);
  }
  if (update_block)
  {
    if (has_break)
    {
      CCode* if_break = c2bf_new_code_if(c2bf_new_expr_var("__bf_tmp_no_break", 0), update_block, NULL);
      CCode* simplified = c2bf_simplify_code_internal(if_break, NULL, NULL, allocator);
      block->c_list = c2bf_list_append_code_and_merge_if_list(block->c_list, simplified);
      c2bf_free_c_code(if_break);
    }
    else
    {
      block->c_list = c2bf_list_append_code_and_merge_if_list(block->c_list, update_block);
    }
  }
  
  block->c_list = c2bf_list_append_code_and_merge_if_list(block->c_list, dup_cond_code);
            
  last_expr_of_cond_code = (CCode*)(list_end(new_wrapping_code->c_list)->data);
  ASSERT(last_expr_of_cond_code->type == EXPRESSION);
  if (!(last_expr_of_cond_code->c_expr->type == ASSIGNMENT &&
        last_expr_of_cond_code->c_expr->ce_expr_right->type == VARIABLE))
  {
    c2bf_deal_with_special_last_expr_for_while_cond(block, allocator, "__bf_tmp_while_cond");
    c2bf_deal_with_special_last_expr_for_while_cond(new_wrapping_code, allocator, "__bf_tmp_while_cond");
        
    new_code->c_while->cond = c2bf_new_expr_var("__bf_tmp_while_cond", numLine);
  }
  else
  {
    new_code->c_while->cond = c2bf_new_expr_var(last_expr_of_cond_code->c_expr->ce_var, numLine);
  }

  new_code->c_while->block = block;
      
  new_wrapping_code->c_list = list_append(new_wrapping_code->c_list, new_code);
  
  if (has_break)
  {
    new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list,
      c2bf_new_code_from_expr(
          c2bf_new_expr_assign_constant("__bf_tmp_no_break", 1, 0)));
  }
  if (has_continue)
  {
    new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list,
        c2bf_new_code_from_expr(
            c2bf_new_expr_assign_constant("__bf_tmp_no_continue", 1, 0)));
  }
  
  return new_wrapping_code;
}

static CCode* c2bf_simplify_code_internal(CONST CCode* ccode, int* has_break, int* has_continue, SimplifyVarAllocator* allocator)
{
  switch (ccode->type)
  {
    case LIST:
      return c2bf_simplify_list(ccode, has_break, has_continue, allocator);
      break;
      
    case DECLARATION:
      return c2bf_dup_c_code(ccode);
      break;
      
    case EXPRESSION:
    {
      return c2bf_simplify_c_expr_to_code(ccode->c_expr, allocator);
      break;
    }
      
    case WHILE:
    {
      if (has_break) *has_break = FALSE;
      if (has_continue) *has_continue = FALSE;
      return c2bf_simplify_code_for_or_while(NULL,
                                             ccode->c_while->cond,
                                             ccode->c_while->block,
                                             NULL,
                                             ccode->numLine,
                                             allocator);
      break;
    }
      
    case DO_WHILE:
    {
      CCode* new_wrapping_code = NULL;
      CCode* new_code = NULL;
      int local_has_break = c2bf_has_loop_break(ccode->c_do_while->block);
      int local_has_continue = c2bf_has_loop_continue(ccode->c_do_while->block);
      CCode* block;
      CCode* cond_code;
      CCode* init_expr_code = NULL;
      
      if (local_has_break == TRUE)
      {
        CExpr* new_cond = c2bf_new_expr_binary(LAND,
                                               c2bf_new_expr_var("__bf_tmp_no_break", ccode->c_do_while->cond->numLine),
                                               c2bf_dup_c_expr(ccode->c_do_while->cond));
        cond_code = c2bf_simplify_c_expr_to_code(new_cond, allocator);
        c2bf_free_expr(new_cond);
      }
      else
      {
        cond_code = c2bf_simplify_c_expr_to_code(ccode->c_do_while->cond, allocator);
      }
      
      if (local_has_continue)
      {
        CCode* new_block;
        new_block = alloc0(CCode, 1);
        new_block->type = LIST;
        new_block->numLine = ccode->c_do_while->block->numLine;
        new_block->c_list = NULL;
        new_block->c_list = c2bf_list_append_code_and_merge_if_list(new_block->c_list,
            c2bf_new_code_from_expr(
                c2bf_new_expr_assign_constant("__bf_tmp_no_continue", 1, 0)));
        new_block->c_list = c2bf_list_append_code_and_merge_if_list(new_block->c_list,
            c2bf_simplify_code_internal(ccode->c_do_while->block, NULL, NULL, allocator));
        block = new_block;
      }
      else
      {
        block = c2bf_simplify_code_internal(ccode->c_do_while->block, NULL, NULL, allocator);
      }
      
      new_wrapping_code = alloc0(CCode, 1);
      new_wrapping_code->numLine = ccode->numLine;
      new_wrapping_code->type = LIST;
      
      init_expr_code = alloc0(CCode, 1);
      init_expr_code->numLine = ccode->numLine;
      init_expr_code->type = EXPRESSION;
      init_expr_code->c_expr =
          c2bf_new_expr_assign(c2bf_new_expr_var("__bf_tmp_while_cond", ccode->numLine),
                                   c2bf_new_expr_constant(1, ccode->numLine));
      
      if (block->type != LIST)
      {
        CCode* trueblock = block;
        block = alloc0(CCode, 1);
        block->type = LIST;
        block->numLine = trueblock->numLine;
        block->c_list = NULL;
        block->c_list = list_append(block->c_list, trueblock);
      }
      block->c_list = c2bf_list_append_code_and_merge_if_list(block->c_list, cond_code);
      c2bf_deal_with_special_last_expr_for_while_cond(block, allocator, "__bf_tmp_while_cond");
        
      new_code = alloc0(CCode, 1);
      new_code->numLine = ccode->numLine;
      new_code->type = WHILE;
      new_code->c_while = alloc0(CWhile, 1);
      new_code->c_while->cond = c2bf_new_expr_var("__bf_tmp_while_cond", ccode->numLine);
      new_code->c_while->block = block;
      
      new_wrapping_code->c_list = NULL;
      if (local_has_break)
      {
        new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list,
            c2bf_new_code_from_expr(
                c2bf_new_expr_assign_constant("__bf_tmp_no_break", 1, 0)));
      }
      new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list, init_expr_code);
      new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list, new_code);
      
      if (local_has_break)
      {
        new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list,
          c2bf_new_code_from_expr(
              c2bf_new_expr_assign_constant("__bf_tmp_no_break", 1, 0)));
      }
      if (local_has_continue)
      {
        new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(new_wrapping_code->c_list,
            c2bf_new_code_from_expr(
                c2bf_new_expr_assign_constant("__bf_tmp_no_continue", 1, 0)));
      }
      if (has_break) *has_break = FALSE;
      if (has_continue) *has_continue = FALSE;
      
      return new_wrapping_code;
      
      break;
    }
      
    case IF:
    {
      /* if(a) { foo } else { bar} 
         -->
         __if_expr_cond = a;
         __then_expr_cond = 1;
         while(__if_expr_cond)
         {
           foo
           __if_expr = 0;
           __then_expr_cond = 0;
         }
         while(__then_expr_cond)
         {
           bar
           __then_expr_cond = 0
         }
      */
      CCode* new_wrapping_code = NULL;
      CCode* new_code = NULL;
      CCode* block = c2bf_simplify_code_internal(ccode->c_if->block, has_break, has_continue, allocator);
      CCode* else_block = (ccode->c_if->else_block) ?
                            c2bf_simplify_code_internal(ccode->c_if->else_block, has_break, has_continue, allocator) : NULL;
      CCode* cond_code = c2bf_simplify_c_expr_to_code(ccode->c_if->cond, allocator);
      
      new_wrapping_code = alloc0(CCode, 1);
      new_wrapping_code->numLine = ccode->numLine;
      new_wrapping_code->type = LIST;
      
      new_wrapping_code->c_list = c2bf_list_append_code_and_merge_if_list(NULL, cond_code);
      c2bf_deal_with_special_last_expr_for_while_cond(new_wrapping_code, allocator, "__bf_tmp_if_expr_cond");
      
      if (else_block)
      {
        new_wrapping_code->c_list = list_append( new_wrapping_code->c_list,
          c2bf_new_code_from_expr(
              c2bf_new_expr_assign_constant("__bf_tmp_then_expr_cond", 1, ccode->numLine)));
      }
      
      if (block->type != LIST)
      {
        CCode* trueblock = block;
        block = alloc0(CCode, 1);
        block->type = LIST;
        block->numLine = trueblock->numLine;
        block->c_list = list_append(NULL, trueblock);
      }
      block->c_list = list_append(block->c_list,
        c2bf_new_code_from_expr(
            c2bf_new_expr_assign_constant("__bf_tmp_if_expr_cond", 0, ccode->numLine)));
      if (else_block)
      {
        block->c_list = list_append(block->c_list,
          c2bf_new_code_from_expr(
            c2bf_new_expr_assign_constant("__bf_tmp_then_expr_cond", 0, ccode->numLine)));
      }
      
      new_code = alloc0(CCode, 1);
      new_code->numLine = ccode->numLine;
      new_code->type = WHILE;
      new_code->c_while = alloc0(CWhile, 1);
      new_code->c_while->cond = c2bf_new_expr_var("__bf_tmp_if_expr_cond", ccode->numLine);
      new_code->c_while->block = block;
      new_wrapping_code->c_list = list_append(new_wrapping_code->c_list, new_code);
      
      if (else_block)
      {
        block = NULL;
        if (else_block->type != LIST)
        {
          CCode* trueblock = else_block;
          else_block = alloc0(CCode, 1);
          else_block->type = LIST;
          else_block->numLine = trueblock->numLine;
          else_block->c_list = list_append(NULL, trueblock);
        }
        else_block->c_list = list_append(else_block->c_list,
            c2bf_new_code_from_expr(
              c2bf_new_expr_assign_constant("__bf_tmp_then_expr_cond", 0, ccode->numLine)));
        
        new_code = alloc0(CCode, 1);
        new_code->numLine = ccode->numLine;
        new_code->type = WHILE;
        new_code->c_while = alloc0(CWhile, 1);
        new_code->c_while->cond = c2bf_new_expr_var("__bf_tmp_then_expr_cond", ccode->numLine);
        new_code->c_while->block = else_block;
        new_wrapping_code->c_list = list_append(new_wrapping_code->c_list, new_code);
      }

      return new_wrapping_code;
      break;
    }
      
    case FOR:
    {
      if (has_break) *has_break = FALSE;
      if (has_continue) *has_continue = FALSE;
      
      return c2bf_simplify_code_for_or_while(ccode->c_for->init_block,
                                             ccode->c_for->cond,
                                             ccode->c_for->block,
                                             ccode->c_for->update_block,
                                             ccode->numLine,
                                             allocator);

      break;
    }
    
    case BREAK:
    {
      if (has_break) *has_break = TRUE;
      return c2bf_new_code_from_expr(
               c2bf_new_expr_assign_constant("__bf_tmp_no_break", 0, ccode->numLine));
      break;
    }
    
    case CONTINUE:
    {
      if (has_continue) *has_continue = TRUE;
      return c2bf_new_code_from_expr(
          c2bf_new_expr_assign_constant("__bf_tmp_no_continue", 0, ccode->numLine));
      break;
    }
      
    case NOTHING_STATEMENT:
      return c2bf_dup_c_code(ccode);
      break;
      
    default:
      SHOULDNT_HAPPEN();
  }
  
  SHOULDNT_HAPPEN();
  return NULL;
}

static CCode* c2bf_new_code_declaration(CONST char* varName)
{
  CCode* code;
  code = alloc0(CCode, 1);
  code->type = DECLARATION;
  code->c_decl = alloc0(CDeclaration, 1);
  code->c_decl->var = strdup(varName);
  return code;
}

static CCode* c2bf_simplify_code(CONST CCode* ccode)
{
  SimplifyVarAllocator allocator;
  CCode* new_code;
  int i;
  int has_break = FALSE;
  int has_continue = FALSE;
  
  allocator.exprTmpVarNum = 0;
  allocator.exprTmpVarMax = -1;

  new_code = c2bf_simplify_code_internal(ccode, &has_break, &has_continue, &allocator);
  for(i=0; i<=allocator.exprTmpVarMax;i++)
  {
    char tmpName[512];
    sprintf(tmpName, "__bf_expr_tmp_%d", i);
    new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration(tmpName));
  }
  
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_no_break"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_no_continue"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_while_cond"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_if_expr_cond"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_then_expr_cond"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_lor"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_land"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_ternary"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_assign_constant"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_assign_constant2"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_affect"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_add"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_lnot"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_neg"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_mul"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_mul2"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_shrl"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_shrl2"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_div"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_div2"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_div3"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_div4"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_equal"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_equal2"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_equal3"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_equal4"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_comp"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_comp2"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_comp3"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_comp4"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_comp5"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_comp6"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_comp7"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_putchar"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_putchar_tmp"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_printf_do_putchar"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_printf_d"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_printf_d2"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_printf_d3"));
  new_code->c_list = list_prepend(new_code->c_list, c2bf_new_code_declaration("__bf_tmp_printf_d4"));
  return new_code;
}

static CExpr* c2bf_new_expr_function_call(CONST char* function_name, CExpr* expr_arg, ...)
{
  va_list vlist;
  CExpr* expr = alloc0(CExpr, 1);
  expr->type = FUNCTION_CALL;
  expr->ce_function_call = alloc0(CFunctionCall, 1);
  expr->ce_function_call->function_name = strdup(function_name);
  va_start(vlist, expr_arg);
  while(expr_arg != NULL)
  {
    expr->ce_function_call->argument_list = list_append(
      expr->ce_function_call->argument_list,
      expr_arg);
    expr_arg = va_arg(vlist, CExpr*);
  }
  va_end(vlist);
  return expr;
}

static CCode* c2bf_ultra_simplify_greater_or_greater_equal_for_bf_set_comp4_if_comp3_is_0()
{
  return c2bf_new_code_list(
    c2bf_new_code_from_expr(c2bf_new_expr_increment("__bf_tmp_comp4", 0)),
    c2bf_new_code_while(
      c2bf_new_expr_var("__bf_tmp_comp3", 0),
      c2bf_new_code_list(
        c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_comp3", 0, 0)),
        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp4", 0)),
        NULL
      )
    ),
    NULL
  );
}

static CCode* c2bf_ultra_simplify_greater_or_greater_equal_for_bf_test_0_0(CExpr* expr, int isOpGreater)
{
  return c2bf_new_code_list(
    c2bf_ultra_simplify_greater_or_greater_equal_for_bf_set_comp4_if_comp3_is_0(),
    c2bf_new_code_while(
      c2bf_new_expr_var("__bf_tmp_comp4", 0),
      c2bf_new_code_list(
        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp4", 0)),
        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp5", 0)),
        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp6", 0)),
        ((!isOpGreater) ?
            c2bf_new_code_list(
              c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)),
              c2bf_new_code_while(
                c2bf_new_expr_var("__bf_tmp_comp2", 0),
                c2bf_new_code_list(
                  c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_comp2", 0, 0)),
                  c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_left->ce_var, 0)),
                  NULL
                )
              ),
              NULL
            )
            :
            c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp7", 0))
        ),
        NULL
      )
    ),
    NULL
  );
}

static CCode* c2bf_ultra_simplify_greater_or_greater_equal_for_bf(CExpr* expr)
{
  /* a = (b >= c) ou a = (b > c); -->
    comp = b;
    comp2 = c;
    a = 0;
    comp3 = comp;
    comp4++;
    comp5++;
    comp6++;
    if (op == ">") comp7++;
    while(comp3) {
      comp3 = 0;
      comp4--;
    }
    while(comp4)
    {
      comp4--;
      comp5--;
      comp6--;
      if (op == ">")
        comp7--;
      else
      {
        a++;
        while(comp2)
        {
          comp2 = 0;
          a--;
        }
      }
    }
    if (op == ">")
    {
      while(comp7)
      {
        comp7--;
        comp--;

        comp3 = comp;
        comp4++;
        while(comp3) {
          comp3 = 0;
          comp4--;
        }
        while(comp4)
        {
          comp4--;
          comp5--;
          comp6--;
          a++;
          while(comp2)
          {
            comp2 = 0;
            a--;
          }
        }
      }
    }
    while(comp5)
    {
      comp5--;
      comp3 = comp2;
      comp4++;
      while(comp3)
      {
        comp3 = 0;
        comp4--;
      }
      while(comp4)
      {
        comp4--;
        comp6--;
        a++;
      }
    }
    while(comp6)
    {
      comp6--;
      while(comp)
      {
        comp--;
        comp2--;
        comp3 = comp2;
        comp4++;
        while(comp3) {
          comp3 = 0;
          comp4--;
        }
        while(comp4)
        {
          comp4--;
          comp = 0;
          a++;
        }
      }
    } */

  CCode* new_code;
  ASSERT(expr->ce_expr_right->ce_expr_left->type == VARIABLE);
  ASSERT(expr->ce_expr_right->ce_expr_right->type == VARIABLE);
  new_code = c2bf_new_code_list(
    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_comp", expr->ce_expr_right->ce_expr_left->ce_var, 0)),
    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_comp2", expr->ce_expr_right->ce_expr_right->ce_var, 0)),
    c2bf_new_code_from_expr(c2bf_new_expr_assign_constant(expr->ce_expr_left->ce_var, 0, 0)),
    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_comp3", "__bf_tmp_comp", 0)),
    c2bf_new_code_from_expr(c2bf_new_expr_increment("__bf_tmp_comp5", 0)),
    c2bf_new_code_from_expr(c2bf_new_expr_increment("__bf_tmp_comp6", 0)),
    ((expr->ce_expr_right->type == GREATER_EQUAL) ?
      c2bf_new_code_list(NULL) :
      c2bf_new_code_from_expr(c2bf_new_expr_increment("__bf_tmp_comp7", 0))
    ),
    c2bf_ultra_simplify_greater_or_greater_equal_for_bf_test_0_0(expr, (expr->ce_expr_right->type == GREATER)),
    ((expr->ce_expr_right->type == GREATER_EQUAL) ?
      c2bf_new_code_list(NULL) :
      c2bf_new_code_while(
        c2bf_new_expr_var("__bf_tmp_comp7", 0),
        c2bf_new_code_list(
          c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp7", 0)),
          c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp", 0)),
          c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_comp3", "__bf_tmp_comp", 0)),
          c2bf_ultra_simplify_greater_or_greater_equal_for_bf_test_0_0(expr, FALSE),
          NULL
        )
      )
    ),

    c2bf_new_code_while(
      c2bf_new_expr_var("__bf_tmp_comp5", 0),
      c2bf_new_code_list(
        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp5", 0)),
        c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_comp3", "__bf_tmp_comp2", 0)),
        c2bf_ultra_simplify_greater_or_greater_equal_for_bf_set_comp4_if_comp3_is_0(),
        c2bf_new_code_while(
          c2bf_new_expr_var("__bf_tmp_comp4", 0),
          c2bf_new_code_list(
            c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp4", 0)),
            c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp6", 0)),
            c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)),
            NULL
          )
        ),
        NULL
      )
    ),

    c2bf_new_code_while(
      c2bf_new_expr_var("__bf_tmp_comp6", 0),
      c2bf_new_code_list(
        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp6", 0)),
        c2bf_new_code_while(
          c2bf_new_expr_var("__bf_tmp_comp", 0),
          c2bf_new_code_list(
            c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp", 0)),
            c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp2", 0)),
            c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_comp3", "__bf_tmp_comp2", 0)),
            c2bf_ultra_simplify_greater_or_greater_equal_for_bf_set_comp4_if_comp3_is_0(),
            c2bf_new_code_while(
              c2bf_new_expr_var("__bf_tmp_comp4", 0),
              c2bf_new_code_list(
                c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_comp4", 0)),
                c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_comp", 0, 0)),
                c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)),
                NULL
              )
            ),
            NULL
          )
        ),
        NULL
      )
    ),
    NULL
  );
  return new_code;
}

static CCode* c2bf_ultra_simplify_div_or_mod_for_bf(CExpr* expr)
{
  /* q = b / c; -->
    tmp_div = b;
    tmp_div2 = c;
    q = 0;
    r = tmp_div;
    while(tmp_div)
    {
      tmp_div--;
      tmp_div2--;
      tmp_div3 = tmp_div2;
      tmp_div4++;
      while(tmp_div3)
      {
        tmp_div3 = 0;
        tmp_div4--;
      }
      while(tmp_div4)
      {
        tmp_div4--;
        q++;
        r = tmp_div;
        tmp_div2 = c;
      }
    }
  */
  return c2bf_new_code_list(
    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_div", expr->ce_expr_right->ce_expr_left->ce_var, 0)),
    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_div2", expr->ce_expr_right->ce_expr_right->ce_var, 0)),
    ((expr->ce_expr_right->type == DIV) ?
      c2bf_new_code_from_expr(c2bf_new_expr_assign_constant(expr->ce_expr_left->ce_var, 0, 0)) :
      c2bf_new_code_from_expr(c2bf_new_expr_assign_var(expr->ce_expr_left->ce_var, "__bf_tmp_div", 0))
    ),
    c2bf_new_code_while(
      c2bf_new_expr_var("__bf_tmp_div", 0),
      c2bf_new_code_list(
        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_div", 0)),
        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_div2", 0)),
        c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_div3", "__bf_tmp_div2", 0)),
        c2bf_new_code_from_expr(c2bf_new_expr_increment("__bf_tmp_div4", 0)),
        c2bf_new_code_while(
          c2bf_new_expr_var("__bf_tmp_div3", 0),
          c2bf_new_code_list(
            c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_div3", 0, 0)),
            c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_div4", 0)),
            NULL
          )
        ),
        c2bf_new_code_while(
          c2bf_new_expr_var("__bf_tmp_div4", 0),
          c2bf_new_code_list(
            c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_div4", 0)),
            ((expr->ce_expr_right->type == DIV) ?
             c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)) :
             c2bf_new_code_from_expr(c2bf_new_expr_assign_var(expr->ce_expr_left->ce_var, "__bf_tmp_div", 0))
            ),
            c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_div2", expr->ce_expr_right->ce_expr_right->ce_var, 0)),
            NULL
          )
        ),
        NULL
      )
    ),
    NULL
  );
}

static List* c2bf_ultra_simplify_for_bf_merge_list(List* begin, List* cur, CCode* subCode)
{
  List* next = cur->next;
  ASSERT(cur == begin || cur->prev);
  ASSERT(subCode->type == LIST);
  if (subCode->c_list)
  {
    if (cur == begin)
    {
      begin = subCode->c_list;
      if (next) next->prev = list_end(subCode->c_list);
      if (subCode->c_list) list_end(subCode->c_list)->next = next;
    }
    else
    {
      cur->prev->next = subCode->c_list;
      subCode->c_list->prev = cur->prev;
      if (next) next->prev = list_end(subCode->c_list);
      list_end(subCode->c_list)->next = next;
    }
  }
  else
  {
    if (cur == begin)
    {
      begin = next;
      begin->prev = NULL;
    }
    else
    {
      cur->prev->next = next;
      next->prev = cur->prev;
    }
  }
  free(subCode);
  free(cur);
  return begin;
}

static void c2bf_ultra_simplify_for_bf(CCode* ccode)
{
  List* cur;
  ASSERT (ccode->type == LIST);
  cur = ccode->c_list;
    
  while(cur)
  {
    List* next = cur->next;
    CCode* subCode = (CCode*)cur->data;
    
    switch(subCode->type)
    {
      case LIST:
      {
        c2bf_ultra_simplify_for_bf(subCode);
        ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, subCode);
        break;
      }

      case EXPRESSION:
      {
        CExpr* expr = subCode->c_expr;
        switch (expr->type)
        {
          case VARIABLE:
          {
            c2bf_free_c_code(subCode);
            ccode->c_list = list_free_elt_and_update_head(ccode->c_list, cur, NULL);
            break;
          }

          case UNARY_PRE_INCREMENT:
          case UNARY_PRE_DECREMENT:
          case UNARY_POST_INCREMENT:
          case UNARY_POST_DECREMENT:
            break;
          
          case ASSIGNMENT:
          {
            ASSERT(expr->ce_expr_left->type == VARIABLE);
            switch(expr->ce_expr_right->type)
            {
              case CHARACTER:
              case CONSTANT:
              { /* a = X  --> while(a) a--; a++; a++; ....; a++ */
                CCode* new_code;
                int i;
                new_code =
                  c2bf_new_code_list(
                    c2bf_new_code_while(c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                                        c2bf_new_code_list(
                                          c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_left->ce_var, 0)),
                                        NULL)
                                       ),
                    NULL
                );
                if (expr->ce_expr_right->ce_constant >= 0)
                {
                  int constant = expr->ce_expr_right->ce_constant;
                  int tabMod[10];
                  int indice = 0;
                  memset(tabMod, 0, sizeof(int) * 10);
                  if (constant > 100)
                  {
                    do
                    {
                      tabMod[indice] = constant % 100;
                      constant /= 100;
                      indice++;
                    } while(constant != 0);
                    indice--;
                  }
                  else
                  {
                    tabMod[0] = constant;
                  }
                  for(i=0;i<tabMod[indice];i++)
                  {
                    new_code->c_list =
                      list_append(new_code->c_list, c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)));
                  }
                  indice--;
                  while(indice >= 0)
                  {
                    new_code->c_list =
                        list_append(new_code->c_list, c2bf_new_code_from_expr(
                                                                  c2bf_new_expr_assign_constant("__bf_tmp_assign_constant", 100, 0)));

                    new_code->c_list =
                        list_append(new_code->c_list, c2bf_new_code_from_expr(
                          c2bf_new_expr_assign_var("__bf_tmp_assign_constant2", expr->ce_expr_left->ce_var, 0)));

                    new_code->c_list = list_append(new_code->c_list, c2bf_new_code_from_expr(
                      c2bf_new_expr_assign(c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                                           c2bf_new_expr_binary(MUL,
                                             c2bf_new_expr_var("__bf_tmp_assign_constant2", 0),
                                             c2bf_new_expr_var("__bf_tmp_assign_constant", 0)))));

                    for(i=0;i<tabMod[indice];i++)
                    {
                      new_code->c_list =
                          list_append(new_code->c_list, c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)));
                    }

                    indice--;
                  }
                }
                else
                {
                  for(i=0;i<-expr->ce_expr_right->ce_constant;i++)
                  {
                    new_code->c_list =
                      list_append(new_code->c_list, c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_left->ce_var, 0)));
                  }
                }
                c2bf_free_c_code(subCode);
                c2bf_ultra_simplify_for_bf(new_code);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }
              
              case ADD:
              {
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr_left->type == VARIABLE);
                ASSERT(expr->ce_expr_right->ce_expr_right->type == VARIABLE);
                if (strcmp(expr->ce_expr_left->ce_var, expr->ce_expr_right->ce_expr_left->ce_var) == 0)
                { /* a = a + b --> tmp_b = b; while(tmp_b) { tmp_b--; a++; } */
                  new_code = c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_add", expr->ce_expr_right->ce_expr_right->ce_var, 0)),
                    c2bf_new_code_while(
                      c2bf_new_expr_var("__bf_tmp_add", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_add", 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)),
                        NULL
                      )
                    ),
                    NULL
                  );
                }
                else if (strcmp(expr->ce_expr_left->ce_var, expr->ce_expr_right->ce_expr_right->ce_var) == 0)
                { /* a = b + a --> tmp_b = b; while(tmp_b) { tmp_b--; a++; } */
                  new_code = c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_add", expr->ce_expr_right->ce_expr_left->ce_var, 0)),
                    c2bf_new_code_while(
                      c2bf_new_expr_var("__bf_tmp_add", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_add", 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)),
                        NULL
                      )
                    ),
                    NULL
                  );
                }
                else 
                { /* a = b + c --> a = b; a = a + c; } */
                  new_code = c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var(expr->ce_expr_left->ce_var,
                                                                           expr->ce_expr_right->ce_expr_left->ce_var, 0)),
                    c2bf_new_code_from_expr(
                      c2bf_new_expr_assign(c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                                               c2bf_new_expr_binary(ADD,
                                                                    c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                                                                    c2bf_new_expr_var(expr->ce_expr_right->ce_expr_right->ce_var, 0)))),
                    NULL
                  );
                }
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }

              case SUB:
              {
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr_left->type == VARIABLE);
                ASSERT(expr->ce_expr_right->ce_expr_right->type == VARIABLE);
                if (strcmp(expr->ce_expr_left->ce_var, expr->ce_expr_right->ce_expr_left->ce_var) == 0)
                { /* a = a - b --> tmp_b = b; while(tmp_b) { tmp_b--; a--; } */
                  new_code = c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_add", expr->ce_expr_right->ce_expr_right->ce_var, 0)),
                    c2bf_new_code_while(
                      c2bf_new_expr_var("__bf_tmp_add", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_add", 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_left->ce_var, 0)),
                        NULL
                      )
                    ),
                    NULL
                  );
                }
                else if (strcmp(expr->ce_expr_left->ce_var, expr->ce_expr_right->ce_expr_right->ce_var) == 0)
                { /* a = b - a --> tmp = a; a = b; while(tmp) { tmp--; a--; } */
                  new_code = c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_add", expr->ce_expr_left->ce_var, 0)),
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var(expr->ce_expr_left->ce_var,
                                                                           expr->ce_expr_right->ce_expr_left->ce_var, 0)),
                    c2bf_new_code_while(
                      c2bf_new_expr_var("__bf_tmp_add", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_add", 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_left->ce_var, 0)),
                        NULL
                      )
                    ),
                    NULL
                  );
                }
                else 
                { /* a = b - c --> a = b; a = a - c; } */
                  new_code = c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var(expr->ce_expr_left->ce_var,
                                                                           expr->ce_expr_right->ce_expr_left->ce_var, 0)),
                    c2bf_new_code_from_expr(
                      c2bf_new_expr_assign(c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                                               c2bf_new_expr_binary(SUB,
                                                                    c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                                                                    c2bf_new_expr_var(expr->ce_expr_right->ce_expr_right->ce_var, 0)))),
                    NULL
                  );
                }
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }

              case MUL:
              {
                /* a = b * c --> a = 0; tmp_b = b; while(tmp_b) { tmp_b--; tmp_c = c; while(tmp_c) { tmp_c--; a++; } } */
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr_left->type == VARIABLE);
                ASSERT(expr->ce_expr_right->ce_expr_right->type == VARIABLE);
                ASSERT(strcmp(expr->ce_expr_left->ce_var, expr->ce_expr_right->ce_expr_right->ce_var) != 0);
                new_code = c2bf_new_code_list(
                  c2bf_new_code_from_expr(c2bf_new_expr_assign_constant(expr->ce_expr_left->ce_var, 0, 0)),
                  c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_mul", expr->ce_expr_right->ce_expr_left->ce_var, 0)),
                  c2bf_new_code_while(
                    c2bf_new_expr_var("__bf_tmp_mul", 0),
                    c2bf_new_code_list(
                      c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_mul", 0)),
                      c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_mul2", expr->ce_expr_right->ce_expr_right->ce_var, 0)),
                      c2bf_new_code_while(
                        c2bf_new_expr_var("__bf_tmp_mul2", 0),
                        c2bf_new_code_list(
                          c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_mul2", 0)),
                          c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)),
                          NULL
                        )
                      ),
                      NULL
                    )
                  ),
                  NULL
                );
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }

              case DIV:
              case MOD:
              {  
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr_left->type == VARIABLE);
                ASSERT(expr->ce_expr_right->ce_expr_right->type == VARIABLE);
                ASSERT(strcmp(expr->ce_expr_left->ce_var, expr->ce_expr_right->ce_expr_right->ce_var) != 0);
                fprintf(stderr, "Warning : division ");
                c2bf_dump_c_expr(stderr, expr);
                fprintf(stderr, " works only if first argument is >= 0 and second arg > 0...\n");
                new_code = c2bf_ultra_simplify_div_or_mod_for_bf(expr);
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }

              case LESSER:
              case LESSER_EQUAL:
              { /* a = (b < c); -->  a = (b >= c); a = !a */
                /* a = (b <= c); -->  a = (b > c); a = !a */
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr_left->type == VARIABLE);
                ASSERT(expr->ce_expr_right->ce_expr_right->type == VARIABLE);
                new_code = c2bf_new_code_list(
                  c2bf_new_code_from_expr(
                   c2bf_new_expr_assign(
                    c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                    c2bf_new_expr_binary((expr->ce_expr_right->type == LESSER) ? GREATER_EQUAL : GREATER,
                                         c2bf_new_expr_var(expr->ce_expr_right->ce_expr_left->ce_var, 0),
                                         c2bf_new_expr_var(expr->ce_expr_right->ce_expr_right->ce_var, 0))
                   )
                  ),
                  c2bf_new_code_from_expr(
                   c2bf_new_expr_assign(
                    c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                    c2bf_new_expr_unary(UNARY_LNOT, c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0))
                   )
                  ),
                  NULL
                );
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }


              case GREATER:
              case GREATER_EQUAL:
              { 
                CCode* new_code;
                fprintf(stderr, "Warning : comparison ");
                c2bf_dump_c_expr(stderr, expr);
                fprintf(stderr, " works only if both arguments are >= 0...\n");
                new_code = c2bf_ultra_simplify_greater_or_greater_equal_for_bf(expr);
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }
              
              case UNARY_LNOT:
              { /* a = !b --> tmp = b; a = 1; while(tmp) { tmp=0; a--; } */
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr->type == VARIABLE);
                new_code = 
                  c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_lnot", expr->ce_expr_right->ce_expr->ce_var, 0)),
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_constant(expr->ce_expr_left->ce_var, 1, 0)),
                    c2bf_new_code_while(
                      c2bf_new_expr_var("__bf_tmp_lnot", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_lnot", 0, 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_left->ce_var, 0)),
                        NULL
                      )
                    ),
                    NULL
                  );
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }
              
              case EQUAL:
              { /* c = (a == b) -->
                   tmp_equal = a;
                   tmp_equal2 = b;
                   c = 1;
                   tmp_equal3 = tmp_equal2;
                   tmp_equal4++;
                   while(tmp_equal3)
                   {
                     tmp_equal3 = 0;
                     tmp_equal4--;
                   }
                   while(tmp_equal4)
                   {
                     tmp_equal4--;
                     tmp_equal2 = 0;
                     tmp_equal3 = tmp_equal;
                     tmp_equal = 0;
                     while(tmp_equal3)
                     {
                       tmp_equal3 = 0;
                       c--;
                     }
                   }
                   while(tmp_equal) { tmp_equal--; tmp_equal2--;}
                   while(tmp_equal2) { tmp_equal2 = 0; c--;}
                */
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr_left->type == VARIABLE);
                ASSERT(expr->ce_expr_right->ce_expr_right->type == VARIABLE);
                fprintf(stderr, "Warning : comparison ");
                c2bf_dump_c_expr(stderr, expr);
                fprintf(stderr, " works only if both arguments are >= 0...\n");
                new_code = 
                  c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_equal", expr->ce_expr_right->ce_expr_left->ce_var, 0)),
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_equal2", expr->ce_expr_right->ce_expr_right->ce_var, 0)),
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_constant(expr->ce_expr_left->ce_var, 1, 0)),
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_equal3", "__bf_tmp_equal2", 0)),
                    c2bf_new_code_from_expr(c2bf_new_expr_increment("__bf_tmp_equal4", 0)),
                    c2bf_new_code_while(c2bf_new_expr_var("__bf_tmp_equal3", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_equal3", 0, 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_equal4", 0)),
                        NULL
                      )
                    ),
                    c2bf_new_code_while(c2bf_new_expr_var("__bf_tmp_equal4", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_equal4", 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_equal2", 0, 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_equal3", "__bf_tmp_equal", 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_equal", 0, 0)),
                        c2bf_new_code_while(c2bf_new_expr_var("__bf_tmp_equal3", 0),
                          c2bf_new_code_list(
                            c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_equal3", 0, 0)),
                            c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_left->ce_var, 0)),
                            NULL
                          )
                        ),
                        NULL
                      )
                    ),
                    c2bf_new_code_while(c2bf_new_expr_var("__bf_tmp_equal", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_equal", 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_equal2", 0)),
                        NULL
                      )
                    ),
                    c2bf_new_code_while(c2bf_new_expr_var("__bf_tmp_equal2", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_equal2", 0, 0)),
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_left->ce_var, 0)),
                        NULL
                      )
                    ),
                    NULL
                  );
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }

              case SHR:
              case SHL:
              {
                /* a = b >> c;  --> 
                   tmp_shr = c;
                   tmp_shr2 = 1;
                   while(tmp_shr)
                   {
                     tmp_shr--;
                     tmp_shr2 = tmp_shr2 + tmp_shr2;
                   }
                   a = b / tmp_shr2;
                */
                CCode* new_code;
                new_code =
                  c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_shrl", expr->ce_expr_right->ce_expr_right->ce_var, 0)),
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_shrl2", 1, 0)),
                    c2bf_new_code_while(
                      c2bf_new_expr_var("__bf_tmp_shrl", 0),
                      c2bf_new_code_list(
                        c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_shrl", 0)),
                        c2bf_new_code_from_expr(
                          c2bf_new_expr_assign(
                            c2bf_new_expr_var("__bf_tmp_shrl2", 0),
                            c2bf_new_expr_binary(ADD, c2bf_new_expr_var("__bf_tmp_shrl2", 0), c2bf_new_expr_var("__bf_tmp_shrl2", 0))
                          )
                        ),
                        NULL
                      )
                    ),
                    c2bf_new_code_from_expr(
                      c2bf_new_expr_assign(
                        c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                        c2bf_new_expr_binary((expr->ce_expr_right->type == SHR) ? DIV : MUL,
                                             c2bf_new_expr_var(expr->ce_expr_right->ce_expr_left->ce_var, 0),
                                             c2bf_new_expr_var("__bf_tmp_shrl2", 0)
                        )
                      )
                    ),
                    NULL
                );
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;

              }
              
              case VARIABLE:
              { /* a = b --> a=0; tmp=0; while(b) { b--; a++; tmp++} while(tmp) { tmp--; b++; } */
                CCode* new_code;
                new_code =
                  c2bf_new_code_list(
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_constant(expr->ce_expr_left->ce_var, 0, 0)),
                    c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_affect", 0, 0)),
                    c2bf_new_code_while(c2bf_new_expr_var(expr->ce_expr_right->ce_var, 0),
                                        c2bf_new_code_list(
                                          c2bf_new_code_from_expr(c2bf_new_expr_decrement(expr->ce_expr_right->ce_var, 0)),
                                          c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_left->ce_var, 0)),
                                          c2bf_new_code_from_expr(c2bf_new_expr_increment("__bf_tmp_affect", 0)),
                                          NULL)
                                      ),
                    c2bf_new_code_while(c2bf_new_expr_var("__bf_tmp_affect", 0),
                                        c2bf_new_code_list(
                                          c2bf_new_code_from_expr(c2bf_new_expr_decrement("__bf_tmp_affect", 0)),
                                          c2bf_new_code_from_expr(c2bf_new_expr_increment(expr->ce_expr_right->ce_var, 0)),
                                        NULL)
                                      ),
                    NULL
                );
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }

              case UNARY_NEG:
              { /* a = -b --> tmp = 0; a = tmp - b; */
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr->type == VARIABLE);
                new_code = c2bf_new_code_list(
                  c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_neg", 0, 0)),
                  c2bf_new_code_from_expr(
                    c2bf_new_expr_assign(
                      c2bf_new_expr_var(expr->ce_expr_left->ce_var, 0),
                      c2bf_new_expr_binary(SUB, c2bf_new_expr_var("__bf_tmp_neg", 0),
                                           c2bf_new_expr_var(expr->ce_expr_right->ce_expr->ce_var, 0))
                    )
                  ),
                  NULL
                );
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }

              case UNARY_PLUS:
              { /* a = +b --> a = b; */
                CCode* new_code;
                ASSERT(expr->ce_expr_right->ce_expr->type == VARIABLE);
                new_code = c2bf_new_code_list(
                  c2bf_new_code_from_expr(
                    c2bf_new_expr_assign_var(expr->ce_expr_left->ce_var, expr->ce_expr_right->ce_expr->ce_var, 0)
                  ),
                  NULL
                );
                c2bf_ultra_simplify_for_bf(new_code);
                c2bf_free_c_code(subCode);
                ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                break;
              }

              case FUNCTION_CALL:
                break;
              
              default:
                c2bf_dump_c_code(stderr, subCode, -1);
                break;
            }
            break;
          }
            
          default:
          {
            int useless;
            int i;
            if (c2bf_is_operator(expr->type))
            {
              CONST COperator* coperator = c2bf_find_operator_of_expr_type(expr->type);
              useless = 1;
              for(i=0;i<coperator->n_ary;i++)
              {
                if (expr->ce_expr_tab[i]->type != VARIABLE)
                  useless = 0;
              }
            }
            else
            {
              if (expr->type == FUNCTION_CALL)
              {
                if (strcmp(expr->ce_function_call->function_name, "printf") == 0)
                {
                  CExpr* arg = (CExpr*)expr->ce_function_call->argument_list->data;
                  List* next_arg = expr->ce_function_call->argument_list->next;
                  if (arg->type == VARIABLE && arg->ce_var[0] == '\"')
                  {
                    CCode* new_code;
                    unsigned int i;
                    new_code = c2bf_new_code_list(NULL);
                    for(i=1;i<strlen(arg->ce_var)-1;i++)
                    {
                      char c;
                      if (arg->ce_var[i] == '%')
                      {
                        int j;
                        CExpr* next_arg_expr;
                        ASSERT(next_arg != NULL);
                        i++;
                        next_arg_expr = (CExpr*)next_arg->data;
                        next_arg = next_arg->next;
                        ASSERT(next_arg_expr->type == VARIABLE);
                        if (arg->ce_var[i] == 'd')
                        {
                          new_code->c_list = list_append(new_code->c_list,
                            c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_printf_do_putchar", 0, 0))
                          );
                          new_code->c_list = list_append(new_code->c_list,
                            c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_printf_d", next_arg_expr->ce_var, 0))
                          );
                          new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_printf_d2", 1, 0))
                          );
                          new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_printf_d3", 10, 0))
                          );
                          for(j=0;j<4;j++)
                          {
                            new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_printf_d4", "__bf_tmp_printf_d2", 0))
                            );  
                            new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign(
                                c2bf_new_expr_var("__bf_tmp_printf_d2", 0),
                                c2bf_new_expr_binary(MUL, c2bf_new_expr_var("__bf_tmp_printf_d4", 0),
                                                          c2bf_new_expr_var("__bf_tmp_printf_d3", 0)))
                              )
                            );
                          }
                          for(j=0;j<5;j++)
                          {
                            CCode* sub_block = c2bf_new_code_list(NULL);
                            
                            new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign(
                                c2bf_new_expr_var("__bf_tmp_putchar", 0),
                                c2bf_new_expr_binary(DIV, c2bf_new_expr_var("__bf_tmp_printf_d", 0),
                                                          c2bf_new_expr_var("__bf_tmp_printf_d2", 0)))
                              )
                            );
                            
                            sub_block->c_list = list_append(sub_block->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_printf_d4", '0', 0))
                            );
                            
                            sub_block->c_list = list_append(sub_block->c_list,
                                c2bf_new_code_from_expr(c2bf_new_expr_assign(
                                    c2bf_new_expr_var("__bf_tmp_putchar", 0),
                                c2bf_new_expr_binary(ADD, c2bf_new_expr_var("__bf_tmp_putchar", 0),
                                    c2bf_new_expr_var("__bf_tmp_printf_d4", 0)))
                                                       )
                            );
                            sub_block->c_list = list_append(sub_block->c_list,
                                c2bf_new_code_from_expr(c2bf_new_expr_function_call("putchar", c2bf_new_expr_var("__bf_tmp_putchar", 0) ,NULL))
                            );

                            
                            if (j < 4)
                            {
                              CCode* while_code;
                              new_code->c_list = list_append(new_code->c_list,
                                  c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_putchar_tmp",
                                                                                   "__bf_tmp_putchar", 0)));
                              while_code = alloc0(CCode, 1);
                              while_code->type = WHILE;
                              while_code->c_while = alloc0(CWhile, 1);
                              while_code->c_while->cond = c2bf_new_expr_var("__bf_tmp_putchar_tmp", 0);
                              while_code->c_while->block = c2bf_new_code_list(
                                c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_printf_do_putchar", 1, 0)),
                                c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_putchar_tmp", 0, 0)),
                                NULL);
                              new_code->c_list = list_append(new_code->c_list, while_code);
                              new_code->c_list = list_append(new_code->c_list,
                                  c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_putchar_tmp",
                                                                                   "__bf_tmp_printf_do_putchar", 0)));
                              
                              while_code = alloc0(CCode, 1);
                              while_code->type = WHILE;
                              while_code->c_while = alloc0(CWhile, 1);
                              while_code->c_while->cond = c2bf_new_expr_var("__bf_tmp_putchar_tmp", 0);
                              while_code->c_while->block = sub_block;
                              sub_block->c_list = list_append(sub_block->c_list,
                                  c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_putchar_tmp", 0, 0)));
                              new_code->c_list = list_append(new_code->c_list, while_code);
                            }
                            else
                            {
                              new_code->c_list = list_append(new_code->c_list, sub_block);
                            }
                            
                            new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_printf_d4", "__bf_tmp_printf_d", 0))
                            );  
                            new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign(
                                c2bf_new_expr_var("__bf_tmp_printf_d", 0),
                                c2bf_new_expr_binary(MOD, c2bf_new_expr_var("__bf_tmp_printf_d4", 0),
                                                          c2bf_new_expr_var("__bf_tmp_printf_d2", 0)))
                              )
                            );
                            new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_printf_d4", "__bf_tmp_printf_d2", 0))
                            );  
                            new_code->c_list = list_append(new_code->c_list,
                              c2bf_new_code_from_expr(c2bf_new_expr_assign(
                                c2bf_new_expr_var("__bf_tmp_printf_d2", 0),
                                c2bf_new_expr_binary(DIV, c2bf_new_expr_var("__bf_tmp_printf_d4", 0),
                                                          c2bf_new_expr_var("__bf_tmp_printf_d3", 0)))
                              )
                            );
                          }
                        }
                        else if (arg->ce_var[i] == 'c')
                        {
                          new_code->c_list = list_append(new_code->c_list,
                            c2bf_new_code_from_expr(c2bf_new_expr_assign_var("__bf_tmp_putchar", next_arg_expr->ce_var, 0))
                          );
                          new_code->c_list = list_append(new_code->c_list,
                            c2bf_new_code_from_expr(c2bf_new_expr_function_call("putchar", c2bf_new_expr_var("__bf_tmp_putchar", 0) ,NULL))
                          );
                        }
                        else
                        {
                          NOT_IMPLEMENTED();
                        }
                      }
                      else
                      {
                        if (arg->ce_var[i] == '\\')
                        {
                          switch (arg->ce_var[i+1])
                          {
                            case 't': c = '\t'; break;
                            case 'n': c = '\n'; break;
                            case '\\': c = '\\'; break;
                            default: NOT_IMPLEMENTED();
                          }
                          i++;
                        }
                        else
                          c = arg->ce_var[i];
                        new_code->c_list = list_append(new_code->c_list,
                          c2bf_new_code_from_expr(c2bf_new_expr_assign_constant("__bf_tmp_putchar", c, 0))
                        );
                        new_code->c_list = list_append(new_code->c_list,
                          c2bf_new_code_from_expr(c2bf_new_expr_function_call("putchar", c2bf_new_expr_var("__bf_tmp_putchar", 0) ,NULL))
                        );
                      }
                    }
                    c2bf_ultra_simplify_for_bf(new_code);
                    c2bf_free_c_code(subCode);
                    ccode->c_list = c2bf_ultra_simplify_for_bf_merge_list(ccode->c_list, cur, new_code);
                    cur = next;
                    continue;
                  }
                }
                else if (strcmp(expr->ce_function_call->function_name, "putchar") == 0)
                {
                  if (list_get_size(expr->ce_function_call->argument_list) == 1)
                  {
                    CExpr* arg = (CExpr*)expr->ce_function_call->argument_list->data;
                    if (arg->type == VARIABLE)
                    {
                      cur = next;
                      continue;
                    }
                  }
                }
              }
              useless = 0;
            }
            if (!useless)
            {
              c2bf_dump_c_code(stderr, subCode, -1);
            }
            else
            {
              c2bf_free_c_code(subCode);
              ccode->c_list = list_free_elt_and_update_head(ccode->c_list, cur, NULL);
              break;
            }
            break;
          }
        }
        
        break;
      }
      
      case WHILE:
      {
        ASSERT(subCode->c_while->cond->type == VARIABLE);
        ASSERT(subCode->c_while->block->type == LIST);
        c2bf_ultra_simplify_for_bf(subCode->c_while->block);
        break;
      }
      
      case NOTHING_STATEMENT:
      {
        c2bf_free_c_code((CCode*)cur->data);
        ccode->c_list = list_free_elt_and_update_head(ccode->c_list, cur, NULL);
        break;
      }

      case DECLARATION:
        break;
      
      default:
        c2bf_dump_c_code(stderr, subCode, -1);
        SHOULDNT_HAPPEN();
        break;
    }
    
    cur = next;
  }
}

typedef struct
{
  char* ori;
  char* subst;
} SubstVar;

static int c2bf_exists_already_var_in_subst_list(CONST List* list, CONST char* varName)
{
  while(list)
  {
    SubstVar* substitution = (SubstVar*)list->data;
    if (strcmp(substitution->ori, varName) == 0) return TRUE;
    list = list->next;
  }
  return FALSE;
}

static void c2bf_rename_ambiguous_var_name_expr(CExpr* expr, List* subst_var)
{
  switch (expr->type)
  {
    case FUNCTION_CALL:
    {
      List* cur = expr->ce_function_call->argument_list;
      while(cur)
      {
        c2bf_rename_ambiguous_var_name_expr((CExpr*)cur->data, subst_var);
        cur = cur->next;
      }
      break;
    }
      
    case VARIABLE:
    {
      while(subst_var)
      {
        SubstVar* substVar = (SubstVar*)subst_var->data;
        if (strcmp(substVar->ori, expr->ce_var) == 0)
        {
          free(expr->ce_var);
          expr->ce_var = strdup(substVar->subst);
          break;
        }
        subst_var = subst_var->next;
      }
      break;
    }
    
    case CHARACTER:
    case CONSTANT:
      break;
    
    default:
    {
      CONST COperator* coperator = c2bf_find_operator_of_expr_type(expr->type);
      int i;
      for(i=0;i<coperator->n_ary;i++)
        c2bf_rename_ambiguous_var_name_expr(expr->ce_expr_tab[i], subst_var);
    }
  }
}

static char* c2bf_find_new_var_name_in_subst_list(CONST List* list, CONST char* radixVarName)
{
  char tmp[512];
  int i = 0;
  int ok;
  while(1)
  {
    CONST List* cur = list;
    ok = TRUE;
    sprintf(tmp, "%s_%d", radixVarName, i);
    while(cur)
    {
      SubstVar* substitution = (SubstVar*)cur->data;
      if (strcmp(substitution->subst, tmp) == 0)
      {
        ok = FALSE;
        break;
      }
      cur = cur->next;
    }
    if (ok)
    {
      return strdup(tmp);
    }
    i++;
  }
}

static void c2bf_rename_ambiguous_var_name_code(CCode* ccode, List* subst_var)
{
  switch (ccode->type)
  {
    case LIST:
    {
      List* cur = ccode->c_list;
      List* subst_var_local = NULL;
      while(cur)
      {
        CCode* subCode = (CCode*)cur->data;
        if (subCode->type == DECLARATION)
        {
          char* varName = subCode->c_decl->var;
          SubstVar* substitution = alloc0(SubstVar, 1);
          substitution->ori = strdup(varName);
          if (c2bf_exists_already_var_in_subst_list(subst_var, varName))
          {
            substitution->subst = c2bf_find_new_var_name_in_subst_list(subst_var, varName);
            free(subCode->c_decl->var);
            subCode->c_decl->var = strdup(substitution->subst);
          }
          else
          {
            substitution->subst = strdup(varName);
          }
          subst_var = list_prepend(subst_var, substitution);
          subst_var_local = list_prepend(subst_var_local, subst_var);
          
        }
        else
        {
          c2bf_rename_ambiguous_var_name_code(subCode, subst_var);
        }
        cur = cur->next;
      }
      while (subst_var_local)
      {
        List* link = (List*)subst_var_local->data;
        SubstVar* substitution = (SubstVar*)link->data;
        List* next = subst_var_local->next;
        free(substitution->ori);
        free(substitution->subst);
        free(substitution);
        subst_var = list_free_elt_and_update_head(subst_var, link, NULL);
        free(subst_var_local);
        subst_var_local = next;
      }
      break;
    }
      
    case DECLARATION:
    {
      SHOULDNT_HAPPEN();
      break;
    }
      
    case EXPRESSION:
    {
      c2bf_rename_ambiguous_var_name_expr(ccode->c_expr, subst_var);
      break;
    }
      
    case WHILE:
    {
      c2bf_rename_ambiguous_var_name_expr(ccode->c_while->cond, subst_var);
      c2bf_rename_ambiguous_var_name_code(ccode->c_while->block, subst_var);
      break;
    }
      
    case DO_WHILE:
    {
      c2bf_rename_ambiguous_var_name_expr(ccode->c_do_while->cond, subst_var);
      c2bf_rename_ambiguous_var_name_code(ccode->c_do_while->block, subst_var);
      break;
    }
      
    case IF:
    {
      c2bf_rename_ambiguous_var_name_expr(ccode->c_if->cond, subst_var);
      c2bf_rename_ambiguous_var_name_code(ccode->c_if->block, subst_var);
      if (ccode->c_if->else_block)
        c2bf_rename_ambiguous_var_name_code(ccode->c_if->else_block, subst_var);
      break;
    }
      
    case FOR:
    {
      c2bf_rename_ambiguous_var_name_expr(ccode->c_for->cond, subst_var);
      c2bf_rename_ambiguous_var_name_code(ccode->c_for->block, subst_var);
      c2bf_rename_ambiguous_var_name_code(ccode->c_for->init_block, subst_var);
      c2bf_rename_ambiguous_var_name_code(ccode->c_for->update_block, subst_var);
      break;
    }
      
    case BREAK:
    case CONTINUE:
    case NOTHING_STATEMENT:
      break;
      
    default:
      SHOULDNT_HAPPEN();
  }  
}

static void c2bf_rename_ambiguous_var_name(CCode* ccode)
{
  c2bf_rename_ambiguous_var_name_code(ccode, NULL);
}

static int c2bf_exists_already_var_in_var_list(CONST List* list, CONST char* varName)
{
  while(list)
  {
    if (strcmp((char*)list->data, varName) == 0) return TRUE;
    list = list->next;
  }
  return FALSE;
}


static List* c2bf_collect_and_remove_decls_code(CCode* ccode, List* listDecls)
{
  switch (ccode->type)
  {
    case LIST:
    {
      List* cur = ccode->c_list;
      while(cur)
      {
        CCode* subCode = (CCode*)cur->data;
        if (subCode->type == DECLARATION)
        {
          List* nextSubCode = cur->next;
          if (!c2bf_exists_already_var_in_var_list(listDecls, subCode->c_decl->var))
          {
            listDecls = list_prepend(listDecls, strdup(subCode->c_decl->var));
          }
          c2bf_free_c_code(subCode);
          ccode->c_list = list_free_elt_and_update_head(ccode->c_list, cur, NULL);
          cur = nextSubCode;
          continue;
        }
        else
        {
          listDecls = c2bf_collect_and_remove_decls_code(subCode, listDecls);
        }
        cur = cur->next;
      }
      break;
    }
      
    case DECLARATION:
    {
      SHOULDNT_HAPPEN();
      break;
    }
      
    case EXPRESSION:
    {
      break;
    }
      
    case WHILE:
    {
      listDecls = c2bf_collect_and_remove_decls_code(ccode->c_while->block, listDecls);
      break;
    }
      
    case DO_WHILE:
    {
      listDecls = c2bf_collect_and_remove_decls_code(ccode->c_do_while->block, listDecls);
      break;
    }
      
    case IF:
    {
      listDecls = c2bf_collect_and_remove_decls_code(ccode->c_if->block, listDecls);
      if (ccode->c_if->else_block)
        listDecls = c2bf_collect_and_remove_decls_code(ccode->c_if->else_block, listDecls);
      break;
    }
      
    case FOR:
    {
      listDecls = c2bf_collect_and_remove_decls_code(ccode->c_for->block, listDecls);
      listDecls = c2bf_collect_and_remove_decls_code(ccode->c_for->init_block, listDecls);
      listDecls = c2bf_collect_and_remove_decls_code(ccode->c_for->update_block, listDecls);
      break;
    }
      
    case BREAK:
    case CONTINUE:
    case NOTHING_STATEMENT:
      break;
      
    default:
      SHOULDNT_HAPPEN();
  }  
  return listDecls;
}


static void c2bf_prepend_decls_list(CCode* ccode, CONST List* list)
{
  ASSERT(ccode->type == LIST);
  while(list)
  {
    char* varName = (char*)list->data;
    CCode* subCode = alloc0(CCode, 1);
    subCode->type = DECLARATION;
    subCode->c_decl = alloc0(CDeclaration, 1);
    subCode->c_decl->var = strdup(varName);
    ccode->c_list = list_prepend(ccode->c_list, subCode);
    list = list->next;
  }
}

static void c2bf_free_decls_list(List* list)
{
  list_free(list, free);
}

static void c2bf_regroup_decls(CCode* ccode)
{
  List* list = c2bf_collect_and_remove_decls_code(ccode, NULL);
  c2bf_prepend_decls_list(ccode, list);
  c2bf_free_decls_list(list);
}

static int c2bf_ultra_simplified_code_to_bf_get_var_indice(CONST List* list_var, CONST char* varName)
{
  int rank = 0;
  while(list_var)
  {
    if (strcmp((char*)list_var->data, varName) == 0)
      return rank;
    rank++;
    list_var = list_var->next;
  }
  SHOULDNT_HAPPEN();
  return -1;
}

static void c2bf_ultra_simplified_code_to_bf_print_displacements(int rank, int prevRank)
{
  int i;
  if (rank > prevRank)
  {
    for(i=0;i<rank-prevRank;i++) printf(">");
  }
  else
  {
    for(i=0;i<prevRank-rank;i++) printf("<");
  }
}

static int c2bf_ultra_simplified_code_to_bf_internal(CONST List* cur, CONST List* list_var, int prevRank)
{
  int rank = 0;
  while(cur)
  {
    CCode* subCode = (CCode*)cur->data;
    switch(subCode->type)
    {
      case EXPRESSION:
      {
        CExpr* expr = subCode->c_expr;
        switch(expr->type)
        {
          case UNARY_PRE_INCREMENT:
          case UNARY_POST_INCREMENT:
          {
            ASSERT(expr->ce_expr->type == VARIABLE);
            rank = c2bf_ultra_simplified_code_to_bf_get_var_indice(list_var, expr->ce_expr->ce_var);
            c2bf_ultra_simplified_code_to_bf_print_displacements(rank, prevRank);
            printf("+");
            prevRank = rank;
            break;
          }

          case UNARY_PRE_DECREMENT:
          case UNARY_POST_DECREMENT:
          {
            ASSERT(expr->ce_expr->type == VARIABLE);
            rank = c2bf_ultra_simplified_code_to_bf_get_var_indice(list_var, expr->ce_expr->ce_var);
            c2bf_ultra_simplified_code_to_bf_print_displacements(rank, prevRank);
            printf("-");
            prevRank = rank;
            break;
          }
          
          case ASSIGNMENT:
          {
            ASSERT(expr->ce_expr_left->type == VARIABLE);
            ASSERT(expr->ce_expr_right->type == FUNCTION_CALL);
            ASSERT(strcmp(expr->ce_expr_right->ce_function_call->function_name, "getchar") == 0 &&
                   list_get_size(expr->ce_expr_right->ce_function_call->argument_list) == 0);
            rank = c2bf_ultra_simplified_code_to_bf_get_var_indice(list_var, expr->ce_expr_left->ce_var);
            c2bf_ultra_simplified_code_to_bf_print_displacements(rank, prevRank);
            printf(",");
            prevRank = rank;
            break;
          }

          case FUNCTION_CALL:
          {
            CExpr* arg;
            ASSERT(strcmp(expr->ce_function_call->function_name, "putchar") == 0 &&
                   list_get_size(expr->ce_function_call->argument_list) == 1);
            arg = (CExpr*)expr->ce_function_call->argument_list->data;
            ASSERT(arg->type == VARIABLE);
            rank = c2bf_ultra_simplified_code_to_bf_get_var_indice(list_var, arg->ce_var);
            c2bf_ultra_simplified_code_to_bf_print_displacements(rank, prevRank);
            printf(".");
            prevRank = rank;
            break;
          }

          default:
            NOT_IMPLEMENTED();
        }
        break;
      }

      case WHILE:
      {
        ASSERT(subCode->c_while->cond->type == VARIABLE);
        rank = c2bf_ultra_simplified_code_to_bf_get_var_indice(list_var, subCode->c_while->cond->ce_var);
        c2bf_ultra_simplified_code_to_bf_print_displacements(rank, prevRank);
        prevRank = rank;
        printf("[");
        ASSERT(subCode->c_while->block->type == LIST);
        prevRank = c2bf_ultra_simplified_code_to_bf_internal(subCode->c_while->block->c_list, list_var, rank);
        rank = c2bf_ultra_simplified_code_to_bf_get_var_indice(list_var, subCode->c_while->cond->ce_var);
        c2bf_ultra_simplified_code_to_bf_print_displacements(rank, prevRank);
        prevRank = rank;
        printf("]");
        break;
      }
      
      default:
        c2bf_dump_c_code(stderr, subCode, -1);
        SHOULDNT_HAPPEN();
        break;
    }

    cur = cur->next;
  }

  return rank;
}

static void c2bf_ultra_simplified_code_to_bf(CONST CCode* ccode)
{
  List* cur;
  List* list_var = NULL;
  ASSERT(ccode->type == LIST);
  cur = ccode->c_list;
  while(cur)
  {
    CCode* subCode = (CCode*)cur->data;
    if (subCode->type == DECLARATION)
    {
      list_var = list_append(list_var, subCode->c_decl->var);
    }
    else
    {
      break;
    }
    cur = cur->next;
  }

  if (cur)
    c2bf_ultra_simplified_code_to_bf_internal(cur->next, list_var, 0);

  printf("\n");
 
  list_free(list_var, NULL);
}

void libbf_c_to_bf(CONST char* code)
{
  CCode* code_simplified;
  List* tokenList = c2bf_create_token_list(code);
  CCode* ccode = c2bf_build_c_code_main(tokenList);
  
  c2bf_free_token_list(tokenList);
  
  c2bf_rename_ambiguous_var_name(ccode);
  
  c2bf_regroup_decls(ccode);
  
  code_simplified = c2bf_simplify_code(ccode);
  
  c2bf_ultra_simplify_for_bf(code_simplified);
  
  /*c2bf_dump_c_code(stdout, code_simplified, 0); */

  c2bf_ultra_simplified_code_to_bf(code_simplified);

  c2bf_free_c_code(code_simplified);
  
  c2bf_free_c_code(ccode);
}
