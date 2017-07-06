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
#include <assert.h>

#include "libbf.h"
#include "libbf_list.h"
#include "libbf_internal.h"
#include "libbf_hash.h"
#include "libbf_interpreter_dynalloc.h"

#ifndef NO_COMPILER
#include "libbf_compiler.h"
#endif

typedef enum
{
  BF_SIMPLE_END,
  BF_SIMPLE_PLUS,
  BF_SIMPLE_MINUS,
  BF_SIMPLE_GREATER,
  BF_SIMPLE_LESSER,
  BF_SIMPLE_DOT,
  BF_SIMPLE_COMMA,
  BF_SIMPLE_LBRACKET,
  BF_SIMPLE_RBRACKET,
} InterpreterInstr;

typedef struct _InterpreterUnit InterpreterUnit;

struct _InterpreterUnit
{
  InterpreterInstr  instr;
  InterpreterUnit*  jump_to;
};

static InterpreterUnit* libbf_gen_interpreted_code(const char* code)
{
  InterpreterUnit* new_code = alloc0(InterpreterUnit, strlen(code)+1);
  const char* code_ptr = code;
  InterpreterUnit* new_code_ptr = new_code;
  InterpreterUnit* stack[32768];
  InterpreterUnit** p_stack = stack;

  while(*code_ptr != 0)
  {
    switch(*(code_ptr++))
    {
      case '+': new_code_ptr->instr = BF_SIMPLE_PLUS;    new_code_ptr++; break;
      case '-': new_code_ptr->instr = BF_SIMPLE_MINUS;   new_code_ptr++; break;
      case '>': new_code_ptr->instr = BF_SIMPLE_GREATER; new_code_ptr++; break;
      case '<': new_code_ptr->instr = BF_SIMPLE_LESSER;  new_code_ptr++; break;
      case '.': new_code_ptr->instr = BF_SIMPLE_DOT;     new_code_ptr++; break;
      case ',': new_code_ptr->instr = BF_SIMPLE_COMMA;   new_code_ptr++; break;
      case '[': new_code_ptr->instr = BF_SIMPLE_LBRACKET;
                *p_stack = new_code_ptr;
                p_stack++;
                new_code_ptr++;
                break;
      case ']': new_code_ptr->instr = BF_SIMPLE_RBRACKET;
                --p_stack;
                if (p_stack < stack)
                {
                  long offset = code_ptr - code;
                  fatal("Unbalanced ] at offset %ld\n", offset);
                }
                new_code_ptr->jump_to = *p_stack;
                (*p_stack)->jump_to = new_code_ptr;
                new_code_ptr++;
                break;
      default: break;
    }
  }
  new_code_ptr->instr = BF_SIMPLE_END;

  return new_code;
}

/* -------------------------------------------------------- */
/* Expression management */
/* -------------------------------------------------------- */

#define IS_SIMPLE_EXPR(expr)  ((expr)->e_type == EXPR_CONSTANT || (expr)->e_type == EXPR_DATA)

static void expr_dump(FILE* f, const char* tab_begin, const char* tab_end, Expression* expr);
static void expr_dump_helper(FILE* f, const char* tab_begin, const char* tab_end, Expression* expr)
{
  if (IS_SIMPLE_EXPR(expr))
  {
    expr_dump(f, tab_begin, tab_end, expr);
  }
  else
  {
    fprintf(f, "(");
    expr_dump(f, tab_begin, tab_end, expr);
    fprintf(f, ")");
  }
}

static void expr_dump(FILE* f, const char* tab_begin, const char* tab_end, Expression* expr)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      fprintf(f, "%ld", (long)expr->e_constant);
      break;
      
    case EXPR_DATA:
      fprintf(f, "%s%ld%s", tab_begin, (long)expr->e_offset, tab_end);
      break;
      
    case EXPR_ADD:
      /* expr_dump_helper(f, expr->e_expr1); */
      fprintf(f, "(");
      expr_dump(f, tab_begin, tab_end, expr->e_expr1);
      fprintf(f, " + ");
      /* expr_dump_helper(f, expr->e_expr2); */
      expr_dump(f, tab_begin, tab_end, expr->e_expr2);
      fprintf(f, ")");
      break;
      
    case EXPR_MUL:
      expr_dump_helper(f, tab_begin, tab_end, expr->e_expr1);
      fprintf(f, " * ");
      expr_dump_helper(f, tab_begin, tab_end, expr->e_expr2);
      break;

    default:
      SHOULDNT_HAPPEN();
      break;
  }
}

static Expression* expr_constant_new(CONSTANT_t constant)
{
  Expression* expr = alloc0(Expression, 1);
  expr->e_type = EXPR_CONSTANT;
  expr->e_constant  = constant;
  return expr;
}


static Expression* expr_data_new(OFFSET_t offset)
{
  Expression* expr = alloc0(Expression, 1);
  expr->e_type   = EXPR_DATA;
  expr->e_offset = offset;
  return expr;
}

static Expression* expr_add_new(Expression* expr1, Expression* expr2)
{
  Expression* expr = alloc0(Expression, 1);
  expr->e_type  = EXPR_ADD;
  expr->e_expr1 = expr1;
  expr->e_expr2 = expr2;
  return expr;
}

static Expression* expr_mul_new(Expression* expr1, Expression* expr2)
{
  Expression* expr = alloc0(Expression, 1);
  expr->e_type  = EXPR_MUL;
  expr->e_expr1 = expr1;
  expr->e_expr2 = expr2;
  return expr;
}

static void expr_free(Expression* expr)
{
  switch(expr->e_type)
  {
    case EXPR_ADD:
    case EXPR_MUL:
      expr_free(expr->e_expr1);
      expr_free(expr->e_expr2);
      /* fallback through */
      
    case EXPR_CONSTANT:
    case EXPR_DATA:
      expr->e_type = EXPR_INVALID; /* help detecting memory corruption */
      free(expr);
      break;
      
    default:
      SHOULDNT_HAPPEN();
      break;
  }
}

static Expression* expr_dup(Expression* expr)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      return expr_constant_new(expr->e_constant);
      break;
      
    case EXPR_DATA:
      return expr_data_new(expr->e_offset);
      break;
    
    case EXPR_ADD:
      return expr_add_new(expr_dup(expr->e_expr1), expr_dup(expr->e_expr2));
      break;
      
    case EXPR_MUL:
      return expr_mul_new(expr_dup(expr->e_expr1), expr_dup(expr->e_expr2));
      break;
      
    default:
      SHOULDNT_HAPPEN();
      break;
  }
  
  return NULL;
}

static int expr_ref_only_offset(Expression* expr, OFFSET_t offset)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      return TRUE;
      break;
      
    case EXPR_DATA:
      return expr->e_offset == offset;
      break;
      
    case EXPR_ADD:
    case EXPR_MUL:
      return expr_ref_only_offset(expr->e_expr1, offset) && expr_ref_only_offset(expr->e_expr2, offset);
      break;

    default:
      SHOULDNT_HAPPEN();
      return FALSE;
      break;
  }
}

static int expr_ref_offset(Expression* expr, OFFSET_t offset)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      return FALSE;
      break;
      
    case EXPR_DATA:
      return expr->e_offset == offset;
      break;
      
    case EXPR_ADD:
    case EXPR_MUL:
      return expr_ref_offset(expr->e_expr1, offset) || expr_ref_offset(expr->e_expr2, offset);
      break;

    default:
      SHOULDNT_HAPPEN();
      return FALSE;
      break;
  }
}

static int expr_ref_offset_and_only_offset(Expression* expr, OFFSET_t offset)
{
  return expr_ref_offset(expr, offset) && expr_ref_only_offset(expr, offset);
}

#define OFFSET_TO_POINTER(x) (void*)(long)(x)
#define POINTER_TO_OFFSET(x) (OFFSET_t)(long)(x)

static List* expr_get_list_of_ref_internal_but_ignore_offset(Expression* expr, List* list, OFFSET_t ignore_offset)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      return list;
      break;
      
    case EXPR_DATA:
      return (expr->e_offset == ignore_offset) ? list : list_prepend(list, OFFSET_TO_POINTER(expr->e_offset));
      break;
      
    case EXPR_ADD:
    case EXPR_MUL:
      list = expr_get_list_of_ref_internal_but_ignore_offset(expr->e_expr1, list, ignore_offset);
      list = expr_get_list_of_ref_internal_but_ignore_offset(expr->e_expr2, list, ignore_offset);
      return list;
      break;

    default:
      SHOULDNT_HAPPEN();
      return NULL;
      break;
  }
}

static List* expr_get_list_of_ref_internal(Expression* expr, List* list)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      return list;
      break;
      
    case EXPR_DATA:
      return list_prepend(list, OFFSET_TO_POINTER(expr->e_offset));
      break;
      
    case EXPR_ADD:
    case EXPR_MUL:
      list = expr_get_list_of_ref_internal(expr->e_expr1, list);
      list = expr_get_list_of_ref_internal(expr->e_expr2, list);
      return list;
      break;

    default:
      SHOULDNT_HAPPEN();
      return NULL;
      break;
  }
}

static int expr_complexity(Expression* expr)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      return 0;

    case EXPR_DATA:
      return 1;
      break;

    case EXPR_ADD:
    case EXPR_MUL:
      return 1 + expr_complexity(expr->e_expr1) + expr_complexity(expr->e_expr2);

    default:
      SHOULDNT_HAPPEN();
      return 0;
      break;
  }
}

static void expr_subst_offset_by_expr(Expression* expr, OFFSET_t offset, Expression* substExpr)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      break;

    case EXPR_DATA:
      if (expr->e_offset == offset)
      {
        Expression* dup = expr_dup(substExpr);
        *expr = *dup;
        free(dup);
      }
      break;

    case EXPR_ADD:
    case EXPR_MUL:
      expr_subst_offset_by_expr(expr->e_expr1, offset, substExpr);
      expr_subst_offset_by_expr(expr->e_expr2, offset, substExpr);
      break;

    default:
      SHOULDNT_HAPPEN();
      break;
  }
}

static void expr_inc_offset_by(Expression* expr, OFFSET_t delta_offset)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      break;

    case EXPR_DATA:
      expr->e_offset += delta_offset;
      break;

    case EXPR_ADD:
    case EXPR_MUL:
      expr_inc_offset_by(expr->e_expr1, delta_offset);
      expr_inc_offset_by(expr->e_expr2, delta_offset);
      break;

    default:
      SHOULDNT_HAPPEN();
      break;
  }
}

static Expression* expr_simplify(Expression* expr)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
    case EXPR_DATA:
      return expr_dup(expr);
      break;
      
    case EXPR_ADD:
    {
      Expression* expr1 = expr_simplify(expr->e_expr1);
      Expression* expr2 = expr_simplify(expr->e_expr2);
      Expression* ret = NULL;
      if (expr1->e_type == EXPR_CONSTANT && expr2->e_type == EXPR_CONSTANT)
      {
        ret = expr_constant_new(expr1->e_constant + expr2->e_constant);
      }
      else if (expr1->e_type == EXPR_CONSTANT && expr1->e_constant == 0)
      {
        ret = expr_dup(expr2);
      }
      else if (expr2->e_type == EXPR_CONSTANT && expr2->e_constant == 0)
      {
        ret = expr_dup(expr1);
      }
      else if (expr1->e_type == EXPR_DATA && expr2->e_type == EXPR_CONSTANT)
      {
        ret = expr_add_new(expr_dup(expr2), expr_dup(expr1));
      }
      else if (expr->e_type == EXPR_ADD &&
               expr1->e_type == EXPR_CONSTANT && expr2->e_type == EXPR_ADD &&
               expr2->e_expr1->e_type == EXPR_CONSTANT && expr2->e_expr2->e_type == EXPR_DATA)
      {
        int constant = expr1->e_constant + expr2->e_expr1->e_constant;
        if (constant == 0)
          ret = expr_data_new(expr2->e_expr2->e_offset);
        else
          ret = expr_add_new(expr_constant_new(constant), expr_data_new(expr2->e_expr2->e_offset));
      }
      else
      {
        ret = expr_add_new(expr_dup(expr1), expr_dup(expr2));
      }
      expr_free(expr1);
      expr_free(expr2);
      return ret;
    }
      
    case EXPR_MUL:
    {
      Expression* expr1 = expr_simplify(expr->e_expr1);
      Expression* expr2 = expr_simplify(expr->e_expr2);
      Expression* ret = NULL;
      if (expr1->e_type == EXPR_CONSTANT && expr2->e_type == EXPR_MUL &&
          expr2->e_expr1->e_type == EXPR_CONSTANT)
      {
        Expression* tmp_expr = expr_mul_new(
            expr_constant_new(expr1->e_constant * expr2->e_expr1->e_constant),
            expr_dup(expr2->e_expr2));
        ret = expr_simplify(tmp_expr);
        expr_free(tmp_expr);
      }
      else if (expr1->e_type == EXPR_CONSTANT && expr2->e_type == EXPR_CONSTANT)
      {
        ret = expr_constant_new(expr1->e_constant * expr2->e_constant);
      }
      else if (expr1->e_type == EXPR_CONSTANT && expr1->e_constant == 1)
      {
        ret = expr_dup(expr2);
      }
      else if (expr2->e_type == EXPR_CONSTANT && expr2->e_constant == 1)
      {
        ret = expr_dup(expr1);
      }
      else if ((expr1->e_type == EXPR_CONSTANT && expr1->e_constant == 0) ||
              (expr2->e_type == EXPR_CONSTANT && expr2->e_constant == 0))
      {
        ret = expr_constant_new(0);
      }
      else
      {
        ret = expr_mul_new(expr_dup(expr1), expr_dup(expr2));
      }
      expr_free(expr1);
      expr_free(expr2);
      return ret;
    }

    default:
      SHOULDNT_HAPPEN();
      break;
  }
}

/* -------------------------------------------------------- */

static void libbf_interpreted_code_list_free_elt(List* list)
{
  OptimizationUnit* unit = (OptimizationUnit*) list->data;
  if (unit->ou_instr == BF_AFFECT_EXPR)
  {
    expr_free(unit->ou_expr);
  }

  list_free_elt(list, free);
}

static void libbf_interpreted_code_list_free(List* list)
{
  while(list)
  {
    List* next = list->next;
    libbf_interpreted_code_list_free_elt(list);
    list = next;
  }
}

static OptimizationUnit* libbf_interpreted_code_list_to_array(List* list)
{
  int i = 0;
  int n = list_get_size(list);
  OptimizationUnit* array = alloc0(OptimizationUnit, n+1);
  while(list)
  {
    OptimizationUnit* unit = (OptimizationUnit*) list->data;
    
    array[i] = *unit;
    if (unit->ou_instr == BF_AFFECT_EXPR)
    {
      array[i].ou_expr = expr_dup(unit->ou_expr);
    }
    i++;
    
    list = list->next;
  }
  array[i].ou_instr = BF_END;
  
  return array;
}

static void libbf_interpreted_code_array_free(OptimizationUnit* code)
{
  OptimizationUnit* code_ptr = code;
  
  while(TRUE)
  {
    switch(code_ptr->ou_instr)
    {
      case BF_END:
        free(code);
        return;
        break;
        
      case BF_AFFECT_EXPR:
        expr_free(code_ptr->ou_expr);
        break;
        
      default:
        break;
    }
    
    code_ptr++;
  }
}



/* -------------------------------------------------------- */


static List* libbf_translate_to_optimization_code(const char* code)
{
  InterpreterUnit* code2 = libbf_gen_interpreted_code(code);
  InterpreterUnit* code2_ptr = code2;
  
  List* list = NULL;
  List* list_end = NULL;

  while(code2_ptr->instr != BF_SIMPLE_END)
  {
    OptimizationUnit* new_code = alloc0(OptimizationUnit, 1);
    list_end = list_append(list_end, new_code);
    if (list == NULL) list = list_end;
    if (list_end->next) list_end = list_end->next;

    switch(code2_ptr->instr)
    {
      case BF_SIMPLE_PLUS :
        new_code->ou_instr = BF_AFFECT_EXPR;
        new_code->ou_offset = 0;
        new_code->ou_expr = expr_add_new(expr_constant_new(1), expr_data_new(0));
        break;

      case BF_SIMPLE_MINUS :
        new_code->ou_instr = BF_AFFECT_EXPR;
        new_code->ou_offset = 0;
        new_code->ou_expr = expr_add_new(expr_constant_new(-1), expr_data_new(0));
        break;

      case BF_SIMPLE_GREATER :
        new_code->ou_instr = BF_INC_DATA_PTR;
        new_code->ou_offset = 1;
        break;

      case BF_SIMPLE_LESSER :
        new_code->ou_instr = BF_INC_DATA_PTR;
        new_code->ou_offset = -1;
        break;

      case BF_SIMPLE_DOT :
        new_code->ou_instr = BF_PRINT_DATA;
        new_code->ou_offset = 0;
        break;

      case BF_SIMPLE_COMMA :
        new_code->ou_instr = BF_READ_DATA;
        new_code->ou_offset = 0;
        break;

      case BF_SIMPLE_LBRACKET :
        new_code->ou_instr = BF_WHILE_DATA;
        new_code->ou_offset = 0;
        new_code->ou_jump_to = 0;
        new_code->ou_loop_type = LIBBF_WHILE;
        break;

      case BF_SIMPLE_RBRACKET :
        new_code->ou_instr = BF_END_WHILE;
        new_code->ou_offset = 0;
        new_code->ou_jump_to = 0;
        new_code->ou_loop_type = LIBBF_WHILE;
        break;

      default:
        SHOULDNT_HAPPEN();
        break;

    }
    code2_ptr++;
        
  }

  free(code2);

  return list;
}

static void printf_spaces(FILE* f, int inc)
{
  while(--inc >= 0)
    fprintf(f, "  ");
}

void libbf_interpreter_optimizer_cdump_internal_instr(FILE* f, OptimizationUnit* code_ptr, int* pinc, int* pnumline)
{
  int inc = (pinc) ? *pinc : 0;
  
  long constant = code_ptr->ou_constant;
  long offset = code_ptr->ou_offset;
  long offset2 = code_ptr->ou_offset2;
  long offset3 = code_ptr->ou_offset3;
  
  if (pnumline)
  {
    (*pnumline)++;
    fprintf(f, "/* %d: (%d) */  ", *pnumline, code_ptr->ou_nb_visits);
  }
  
  switch(code_ptr->ou_instr)
  {
    case BF_AFFECT_EXPR:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] ", offset);
      if (code_ptr->ou_expr->e_type == EXPR_ADD &&
          code_ptr->ou_expr->e_expr2->e_type == EXPR_DATA &&
          code_ptr->ou_expr->e_expr2->e_offset == offset)
      {
        fprintf(f, "+= ");
        expr_dump(f, "data_ptr[", "]", code_ptr->ou_expr->e_expr1);
      }
      else
      {
        fprintf(f, "= ");
        expr_dump(f, "data_ptr[", "]", code_ptr->ou_expr);
      }
      fprintf(f, ";\n");
      break;

    case BF_INC_DATA_PTR:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr += %ld;\n", offset);
      break;

    case BF_PRINT_DATA:
      printf_spaces(f, inc);
      fprintf(f, "putchar(data_ptr[%ld]);\n", offset);
      break;

    case BF_READ_DATA:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] = getchar();\n", offset);
      break;

    case BF_WHILE_DATA:
      printf_spaces(f, inc);
      if (code_ptr->ou_loop_type == LIBBF_DO_WHILE)
      {
        fprintf(f, "do\n");
      }
      else
      {
        if (code_ptr->ou_loop_type == LIBBF_IF)
          fprintf(f, "if");
        else
          fprintf(f, "while");
        fprintf(f, "(data_ptr[%ld] != 0)\n", offset);
      }
      if (pnumline)
      {
        (*pnumline)++;
        fprintf(f, "/* %d:      */  ", *pnumline);
      }
      printf_spaces(f, inc);
      fprintf(f, "{\n");
      inc++;
      break;

    case BF_NOP:
    case BF_END_WHILE:
      inc--;
      printf_spaces(f, inc);
      if (code_ptr->ou_loop_type == LIBBF_DO_WHILE)
      {
        fprintf(f, "}\n");
        printf_spaces(f, inc);
        fprintf(f, "while (data_ptr[%ld] != 0);", offset);
      }
      else
      {
        fprintf(f, "}");
        if (DEBUG)
        {        
          fprintf(f, " /* end ");
          if (code_ptr->ou_loop_type == LIBBF_IF)
            fprintf(f, "if");
          else
            fprintf(f, "while");
          fprintf(f, "(data_ptr[%ld] != 0) */", offset);
        }
      }
      fprintf(f, "\n");
      break;

    case BF_SET_CST:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] = %ld;\n", offset, constant);
      break;

    case BF_SET_CST_0:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] = 0;\n", offset);
      break;

    case BF_INC_CST:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] += %ld;\n", offset, constant);
      break;

    case BF_INC_CST_1:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] += 1;\n", offset);
      break;

    case BF_INC_CST_M_1:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] -= 1;\n", offset);
      break;

    case BF_SET_DATA:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] = data_ptr[%ld];\n", offset, offset2);
      break;

    case BF_INC_DATA:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] += data_ptr[%ld];\n", offset, offset2);
      break;

    case BF_DEC_DATA:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] -= data_ptr[%ld];\n", offset, offset2);
      break;

    case BF_INC_CST_MUL_DATA:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] += %ld * data_ptr[%ld];\n", offset, constant, offset2);
      break;

    case BF_INC_CST_ADD_DATA:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] += %ld + data_ptr[%ld];\n", offset, constant, offset2);
      break;

    case BF_INC_DATA_MUL_DATA:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr[%ld] += data_ptr[%ld] * data_ptr[%ld];\n", offset, offset2, offset3);
      break;

    case BF_WHILE_INC_DATA_PTR:
      printf_spaces(f, inc);
      fprintf(f, "while(data_ptr[%ld] != 0)\n", offset);
      printf_spaces(f, inc);
      fprintf(f, "{\n");
      printf_spaces(f, inc);
      fprintf(f, "  data_ptr += %ld;\n", offset2);
      printf_spaces(f, inc);
      fprintf(f, "}\n");
      break;

#ifdef USE_END_WHILE_INC_DATA_PTR      
    case BF_END_WHILE_INC_DATA_PTR:
      printf_spaces(f, inc);
      fprintf(f, "data_ptr += %ld;\n", offset2);
      inc--;
      printf_spaces(f, inc);
      fprintf(f, "}");
      if (DEBUG)
      {
        fprintf(f, " /* end while(data_ptr[%ld] != 0) */", offset);
      }
      fprintf(f, "\n");
      break;
#endif

    case BF_END:
      printf_spaces(f, inc);
      fprintf(f, "/* end */\n");
      break;

    default:
      SHOULDNT_HAPPEN();
      break;
  }

  if (pinc) *pinc = inc;
}

static void libbf_interpreter_optimizer_cdump_internal(FILE* f,
                                                       List* list,
                                                       const char* data_type,
                                                       int n_instr_limit)
{
  int inc = 1;
  int count_instr = 0;

  if (n_instr_limit < 0)
  {
    fprintf(f, "#include <stdlib.h>\n");
    fprintf(f, "#include <string.h>\n");
    fprintf(f, "#include <stdio.h>\n");
    fprintf(f, "\n");
    fprintf(f, "int main(int argc, char* argv[])\n");
    fprintf(f, "{\n");
    /*
      printf_spaces(f, inc);
      fprintf(f, "unsigned %s  data[32768] = {0};\n", data_type);
      printf_spaces(f, inc);
      fprintf(f, "unsigned %s* data_ptr = data;\n", data_type);
    */
    printf_spaces(f, inc);
    fprintf(f, "unsigned %s* data_ptr = calloc(%d, sizeof(unsigned %s));\n", data_type, 32768, data_type);
    printf_spaces(f, inc);
    fprintf(f, "data_ptr += 1024;\n");
  }


  while(list)
  {
    OptimizationUnit* code_ptr;

      if (n_instr_limit >= 0 && count_instr++ >= n_instr_limit)
    {
      return;
    }
    code_ptr = (OptimizationUnit*) list->data;
    
    libbf_interpreter_optimizer_cdump_internal_instr(f, code_ptr, &inc, NULL);
    
    list = list->next;
  }
  
  if (n_instr_limit < 0)
  {
    printf_spaces(f, inc);
    fprintf(f, "return 0;\n");
    inc--;
    printf_spaces(f, inc);
    fprintf(f, "}\n");
  }
}


static void libbf_interpreter_optimizer_javadump_internal(FILE* f,
    List* list,
    const char* data_type,
    int n_instr_limit)
{
  int inc = 1;
  int count_instr = 0;

  if (n_instr_limit < 0)
  {
    fprintf(f, "public class test\n");
    fprintf(f, "{\n");
    printf_spaces(f, inc);
    fprintf(f, "public static void main(String argv[]) throws java.io.IOException\n");
    printf_spaces(f, inc);
    fprintf(f, "{\n");
    inc++;
    printf_spaces(f, inc);
    fprintf(f, "%s data[] = new %s[32768];\n", data_type, data_type);
    printf_spaces(f, inc);
    fprintf(f, "int offset = 0;\n");
  }


  while(list)
  {
    OptimizationUnit* code_ptr;
    long offset;

    if (n_instr_limit >= 0 && count_instr++ >= n_instr_limit)
    {
      return;
    }
    
    code_ptr = (OptimizationUnit*) list->data;
    offset = code_ptr->ou_offset;
    
    switch(code_ptr->ou_instr)
    {
      case BF_AFFECT_EXPR:
        printf_spaces(f, inc);
        fprintf(f, "data[offset+%ld] ", offset);
        if (code_ptr->ou_expr->e_type == EXPR_ADD &&
            code_ptr->ou_expr->e_expr2->e_type == EXPR_DATA &&
            code_ptr->ou_expr->e_expr2->e_offset == offset)
        {
          fprintf(f, "+= ");
          expr_dump(f, "data[offset+", "]", code_ptr->ou_expr->e_expr1);
        }
        else
        {
          fprintf(f, "= ");
          expr_dump(f, "data[offset+", "]", code_ptr->ou_expr);
        }
        fprintf(f, ";\n");
        break;

      case BF_INC_DATA_PTR:
        printf_spaces(f, inc);
        fprintf(f, "offset += %ld;\n", offset);
        break;

      case BF_PRINT_DATA:
        printf_spaces(f, inc);
        fprintf(f, "System.out.print((char)data[offset+%ld]);\n", offset);
        break;

      case BF_READ_DATA:
        printf_spaces(f, inc);
        fprintf(f, "data[offset+%ld] = System.in.read();\n", offset);
        break;

      case BF_WHILE_DATA:
        printf_spaces(f, inc);
        if (code_ptr->ou_loop_type == LIBBF_IF)
          fprintf(f, "if");
        else
          fprintf(f, "while");
        fprintf(f, "(data[offset+%ld] != 0)\n", offset);
        printf_spaces(f, inc);
        fprintf(f, "{\n");
        inc++;
        break;

      case BF_END_WHILE:
        inc--;
        printf_spaces(f, inc);
        fprintf(f, "}\n");
        break;

      default:
        SHOULDNT_HAPPEN();
        break;
    }
    
    list = list->next;
  }
  
  if (n_instr_limit < 0)
  {
    inc--;
    printf_spaces(f, inc);
    fprintf(f, "}\n");
    inc--;
    printf_spaces(f, inc);
    fprintf(f, "}\n");
  }
}

static int is_inc_expr_of_1(Expression* expr, OFFSET_t offset)
{
  return (expr->e_type == EXPR_ADD &&
          expr->e_expr1->e_type == EXPR_CONSTANT &&
          abs(expr->e_expr1->e_constant) == 1 &&
          expr->e_expr2->e_type == EXPR_DATA &&
          expr->e_expr2->e_offset == offset);
}

/* Optimize ++++, ----, >>>>> and <<<<< */
static int libbf_very_simple_optimization(List* list)
{
  int has_done_changes = FALSE;

  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    List* next = list->next;
    OptimizationUnit* code_next = (OptimizationUnit*) ((next) ? next->data : NULL);
    
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
        assert(is_inc_expr_of_1(code->ou_expr, code->ou_offset));
        while (code_next &&
              code->ou_instr == code_next->ou_instr &&
              code->ou_offset == code_next->ou_offset)
        {
          List* next_next = next->next;
          
          assert(is_inc_expr_of_1(code_next->ou_expr, code_next->ou_offset));
          
          has_done_changes = TRUE;
          code->ou_expr->e_expr1->e_constant += code_next->ou_expr->e_expr1->e_constant;

          libbf_interpreted_code_list_free_elt(next);

          next = next_next;
          code_next = (OptimizationUnit*) ((next) ? next->data : NULL);
        }
        break;

      case BF_INC_DATA_PTR:
        while (code_next &&
              code->ou_instr == code_next->ou_instr)
        {
          List* next_next = next->next;
          has_done_changes = TRUE;
          code->ou_offset += code_next->ou_offset;
          
          libbf_interpreted_code_list_free_elt(next);

          next = next_next;
          code_next = (OptimizationUnit*) ((next) ? next->data : NULL);
        }
        break;

      default:
        break;
    }
    
    list = next;
  }

  return has_done_changes;
}

static int libbf_optimize_remove_inc_data_ptr(List** p_list)
{
  List* list = *p_list;
  List* next;
  
  int has_done_changes = TRUE;
  int level = 0;
  OFFSET_t offsets[256];
  OFFSET_t prev_sum_offset = 0;
  offsets[level] = 0;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        code->ou_offset += offsets[level] + prev_sum_offset;
        prev_sum_offset += offsets[level];
        level++;
        offsets[level] = 0;
        break;

      case BF_END_WHILE:
        if (offsets[level] != 0)
        {
          OptimizationUnit* new_code = alloc0(OptimizationUnit, 1);
          new_code->ou_instr = BF_INC_DATA_PTR;
          new_code->ou_offset = offsets[level];

          /* Chain new BF_INC_DATA_PTR before BF_END_WHILE */
          list_insert_before(list, new_code);
        }
        level--;
        prev_sum_offset -= offsets[level];
        code->ou_offset += offsets[level] + prev_sum_offset;
        break;

      case BF_INC_DATA_PTR:
        offsets[level] += code->ou_offset;
        
        /* Remove ADD_DATA_PTR instr */
        next = list->next;
        if (list == *p_list)
          *p_list = next;
        libbf_interpreted_code_list_free_elt(list);
        list = next;
        continue;
        break;
        
      case BF_AFFECT_EXPR:
        expr_inc_offset_by(code->ou_expr, offsets[level] + prev_sum_offset);
        /* fallback through */

      default:
        code->ou_offset += offsets[level] + prev_sum_offset;
        break;
    }

    list = list->next;
  }

  return has_done_changes;
}

static int libbf_is_innerloop(List* list)
{
  OptimizationUnit* code = (OptimizationUnit*) list->data;
  
  assert(code->ou_instr == BF_WHILE_DATA);

  list = list->next;  
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        return FALSE;
        break;

      case BF_END_WHILE:
        return TRUE;
        break;

      default:
        break;
    }
    list = list->next;
  }

  SHOULDNT_HAPPEN();
  return FALSE;
}

static int is_inc_expr_by_constant(Expression* expr, OFFSET_t offset)
{
  return (expr->e_type == EXPR_ADD &&
      expr->e_expr1->e_type == EXPR_CONSTANT &&
      expr->e_expr2->e_type == EXPR_DATA &&
      expr->e_expr2->e_offset == offset);
}

static int libbf_can_transform_while_loop_into_mul_expr(List* list, CONSTANT_t* p_count_increment)
{
  OptimizationUnit* code = (OptimizationUnit*) list->data;
  
  OFFSET_t interest_offset = code->ou_offset;
  CONSTANT_t count_increment = 0;
  assert(code->ou_instr == BF_WHILE_DATA);
  
  list = list->next;
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        SHOULDNT_HAPPEN();
        break;

      case BF_END_WHILE:
        *p_count_increment = count_increment;
        return count_increment == -1 || count_increment == 1;
        break;

      case BF_AFFECT_EXPR:
        if (!is_inc_expr_by_constant(code->ou_expr, code->ou_offset)) /* data[offset] = constant + data[offset] */
          return FALSE;
        
        if (code->ou_offset == interest_offset)
          count_increment += code->ou_expr->e_expr1->e_constant;
        break;

      case BF_INC_DATA_PTR:
      case BF_PRINT_DATA:
      case BF_READ_DATA:
        return FALSE;
        break;

      default:
        SHOULDNT_HAPPEN();
        break;
    }

    list = list->next;
  }

  SHOULDNT_HAPPEN();
  return FALSE;
}

/* Optimize following sequences :
  while(data[0])
  {
    data[1] += 2;
    data[0] += -1;
    data[3] -= 3;
  }

  into:
  data[1] += 2 * data[0];
  data[3] -= 3 * data[0];
  data[0] = 0;
*/   
static int libbf_transform_while_loop_into_mul_expr(List** p_list)
{
  List* list = *p_list;
  

  int has_done_changes = FALSE;
  OFFSET_t interest_offset = -1;
  int can_optimize = FALSE;
  CONSTANT_t count_increment = 0;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        
        /* Special case : delete useless initial while loops */
        if (list->prev != NULL)
        {
          OptimizationUnit* prev_code = (OptimizationUnit*) list->prev->data;
          if (prev_code->ou_instr == BF_AFFECT_EXPR &&
              prev_code->ou_offset == code->ou_offset &&
              prev_code->ou_expr->e_type == EXPR_CONSTANT &&
              prev_code->ou_expr->e_constant == 0)
          {
            int level = 0;
            has_done_changes = TRUE;
            
            do
            {
              OptimizationUnit* cur_code = (OptimizationUnit*) list->data;
              List* next = list->next;
              if (cur_code->ou_instr == BF_WHILE_DATA) level++;
              else if (cur_code->ou_instr == BF_END_WHILE) level--;
              if (list == *p_list)
                *p_list = next;
              libbf_interpreted_code_list_free_elt(list);
              list = next;
            }
            while(level > 0);
            continue;
          }
        }
        
        
        if (libbf_is_innerloop(list) && libbf_can_transform_while_loop_into_mul_expr(list, &count_increment))
        {
          List* next = list->next;
          can_optimize = TRUE;

          has_done_changes = TRUE;
          interest_offset = code->ou_offset;

          /* Remove BF_WHILE_DATA instr */
          if (list == *p_list)
            *p_list = next;
          libbf_interpreted_code_list_free_elt(list);
          list = next;
          continue;
        }
        break;

      case BF_END_WHILE:
        if (can_optimize)
        {
          code->ou_instr  = BF_AFFECT_EXPR;
          code->ou_offset = interest_offset;
          code->ou_expr   = expr_constant_new(0);

          can_optimize = FALSE;
        }
        break;

      case BF_AFFECT_EXPR:
        if (can_optimize)
        {
          assert(is_inc_expr_by_constant(code->ou_expr, code->ou_offset));  /* data[offset] = constant + data[offset] */
          
          if (code->ou_offset != interest_offset)
          {
            Expression* new_expr;
            /* data[offset] = -count_increment * constant * data[interest_offset] + data[offset] */
            code->ou_expr->e_expr1 = expr_mul_new(expr_constant_new(-count_increment),
                                            expr_mul_new(code->ou_expr->e_expr1, expr_data_new(interest_offset)));
            new_expr = expr_simplify(code->ou_expr);
            expr_free(code->ou_expr);
            code->ou_expr = new_expr;
          }
          else
          {
            /* Remove BF_AFFECT_EXPR instr */
            List* next = list->next;
            if (list == *p_list)
              *p_list = next;
            libbf_interpreted_code_list_free_elt(list);
            list = next;
            continue;
          }
        }
        break;

      default:
        break;
    }

    list = list->next;
  }

  return has_done_changes;
}

/* Optimize data[x] = constant or f(data[x])
            ...
            data[x] = stuff or data[y] = f(data[x], ...)
 */

static int libbf_optimize_group_expr_int(List** p_list, List* cur)
{
  List* first = cur;
  OptimizationUnit* codeRef = (OptimizationUnit*) cur->data;
  int level = 0;
  
  /* Go on if data[x] = constant or data[x] = f(data[x]) */
  if (!(codeRef->ou_expr->e_type == EXPR_CONSTANT ||
        expr_ref_offset_and_only_offset(codeRef->ou_expr, codeRef->ou_offset))) return FALSE;
  
  cur = cur->next;
  
  while(cur)
  {
    OptimizationUnit* code = (OptimizationUnit*) cur->data;

    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        if (code->ou_offset == codeRef->ou_offset)
        {
          return FALSE; /* FIXME : too strong */
        }        
        level++;
        break;
        
      case BF_END_WHILE:
        level--;
        if (level < 0)
          return FALSE;
        break;
        
      case BF_INC_DATA_PTR:
        return FALSE;
        break;

      case BF_AFFECT_EXPR:
      {
        if (level == 0)
        {
          int ret = FALSE;
          int code_offset = code->ou_offset;
          
          /* Stop for following seq : */
          /*   data[x] = f(data[x]) */
          /*   data[y] = f(data[x], ...) */
          if (codeRef->ou_expr->e_type != EXPR_CONSTANT &&
              expr_ref_offset(code->ou_expr, codeRef->ou_offset) &&
              code->ou_offset != codeRef->ou_offset)
          {
            return FALSE;
          }

          if (expr_ref_offset(code->ou_expr, codeRef->ou_offset))
          {
            Expression* new_expr;
            Expression* old_expr = expr_dup(code->ou_expr);
            int complexity_before = expr_complexity(code->ou_expr);
                
            if (DEBUG)
            {
              fprintf(stderr, "\nbefore_optimize (expr) :\n");
              libbf_interpreter_optimizer_cdump_internal(stderr, first, "", 10);
            }
            
            expr_subst_offset_by_expr(code->ou_expr, codeRef->ou_offset, codeRef->ou_expr);
            new_expr = expr_simplify(code->ou_expr);
            expr_free(code->ou_expr);
            code->ou_expr = new_expr;
            if (expr_complexity(code->ou_expr) > complexity_before)
            {
              expr_free(code->ou_expr);
              code->ou_expr = old_expr;
              return FALSE;
            }
            else
            {
              expr_free(old_expr);
            }
            if (code->ou_offset == codeRef->ou_offset &&
                code->ou_expr->e_type == EXPR_DATA &&
                code->ou_expr->e_offset == codeRef->ou_offset)
            {
              libbf_interpreted_code_list_free_elt(cur);
            }
            
            ret = TRUE;
            
            if (DEBUG)
            {
              fprintf(stderr, "\nafter_optimize (expr) :\n");
              libbf_interpreter_optimizer_cdump_internal(stderr, first, "", 10);
            }
          }
          
          if (code_offset == codeRef->ou_offset)
          {
            /* Remove first data[offset] = constant instr */
            if (first == *p_list)
              *p_list = first->next;
            libbf_interpreted_code_list_free_elt(first);
            ret = TRUE;
          }

          if (ret) return TRUE;
          break;
        }
        else
        {
          if (code->ou_offset == codeRef->ou_offset ||
              expr_ref_offset(code->ou_expr, codeRef->ou_offset))
          {
            return FALSE; /* FIXME : too strong */
          }
        }
      }

      case BF_PRINT_DATA:
        if (code->ou_offset == codeRef->ou_offset)
          return FALSE;
        break;

      case BF_READ_DATA:
        if (code->ou_offset == codeRef->ou_offset)
          return FALSE;
        break;

      default:
        SHOULDNT_HAPPEN();
        break;
    }
    
    cur = cur->next;
  }
  
  return FALSE;
}


static int libbf_optimize_group_expr(List** p_list)
{
  List* list = *p_list;

  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
      {
        List* next = list->next;
        has_done_changes |= libbf_optimize_group_expr_int(p_list, list);
        list = next;
        continue;
        break;
      }
      
      default:
        break;
    }

    list = list->next;
  }

  return has_done_changes;
}


/* Optimize data[x] = data[y];
            stuff where no data[x]= or data[y]=;
            data[z] = fct(data[x])
  into
            data[x] = data[y];
            stuff;
            data[z] = fct(data[y])
*/
static int libbf_optimize_x_eq_y_z_fct_x_internal(List* list)
{
  OptimizationUnit* codeRef = (OptimizationUnit*) list->data;
  OFFSET_t x = codeRef->ou_offset;
  OFFSET_t y = codeRef->ou_expr->e_offset;
  int level = 0;
  
  list = list->next;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        level++;
        break;
        
      case BF_END_WHILE:
        level--;
        if (level < 0) return FALSE;
        break;
        
      case BF_INC_DATA_PTR:
        return FALSE;
        break;
        
      case BF_PRINT_DATA:
      case BF_READ_DATA:
        if (code->ou_offset == x || code->ou_offset == y)
          return FALSE;
        break;
        
      case BF_AFFECT_EXPR:
      {
        if (code->ou_offset == x) return FALSE;
        if (code->ou_offset == y)
        {
          if (level == 0 && expr_ref_offset(code->ou_expr, x))
          {
            if (code->ou_expr->e_type == EXPR_DATA && code->ou_expr->e_offset == x)
            {
              libbf_interpreted_code_list_free_elt(list);
            }
            else
            {
              Expression* subst_expr = expr_data_new(y);
              Expression* exprBefore = code->ou_expr;
              expr_subst_offset_by_expr(code->ou_expr, x, subst_expr);
              expr_free(subst_expr);
              code->ou_expr = expr_simplify(exprBefore);
              expr_free(exprBefore);
            }
            return TRUE;
          }
          else
          {
            return FALSE;
          }
        }
          
        if (level == 0)
        {
          if (expr_ref_offset(code->ou_expr, x))
          {
            Expression* subst_expr = expr_data_new(y);
            expr_subst_offset_by_expr(code->ou_expr, x, subst_expr);
            expr_free(subst_expr);
            return TRUE;
          }
        }

        break;
      }

      default:
        break;
    }

    list = list->next;
  }
  return FALSE;
  
}

/* Optimize data[x] = data[y];
            stuff where no data[a]=, data[b]=, etc... and no data[eps] = fct(data[z])
            data[z] = fct(data[x], data[a], data[b], ...) where z!=x and z!=y
  into
            data[x] = data[y];
            data[z] = fct(data[y], data[a], data[b], ...)
            stuff;
*/
static int libbf_optimize_x_eq_y_z_fct_x_internal2(List* list)
{
  OptimizationUnit* codeRef = (OptimizationUnit*) list->data;
  OFFSET_t x = codeRef->ou_offset;
  OFFSET_t y = codeRef->ou_expr->e_offset;
  List* first = list;
  List* list_of_offset_left = NULL;
  List* list_of_offset_right = NULL;
  int ret = FALSE;
  
  list = list->next;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
      case BF_END_WHILE:
      case BF_INC_DATA_PTR:
        goto libbf_optimize_x_eq_y_z_fct_x_internal2_end;
        break;
        
      case BF_PRINT_DATA:
      case BF_READ_DATA:
        if (code->ou_offset == x || code->ou_offset == y)
          goto libbf_optimize_x_eq_y_z_fct_x_internal2_end;
        list_of_offset_left = list_prepend(list_of_offset_left, OFFSET_TO_POINTER(code->ou_offset));
        break;
        
      case BF_AFFECT_EXPR:
      {
        if (code->ou_offset == x)
          goto libbf_optimize_x_eq_y_z_fct_x_internal2_end;
        if (expr_ref_offset(code->ou_expr, x))
        {
          List* list_referenced_offsets =
              expr_get_list_of_ref_internal(code->ou_expr,
                                            list_prepend(NULL, OFFSET_TO_POINTER(code->ou_offset)));
          int intersects = list_intersects(list_of_offset_left, list_referenced_offsets);
          list_free(list_referenced_offsets, NULL);
          if (intersects == FALSE &&
              list_find(list_of_offset_right, OFFSET_TO_POINTER(code->ou_offset)) == FALSE)
          {
            OptimizationUnit* new_code = alloc0(OptimizationUnit, 1);
            new_code->ou_instr  = BF_AFFECT_EXPR;
            new_code->ou_offset = code->ou_offset;
            new_code->ou_expr   = expr_dup(code->ou_expr);
            libbf_interpreted_code_list_free_elt(list);
            list_insert_before(first->next, new_code);
            ret = TRUE;
          }
          goto libbf_optimize_x_eq_y_z_fct_x_internal2_end;
        }
        else
        {
          list_of_offset_left = list_prepend(list_of_offset_left, OFFSET_TO_POINTER(code->ou_offset));
          list_of_offset_right = expr_get_list_of_ref_internal(code->ou_expr, list_of_offset_right);
        }
        break;
      }

      default:
        break;
    }

    list = list->next;
  }
libbf_optimize_x_eq_y_z_fct_x_internal2_end:
  list_free(list_of_offset_left, NULL);
  list_free(list_of_offset_right, NULL);
  return ret;
  
}

static int libbf_optimize_x_eq_y_z_fct_x(List** p_list)
{
  List* list = *p_list;

  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
      {
        if (code->ou_expr->e_type == EXPR_DATA)
        {
          has_done_changes |= libbf_optimize_x_eq_y_z_fct_x_internal(list);
          has_done_changes |= libbf_optimize_x_eq_y_z_fct_x_internal2(list);
        }

        break;
      }

      default:
        break;
    }

    list = list->next;
  }

  return has_done_changes;
}

/* Optimize
data_ptr[x] += data_ptr[y];
... no data_ptr[x] = ... or data_ptr[y]= ... or data_ptr[z] = f(data_ptr[x])
data_ptr[y] = data_ptr[x];
... no data_ptr[y] = ... or data_ptr[x] = ... or data_ptr[z] = f(data_ptr[x])
data_ptr[x] = f(all but data_ptr[y]);

into 

data_ptr[y] += data_ptr[x];
data_ptr[x] = f(...)
*/
static int libbf_optimize_x_pluseq_y_y_eq_x_x_eq_stuff_internal(List* list, List** p_list)
{
  List* listRef = list;
  
  OptimizationUnit* codeRef = (OptimizationUnit*) list->data;
  OFFSET_t x = codeRef->ou_offset;
  OFFSET_t y = codeRef->ou_expr->e_expr1->e_offset;
  int level = 0;
  int firstStage = FALSE;
  OptimizationUnit* codeFirstStage = NULL;
  
  list = list->next;
  
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        level++;
        break;
        
      case BF_END_WHILE:
        level--;
        if (level < 0) return FALSE;
        break;
        
      case BF_INC_DATA_PTR:
        return FALSE;
        break;
        
      case BF_PRINT_DATA:
        if (code->ou_offset == x)
          return FALSE;
        
      case BF_READ_DATA:
        if (code->ou_offset == x || code->ou_offset == y)
          return FALSE;
        break;
        
      case BF_AFFECT_EXPR:
      {
        if (level == 0)
        {
          if (firstStage == FALSE)
          {
            if (code->ou_offset == x)
              return FALSE;
            
            if (code->ou_offset == y)
            {
              if (code->ou_expr->e_type == EXPR_DATA && code->ou_expr->e_offset == x)
              {
                firstStage = TRUE;
                codeFirstStage = code;
              }
              else
              {
                return FALSE;
              }
            }
            else if (expr_ref_offset(code->ou_expr, x))
            {
              return FALSE;
            }
          }
          else
          {
            if (code->ou_offset == y)
              return FALSE;
            
            if (code->ou_offset == x)
            {
              if (expr_ref_offset(code->ou_expr, y))
              {
                return FALSE;
              }
              else
              {
                if (listRef == *p_list)
                {
                  *p_list = listRef->next;
                }

                expr_free(codeFirstStage->ou_expr);
                codeFirstStage->ou_expr = expr_add_new(expr_data_new(x), expr_data_new(y));

                libbf_interpreted_code_list_free_elt(listRef);

                return TRUE;
              }
            }
            else if (expr_ref_offset(code->ou_expr, x))
            {
              return FALSE;
            }
          }
        }
        else
        {
          if (code->ou_offset == x || code->ou_offset == y)
            return FALSE;
        }

        break;
      }

      default:
        break;
    }

    list = list->next;
  }
  return FALSE;
  
}

static int libbf_optimize_x_pluseq_y_y_eq_x_x_eq_stuff(List** p_list)
{
  List* list = *p_list;

  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
      {
        if (code->ou_expr->e_type == EXPR_ADD &&
            code->ou_expr->e_expr1->e_type == EXPR_DATA &&
            code->ou_expr->e_expr2->e_type == EXPR_DATA &&
            code->ou_expr->e_expr2->e_offset == code->ou_offset)
        {
          List* next = list->next;
          has_done_changes |= libbf_optimize_x_pluseq_y_y_eq_x_x_eq_stuff_internal(list, p_list);
          list = next;
          continue;
        }
        break;
      }

      default:
        break;
    }

    list = list->next;
  }

  return has_done_changes;
}


static int libbf_optimize_can_remove_following_affects_internal(List* list)
{

  OptimizationUnit* code = (OptimizationUnit*) list->data;
  OFFSET_t interest_offset = code->ou_offset;

  int level = 0;

  list = list->next;
  while(list)
  {
    code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
      {
        level++;
        break;
      }
      
      case BF_END_WHILE:
      {
        level--;
        if (level < 0)
        {
          return FALSE;
        }
        break;
      }
      
      case BF_INC_DATA_PTR:
      {
        return FALSE;
        break;
      }

      case BF_PRINT_DATA:
      {
        if (code->ou_offset == interest_offset)
        {
          return FALSE;
        }
        break;
      }

      case BF_READ_DATA:
      {
        if (level == 0 && code->ou_offset == interest_offset)
        {
          return TRUE;
        }
        break;
      }

      case BF_AFFECT_EXPR:
      {
        if (level == 0)
        {
          if (code->ou_offset == interest_offset)
          {
            return !expr_ref_offset(code->ou_expr, code->ou_offset);
          }
          else if (expr_ref_offset(code->ou_expr, interest_offset))
          {
            return FALSE;
          }
        }
        else
        {
          if (code->ou_offset == interest_offset ||
              expr_ref_offset(code->ou_expr, interest_offset))
          {
            return FALSE;
          }
        }
        break;
      }

      default:
        SHOULDNT_HAPPEN();
        break;
    }

    list = list->next;
  }
  
  return FALSE;
}

/* Optimize following seq : */
/* (1) data[x] = f(....) where there is no reference to data[x] in f */
/* (2) data[y] = g(....) where there is no reference to data[x] in g */
/* ... no data_ptr += stuff ... */
/* (n) data[x] = h(...) where there is no reference to data[x] in h */
/* In that case, remove (1) */
static int libbf_optimize_remove_following_affects(List** p_list)
{
  List* list = *p_list;

  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
      {
        if (!expr_ref_offset(code->ou_expr, code->ou_offset))
        {
          if (libbf_optimize_can_remove_following_affects_internal(list))
          {
            List* next = list->next;
            has_done_changes = TRUE;

            if (list == *p_list)
              *p_list = next;
            libbf_interpreted_code_list_free_elt(list);
            list = next;
            continue;
          }
        }
      }
      
      default:
        break;
    }

    list = list->next;
  }

  return has_done_changes;
    
}

static void libbf_add_initial_explicit_zero_add(LibbfHashTable* hash_table_dealt_offset, List** p_list, int offset)
{
  OptimizationUnit* new_code;
  List* list;
  int level = 0;
  
  if (libbf_hash_table_lookup(hash_table_dealt_offset, offset))
  {
    return;
  }
  libbf_hash_table_insert(hash_table_dealt_offset, offset, (void*)1);
  
  list = *p_list;
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_INC_DATA_PTR:
      {
        return;
      }
      
      case BF_WHILE_DATA:
      {
        level++;
        break;
      }
      
      case BF_END_WHILE:
      {
        level--;
        break;
      }
      
      case BF_AFFECT_EXPR:
      {
        if (expr_ref_offset(code->ou_expr, offset))
        {
          list = NULL;
          continue;
        }
        if (level == 0 && code->ou_offset == offset)
        {
          return;
        }
      }
      
      default:
        break;
    }

    list = list->next;
  }
  
  new_code = alloc0(OptimizationUnit, 1);
  new_code->ou_instr = BF_AFFECT_EXPR;
  new_code->ou_offset = offset;
  new_code->ou_expr = expr_constant_new(0);
  *p_list = list_prepend(*p_list, new_code);
}

static void libbf_add_initial_explicit_zero_internal(LibbfHashTable* hash_table_dealt_offset, List** p_list, Expression* expr)
{
  switch(expr->e_type)
  {
    case EXPR_ADD:
    case EXPR_MUL:
      libbf_add_initial_explicit_zero_internal(hash_table_dealt_offset, p_list, expr->e_expr1);
      libbf_add_initial_explicit_zero_internal(hash_table_dealt_offset, p_list, expr->e_expr2);
      break;
      
    case EXPR_CONSTANT:
      break;
      
    case EXPR_DATA:
      libbf_add_initial_explicit_zero_add(hash_table_dealt_offset, p_list, expr->e_offset);
      break;
      
    default:
      SHOULDNT_HAPPEN();
      break;
  }

}

static void libbf_add_initial_explicit_zero(List** p_list)
{
  List* list = *p_list;
  LibbfHashTable* hash_table_dealt_offset = libbf_hash_table_new(NULL);
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_INC_DATA_PTR:
      {
        list = NULL;
        continue;
      }
      
      case BF_AFFECT_EXPR:
      {
        libbf_add_initial_explicit_zero_internal(hash_table_dealt_offset, p_list, code->ou_expr);
        break;
      }
      
      case BF_PRINT_DATA:
      case BF_WHILE_DATA:
      {
        libbf_add_initial_explicit_zero_add(hash_table_dealt_offset, p_list, code->ou_offset);
        break;
      }
      
      default:
        break;
    }

    list = list->next;
  }

  libbf_hash_table_destroy(hash_table_dealt_offset);
}

/* Optimize :
while(data_ptr[3] != 0)
{
  data_ptr[3] = data_ptr[2];
  data_ptr[4] = 5;
  data_ptr[3] = 0;  or while(data_ptr[3]) {  }
  data_ptr[2] += data_ptr[5];
}

into
if(data_ptr[3] != 0)
{
  data_ptr[3] = data_ptr[2];
  data_ptr[4] = 5;
  data_ptr[3] = 0;  or while(data_ptr[3]) { }
  data_ptr[2] += data_ptr[5];
}
*/

static List* libbf_jump_until_end_while(List* list)
{
  OptimizationUnit* code = (OptimizationUnit*) list->data;
  int level = 0;
  ASSERT(code->ou_instr == BF_WHILE_DATA);
  list = list->next;
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
      {
        level++;
        break;
      }
      
      case BF_END_WHILE:
      {
        if (level == 0)
          return list;
        level--;
        break;
      }
      
      default:
        break;
    }
    list = list->next;
  }
  SHOULDNT_HAPPEN();
  return NULL;
}

static int libbf_mark_while_as_if_internal(List* list)
{
  int has_done_changes = FALSE;
  OptimizationUnit* code = (OptimizationUnit*) list->data;
  OptimizationUnit* while_code = code;
  List* list_begin = list;
  OFFSET_t interest_offset = code->ou_offset;
  int level = 1;
  int can_turn_to_if = FALSE;
  
  ASSERT(code->ou_instr == BF_WHILE_DATA);
  list = list->next;
  
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
      {
        if (code->ou_offset == interest_offset)
        {
          can_turn_to_if = TRUE;
          list = libbf_jump_until_end_while(list);
        }
        else
        {
          level++;
        }
        break;
      }
        
      case BF_INC_DATA_PTR:
        return FALSE;
        break;
        
      case BF_END_WHILE:
        level--;
        if (level == 0)
        {
          if (while_code->ou_loop_type == LIBBF_IF)
          {
            if (!can_turn_to_if)
              libbf_interpreter_optimizer_cdump_internal(stderr, list_begin, "", 10);
            
            assert(can_turn_to_if); /* consistency check */
          }
          if (can_turn_to_if && while_code->ou_loop_type == LIBBF_WHILE)
          {
            while_code->ou_loop_type = LIBBF_IF;
            code->ou_loop_type = LIBBF_IF;
            has_done_changes = TRUE;
          }
          return has_done_changes;
        }
        break;

      case BF_AFFECT_EXPR:
        if (code->ou_offset == interest_offset)
        {
          can_turn_to_if = (level == 1 && code->ou_expr->e_type == EXPR_CONSTANT && code->ou_expr->e_constant == 0);
        }
        break;
        
      case BF_READ_DATA:
        if (code->ou_offset == interest_offset)
        {
          can_turn_to_if = FALSE;
        }

      default:
        break;
    }

    list = list->next;
  }

  SHOULDNT_HAPPEN();
  return FALSE;
}

static int libbf_mark_while_as_if(List* list)
{
  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    if(code->ou_instr == BF_WHILE_DATA)
    {
      has_done_changes |= libbf_mark_while_as_if_internal(list);
    }

    list = list->next;
  }
  
  return has_done_changes;
}

static int libbf_mark_while_as_do_internal(List* list, int interest_offset, List** p_patchNext)
{
  int level = 0;
  List* while_list = NULL;
  OptimizationUnit* while_code = NULL;
  int last_affect_is_0 = FALSE;
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_INC_DATA_PTR:
        if (while_code == NULL) return FALSE;
        last_affect_is_0 = FALSE;
        break;
        
      case BF_READ_DATA:
      case BF_AFFECT_EXPR:
        if (while_code == NULL && code->ou_offset == interest_offset) return FALSE;
        if (code->ou_offset == interest_offset)
          last_affect_is_0 = (code->ou_instr == BF_AFFECT_EXPR &&
                              code->ou_expr->e_type == EXPR_CONSTANT &&
                              code->ou_expr->e_constant == 0 && level == 1);
        break;
        
      case BF_WHILE_DATA:
        if (code->ou_offset == interest_offset && level == 0)
        {
          while_list = list;
          while_code = code;
        }
        level++;
        break;
        
      case BF_END_WHILE:
        level--;
        if (code->ou_offset == interest_offset && level == 0)
        {
          if (last_affect_is_0)
          {
            if (while_list == list)
              *p_patchNext = while_list->next;
            libbf_interpreted_code_list_free_elt(while_list);
            libbf_interpreted_code_list_free_elt(list);
            return TRUE;
          }
          else if (while_code->ou_loop_type != LIBBF_DO_WHILE)
          {
            while_code->ou_loop_type = LIBBF_DO_WHILE;
            code->ou_loop_type = LIBBF_DO_WHILE;
            return TRUE;
          }
          else
          {
            return FALSE;
          }
        }
        if (level < 0) return FALSE;
        break;
        
      default:
        break;
    }

    list = list->next;
  }
  
  return FALSE;
}

static int libbf_mark_while_as_do(List* list)
{
  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
        if (code->ou_expr->e_type == EXPR_CONSTANT && code->ou_expr->e_constant != 0)
        {
          List* patch_next = NULL;
          has_done_changes |= libbf_mark_while_as_do_internal(list->next, code->ou_offset, &patch_next);
          if (patch_next)
          {
            list = patch_next;
            continue;
          }
        }
        break;
        
      case BF_WHILE_DATA:
      {
        List* patch_next = NULL;
        has_done_changes |=  libbf_mark_while_as_do_internal(list->next, code->ou_offset, &patch_next);
        if (patch_next)
        {
          list = patch_next;
          continue;
        }
        break;
      }
        
      default:
        break;
    }

    list = list->next;
  }
  
  return has_done_changes;
}


/* Optimize :
  data_ptr[x] = 0;
  ....
  while(data_ptr[x])
  {
  }
*/

static int libbf_remove_useless_loop_internal(List* list, int interest_offset)
{
  OptimizationUnit* code;
  int level = 0;
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
      {
        if (level == 0 && code->ou_offset == interest_offset)
        {
          while(list)
          {
            List* next = list->next;
            int instr;
            code = (OptimizationUnit*) list->data; 
            instr = code->ou_instr;
            libbf_interpreted_code_list_free_elt(list);
            
            if (instr == BF_WHILE_DATA)
            {
              level++;
            }
            else if (instr == BF_END_WHILE)
            {
              level--;
              if (level == 0) return TRUE;
            }
            
            list = next;
          }
        }
        level++;
        break;
      }
      case BF_END_WHILE:
      {
        level--;
        if (level < 0)
          return FALSE;
        break;
      }
      
      case BF_READ_DATA:
      case BF_AFFECT_EXPR:
      {
        if (code->ou_offset == interest_offset)
          return FALSE;
        break;
      }
        
      case BF_INC_DATA_PTR:
        return FALSE;
        break;
        
      case BF_PRINT_DATA:
        break;
      
      default:
        SHOULDNT_HAPPEN();
        break;
    }

    list = list->next;
  }
  
  return FALSE;
}


static int libbf_remove_useless_loop(List* list)
{
  OptimizationUnit* code;
  int has_done_changes = FALSE;
  
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {

      case BF_AFFECT_EXPR:
        if (code->ou_expr->e_type == EXPR_CONSTANT && code->ou_expr->e_constant == 0)
        {
          has_done_changes |= libbf_remove_useless_loop_internal(list->next, code->ou_offset);
        }
        break;

      default:
        break;
    }

    list = list->next;
  }

  return has_done_changes;
}



/* Optimize :
  data_ptr[x] = cst;
  ....
  do
  {
    data_ptr[x] -= cst;
  } while(data_ptr[x]);
*/

static int libbf_remove_useless_do_loop_internal(List* list, int interest_offset, int constant)
{
  OptimizationUnit* code;
  int level = 0;
  int may_remove = FALSE;
  List* list_while = NULL;
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
      {
        level++;
        if (level == 1 && code->ou_offset == interest_offset && code->ou_loop_type == LIBBF_DO_WHILE)
        {
          if (list_while) return FALSE;
          list_while = list;
        }
        break;
      }
      case BF_END_WHILE:
      {
        if (level == 1 && code->ou_offset == interest_offset && code->ou_loop_type == LIBBF_DO_WHILE)
        {
          ASSERT(list_while != NULL);
          libbf_interpreted_code_list_free_elt(list);
          libbf_interpreted_code_list_free_elt(list_while);
          return TRUE;
        }
        level--;
        if (level < 0)
          return FALSE;
        break;
      }
      
      case BF_READ_DATA:
      {
        if (code->ou_offset == interest_offset)
        return FALSE;
        
        break;
      }

      case BF_AFFECT_EXPR:
      {
        if (code->ou_offset == interest_offset)
        {
          if (may_remove)
          {
            if (!(code->ou_expr->e_type == EXPR_CONSTANT && 
                  code->ou_expr->e_constant == 0))
              return FALSE;
          }
          else
          {
            if (level != 1) return FALSE;
            may_remove =  (code->ou_expr->e_type == EXPR_ADD &&
                          code->ou_expr->e_expr1->e_type == EXPR_CONSTANT &&
                          code->ou_expr->e_expr1->e_constant == -constant &&
                          code->ou_expr->e_expr2->e_type == EXPR_DATA &&
                          code->ou_expr->e_expr2->e_offset == interest_offset) ||
                          (code->ou_expr->e_type == EXPR_ADD &&
                          code->ou_expr->e_expr2->e_type == EXPR_CONSTANT &&
                          code->ou_expr->e_expr2->e_constant == -constant &&
                          code->ou_expr->e_expr1->e_type == EXPR_DATA &&
                          code->ou_expr->e_expr1->e_offset == interest_offset);
            if (!may_remove) return FALSE;
          }
        }
        break;
      }
        
      case BF_INC_DATA_PTR:
        return FALSE;
        break;
        
      case BF_PRINT_DATA:
        break;
      
      default:
        SHOULDNT_HAPPEN();
        break;
    }

    list = list->next;
  }
  
  return FALSE;
}



static int libbf_remove_useless_do_loop(List* list)
{
  OptimizationUnit* code;
  int has_done_changes = FALSE;
  
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {

      case BF_AFFECT_EXPR:
        if (code->ou_expr->e_type == EXPR_CONSTANT)
        {
          has_done_changes |= libbf_remove_useless_do_loop_internal(list->next, code->ou_offset, code->ou_expr->e_constant);
        }
        break;

      default:
        break;
    }

    list = list->next;
  }

  return has_done_changes;
}


/* Optimize :
while(data_ptr[7] != 0)
{
  data_ptr[6] = 1;
  data_ptr[3] = data_ptr[4] + 1;
  data_ptr[2] += data_ptr[5];
   But not data_ptr[2] += data_ptr[6]
   or data_ptr[2] += data_ptr[2] * 3
  data_ptr[7] += -1;
}

into
if(data_ptr[7] != 0)
{
  data_ptr[6] = 1;
  data_ptr[3] = data_ptr[4] + 1;
  data_ptr[2] += data_ptr[5] * data_ptr[7];
  data_ptr[7] = 0;
}
*/

static int libbf_transform_while_loop_into_if_internal(List* list)
{
  OptimizationUnit* code = (OptimizationUnit*) list->data;
  
  OFFSET_t interest_offset = code->ou_offset;
  CONSTANT_t count_increment = 0;
  int ret = FALSE;
    
  List* offset_list_left = NULL;
  List* offset_list_right = NULL;
    
  assert(code->ou_instr == BF_WHILE_DATA);

  list = list->next;
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        SHOULDNT_HAPPEN();
        break;

      case BF_END_WHILE:
        ret = count_increment == -1 && !list_intersects(offset_list_left, offset_list_right);
        goto end_libbf_transform_while_loop_into_if_internal;
        break;

      case BF_AFFECT_EXPR:
        if (code->ou_offset == interest_offset)
        {
          offset_list_left = list_prepend(offset_list_left, OFFSET_TO_POINTER(code->ou_offset));

          if (!is_inc_expr_by_constant(code->ou_expr, code->ou_offset)) /* data[offset] = constant + data[offset] */
          {
            goto end_libbf_transform_while_loop_into_if_internal;
          }
          count_increment += code->ou_expr->e_expr1->e_constant;
        }
        else
        {
          if (list_find(offset_list_left, OFFSET_TO_POINTER(code->ou_offset)))
          {
            goto end_libbf_transform_while_loop_into_if_internal;
          }

          offset_list_left = list_prepend(offset_list_left, OFFSET_TO_POINTER(code->ou_offset));

          /* If expression reference dest offset, assure that it is a simple data[offset] += f(...) */
          /* without any reference to data[offset] in f */
          if (expr_ref_offset(code->ou_expr, code->ou_offset))
          {
            if (!(code->ou_expr->e_type == EXPR_ADD &&
                  code->ou_expr->e_expr2->e_type == EXPR_DATA &&
                  code->ou_expr->e_expr2->e_offset == code->ou_offset &&
                  !expr_ref_offset(code->ou_expr->e_expr1, code->ou_offset)))
            {
              goto end_libbf_transform_while_loop_into_if_internal;
            }
          }
          
          offset_list_right = expr_get_list_of_ref_internal_but_ignore_offset(code->ou_expr, offset_list_right, code->ou_offset);
        }
        break;

        
      case BF_INC_DATA_PTR:
      case BF_PRINT_DATA:
      case BF_READ_DATA:
        goto end_libbf_transform_while_loop_into_if_internal;
        break;

      default:
        SHOULDNT_HAPPEN();
        break;
    }

    list = list->next;
  }

  SHOULDNT_HAPPEN();
  
end_libbf_transform_while_loop_into_if_internal:
  list_free(offset_list_left, NULL);
  list_free(offset_list_right, NULL);

  return ret;
}

static int libbf_transform_while_loop_into_if(List* list)
{
  int has_done_changes = FALSE;
  OFFSET_t interest_offset = -1;
  int can_optimize = FALSE;
  
  List* start_list = NULL;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;

    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        if (libbf_is_innerloop(list) && libbf_transform_while_loop_into_if_internal(list))
        {
          can_optimize = TRUE;
          
          start_list = list;
          
          interest_offset = code->ou_offset;
          
          has_done_changes = TRUE;
          code->ou_loop_type = LIBBF_IF;

          if (DEBUG)
          {
            fprintf(stderr, "\nbefore_optimize :\n");
            libbf_interpreter_optimizer_cdump_internal(stderr, list, "", 10);
          }
        }
        break;

      case BF_END_WHILE:
        if (can_optimize)
        {
          OptimizationUnit* new_code = alloc0(OptimizationUnit, 1);
          new_code->ou_instr  = BF_AFFECT_EXPR;
          new_code->ou_offset = interest_offset;
          new_code->ou_expr   = expr_constant_new(0);
          list_insert_before(list, new_code);
          
          code->ou_loop_type = LIBBF_IF;

          if (DEBUG)
          {
            fprintf(stderr, "\nafter_optimize :\n");
            libbf_interpreter_optimizer_cdump_internal(stderr, start_list, "", 10);
          }
                    
          can_optimize = FALSE;
        }
        break;

      case BF_AFFECT_EXPR:
        if (can_optimize)
        {
          if (code->ou_offset == interest_offset)
          {
            /* Remove BF_AFFECT_EXPR instr */
            List* next = list->next;
            libbf_interpreted_code_list_free_elt(list);
            list = next;
            continue;
          }
          else
          {
            if (code->ou_expr->e_type == EXPR_ADD &&
                  code->ou_expr->e_expr2->e_type == EXPR_DATA &&
                  code->ou_expr->e_expr2->e_offset == code->ou_offset &&
                  !expr_ref_offset(code->ou_expr->e_expr1, code->ou_offset))
            {
              Expression* new_expr;
              code->ou_expr->e_expr1 = expr_mul_new(code->ou_expr->e_expr1, expr_data_new(interest_offset));
              new_expr = expr_simplify(code->ou_expr);
              expr_free(code->ou_expr);
              code->ou_expr = new_expr;
            }
            else
            {
            }
          }
        }
        break;

      default:
        break;
    }

    list = list->next;
  }

  return has_done_changes;
}

/* Optimize following seq :
data_ptr[4] = 0;
while(data_ptr[2] != 0)
{
  data_ptr[2] += -1;
  data_ptr[5] += data_ptr[3];
  data_ptr[4] += data_ptr[3];
  data_ptr[3] = data_ptr[4];
  data_ptr[4] = 0;
}
*/

static int libbf_optimize_can_propagate_affect_in_loop(List* list, List** p_while_list)
{
  OptimizationUnit* code_affect = (OptimizationUnit*)list->data;
  
  int in_loop = FALSE;
  OFFSET_t while_offset = -1;
  CONSTANT_t while_inc = 0;
  int interesting_affect = FALSE;
  int interesting_inc = FALSE;

  list = list->next;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_INC_DATA_PTR:
        return FALSE;
        break;
        
      case BF_WHILE_DATA:
        if (in_loop)
          return FALSE;
        in_loop = TRUE;
        *p_while_list = list;
        while_offset = code->ou_offset;
        if (while_offset == code_affect->ou_offset)
          return FALSE; /* FIXME? : maybe too restrictive */
        break;
        
      case BF_END_WHILE:
        return (while_inc == -1 && interesting_affect && interesting_inc);
        
      case BF_READ_DATA:
        if (code->ou_offset == code_affect->ou_offset)
          return FALSE;
        break;
        
      case BF_AFFECT_EXPR:
        if (in_loop)
        {
          if (code->ou_offset == while_offset)
          {
            if (!is_inc_expr_by_constant(code->ou_expr, code->ou_offset))
              return FALSE;
            while_inc += code->ou_expr->e_expr1->e_constant;
          }
          else if (code->ou_offset == code_affect->ou_offset)
          {
            if (code->ou_expr->e_type == EXPR_ADD &&
                code->ou_expr->e_expr2->e_type == EXPR_DATA &&
                code->ou_expr->e_expr2->e_offset == code->ou_offset &&
                !expr_ref_offset(code->ou_expr->e_expr1, code->ou_offset))
            {
              interesting_affect = FALSE;
              interesting_inc = TRUE;
            }
            else if (code->ou_expr->e_type == EXPR_CONSTANT &&
                    code->ou_expr->e_constant == code_affect->ou_expr->e_constant)
            {
              interesting_affect = TRUE;
            }
            else
            {
              interesting_affect = FALSE;
              interesting_inc = FALSE;
            }
          }
        }
        else
        {
          if (code->ou_offset == code_affect->ou_offset)
            return FALSE;
        }
        break;
        
      default:
        break;
    }
    
    list = list->next;
  }
  
  return FALSE;
}

static int libbf_optimize_propagate_affect_in_loop(List* list)
{
  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
        if (!expr_ref_offset(code->ou_expr, code->ou_offset))
        {
          /* for the moment, let's deal only with constants */
          if (code->ou_expr->e_type == EXPR_CONSTANT)
          {
            List* while_list = NULL;
            if (libbf_optimize_can_propagate_affect_in_loop(list, &while_list))
            {
              OptimizationUnit* new_code;
              has_done_changes = TRUE;
              
              if (DEBUG)
              {
                fprintf(stderr, "\nbefore_optimize :\n");
                libbf_interpreter_optimizer_cdump_internal(stderr, list, "", 10);
              }
              new_code = alloc0(OptimizationUnit, 1);
              new_code->ou_instr = BF_AFFECT_EXPR;
              new_code->ou_offset = code->ou_offset;
              new_code->ou_expr = expr_dup(code->ou_expr);
              list_insert_before(while_list->next, new_code);
              if (DEBUG)
              {
                fprintf(stderr, "\nafter_optimize :\n");
                libbf_interpreter_optimizer_cdump_internal(stderr, list, "", 10);
              }              
            }
          }
          
        }
        break;
        
      default:
        break;
    }
    
    list = list->next;
  }
  
  return has_done_changes;
}


/* Optimize :
data_ptr[x] = cst
if (data_ptr[y])
{
  data_ptr[x] = f(data_ptr[x], ....)
}
*/

static int libbf_optimize_expr_through_if_int(List* list)
{
  OptimizationUnit* code_affect = (OptimizationUnit*)list->data;
  int interest_offset = code_affect->ou_offset;
  int level = 0;
  int inIf = FALSE;
  
  list = list->next;
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        level++;
        inIf = FALSE;
        if (level == 1 && code->ou_loop_type == LIBBF_IF)
        {
          inIf = TRUE;
        }
        break;

      case BF_END_WHILE:
        level--;
        inIf = FALSE;
        if (level < 0)
        {
          return FALSE;
        }
        break;
        
      case BF_READ_DATA:
        if (code->ou_offset == interest_offset)
        {
          return FALSE;
        }
        break;
              
      case BF_AFFECT_EXPR:
        if (code->ou_offset == interest_offset)
        {
          if (inIf && expr_ref_offset(code->ou_expr, interest_offset))
          {
            Expression* new_expr;
            expr_subst_offset_by_expr(code->ou_expr, interest_offset, code_affect->ou_expr);
            new_expr = expr_simplify(code->ou_expr);
            expr_free(code->ou_expr);
            code->ou_expr = new_expr;
            return TRUE;
          }
          else
          {
            return FALSE;
          }
        }
        break;
          
      default:
        break;
    }
    list = list->next;
  }
  return FALSE;
}

static int libbf_optimize_expr_through_if(List* list)
{
  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
        if (code->ou_expr->e_type == EXPR_CONSTANT)
        {
          has_done_changes |= libbf_optimize_expr_through_if_int(list);
        }
        break;
        
      default:
        break;
    }
    list = list->next;
  }
  
  return has_done_changes;
}



/* Optimize :
data_ptr[2] = cst
while(data_ptr[3] != 0)
{
  ...
  data_ptr[2] = cst;
  ..
}
*/

static int libbf_optimize_remove_useless_affect_in_loop_int(List* list)
{
  OptimizationUnit* code = (OptimizationUnit*) list->data;
  OFFSET_t interest_offset = code->ou_offset;
  CONSTANT_t interest_constant = code->ou_expr->e_constant;
  int ret = FALSE;
  List* removable_list = NULL;
  
  int level = 0;
  
  list = list->next;
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        level++;
        break;

      case BF_END_WHILE:
        level--;
        if (level < 0)
        {
          return FALSE;
        }
        else if (level == 0)
        {
          if (removable_list != NULL)
          {
            libbf_interpreted_code_list_free_elt(removable_list);
            removable_list = NULL;
            ret = TRUE;
          }
        }
        break;

      case BF_INC_DATA_PTR:
        return ret;
        break;

      case BF_READ_DATA:
        if (code->ou_offset == interest_offset)
        {
          return ret;
        }
        break;

      case BF_AFFECT_EXPR:
        if (level == 0)
        {
          if (code->ou_offset == interest_offset)
          {
            return ret;
          }
        }
        else
        {
          if (code->ou_offset == interest_offset)
          {
            if (code->ou_expr->e_type == EXPR_CONSTANT &&
                code->ou_expr->e_constant == interest_constant)
            {
              removable_list = list;
            }
            else
            {
              return ret;
            }
          }
        }
        break;
        
      default:
        break;
    }
    list = list->next;
  }

  return ret;
}

static int libbf_optimize_remove_useless_affect_in_loop(List* list)
{
  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
        if (code->ou_expr->e_type == EXPR_CONSTANT)
        {
          has_done_changes |= libbf_optimize_remove_useless_affect_in_loop_int(list);
        }
        break;
        
      default:
        break;
    }
    list = list->next;
  }
  
  return has_done_changes;
}


/* Optimize :
while(data_ptr[2] != 0)
{
  ...
}
data_ptr[2] = 0 or data_ptr[x] = f(data_ptr[2], ...) or while(data_ptr[2] != 0) {}
*/

static void libbf_delete_loop(List* list)
{
  OptimizationUnit* code = (OptimizationUnit*) list->data;
  int level = 0;
  ASSERT(code->ou_instr == BF_WHILE_DATA);
  while(list)
  {
    List* next = list->next;
    int code_ou_instr = ((OptimizationUnit*) list->data)->ou_instr;
    libbf_interpreted_code_list_free_elt(list);
    
    switch(code_ou_instr)
    {
      case BF_WHILE_DATA:
        level++;
        break;
        
      case BF_END_WHILE:
        level--;
        if (level == 0) return;
        break;
        
      default:
        break;
    }
    list = next;
  }
}
static int libbf_optimize_propagate_while_zero_after_loop_int(List* list)
{
  OptimizationUnit* code = (OptimizationUnit*) list->data;
  OFFSET_t interest_offset = code->ou_offset;
  int ret = FALSE;
  int level = 1;
  int nbEndLevel0 = 0;
  
  list = list->next;
  while(list)
  {
    code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        if (level == 0 && nbEndLevel0 > 0 && code->ou_offset == interest_offset)
        {
          List* prev = list->prev;
          libbf_delete_loop(list);
          list = prev->next;
          ret = TRUE;
          continue;
        }
        level++;
        break;
        
      case BF_END_WHILE:
        level--;
        if (level < 0)
        {
          return ret;
        }
        if (level == 0)
        {
          nbEndLevel0++;
        }
        break;
      
      case BF_INC_DATA_PTR:
        /* the following case is ok :
        while(data_ptr[2] != 0)
        {
          ...
          data_ptr += foo
        }
        data_ptr[2] += stuff
        */
        if (nbEndLevel0 != 0)
          return ret;
        break;
        
      case BF_READ_DATA:
        if (nbEndLevel0 > 0 && code->ou_offset == interest_offset)
        {
          return ret;
        }
        break;

      case BF_AFFECT_EXPR:
        if (code->ou_offset == interest_offset)
        {
          if (level == 0)
          {
            if (code->ou_expr->e_type == EXPR_CONSTANT && code->ou_expr->e_constant == 0)
            {
              List* next = list->next;
              libbf_interpreted_code_list_free_elt(list);
              list = next;
              ret = TRUE;
              continue;
            }
            else if (expr_ref_offset(code->ou_expr, interest_offset))
            {
              Expression* subst_expr = expr_constant_new(0);
              Expression* new_expr;
              expr_subst_offset_by_expr(code->ou_expr, interest_offset, subst_expr);
              expr_free(subst_expr);
              new_expr = expr_simplify(code->ou_expr);
              expr_free(code->ou_expr);
              code->ou_expr = new_expr;
              return TRUE;
            }
            else
            {
              return ret;
            }
          }
          else if (nbEndLevel0 > 0)
          {
            if (!(code->ou_expr->e_type == EXPR_CONSTANT && code->ou_expr->e_constant == 0))
              return ret;
          }
        }
        else if (level == 0 && expr_ref_offset(code->ou_expr, interest_offset))
        {
          Expression* subst_expr = expr_constant_new(0);
          Expression* new_expr;
          expr_subst_offset_by_expr(code->ou_expr, interest_offset, subst_expr);
          expr_free(subst_expr);
          new_expr = expr_simplify(code->ou_expr);
          expr_free(code->ou_expr);
          code->ou_expr = new_expr;
          ret = TRUE;
        }
        break;
        
      default:
        break;
    }
    list = list->next;
  }
  
  return ret;
}

static int libbf_optimize_propagate_while_zero_after_loop(List* list)
{
  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_WHILE_DATA:
        has_done_changes |= libbf_optimize_propagate_while_zero_after_loop_int(list);
        break;
        
      default:
        break;
    }
    list = list->next;
  }
  
  return has_done_changes;
}

/* Optimize
   data_ptr[y] = 0;
   ...no data_ptr[y]=...
   if (data_ptr[x])
   {
     ...no data_ptr[x]=...
     data_ptr[y] = data_ptr[x]
     ...no data_ptr[y]=...
   }
*/

static int libbf_optimize_propagate_affect_outside_if_int(List* list)
{
  OptimizationUnit* code_affect = (OptimizationUnit*) list->data;
  int interest_offset = code_affect->ou_offset;
  int interest_offset_if = -100000000;
  int level = 0;
  int can_optimize = FALSE;
  List* list_in_if = NULL;
  
  list = list->next;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
        if (code->ou_offset == interest_offset)
        {
          if (level == 0)
            return FALSE;
          else if (!can_optimize)
          {
            ASSERT(interest_offset_if != -100000000);
            if (code->ou_expr->e_type == EXPR_DATA &&
                code->ou_expr->e_offset == interest_offset_if)
            {
              list_in_if = list;
              can_optimize = TRUE;
            }
            else
              return FALSE;
          }
          else
            return FALSE;
        }
        else if (level != 0 && !can_optimize)
        {
          ASSERT(interest_offset_if != -100000000);
          if (code->ou_offset == interest_offset_if)
            return FALSE;
        }
        break;
        
      case BF_READ_DATA:
        return FALSE;
        
      case BF_WHILE_DATA:
        if (level == 0)
        {
          if (code->ou_loop_type == LIBBF_IF)
          {
            interest_offset_if = code->ou_offset;
          }
          else
          {
            return FALSE;
          }
        }
        level++;
        break;

      case BF_END_WHILE:
        level--;
        if (level < 0)
          return FALSE;
        else if (level == 0)
        {
          if (can_optimize)
          {
            code_affect->ou_expr->e_type = EXPR_DATA;
            code_affect->ou_expr->e_offset = interest_offset_if;
            libbf_interpreted_code_list_free_elt(list_in_if);
          }
          return can_optimize;
        }
        break;

      default:
        break;
    }
    list = list->next;
  }
  return FALSE;
}
  
static int libbf_optimize_propagate_affect_outside_if(List* list)
{
  int has_done_changes = FALSE;
  
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
        if (code->ou_expr->e_type == EXPR_CONSTANT && code->ou_expr->e_constant == 0)
          has_done_changes |= libbf_optimize_propagate_affect_outside_if_int(list);
        break;
        
      default:
        break;
    }
    list = list->next;
  }
  
  return has_done_changes;
}

typedef enum
{
  BF_OPTIMIZE_INIT_EXPLICIT_ZERO = 1 << 0,
  BF_OPTIMIZE_GROUP_EXPRESSIONS = 1 << 1,
  BF_OPTIMIZE_LOOPS = 1 << 2,
  BF_OPTIMIZE_MARK_WHILE_AS_IF = 1 << 3,
  BF_OPTIMIZE_MARK_WHILE_AS_DO = 1 << 4,
  
  BF_OPTIMIZE_ALL = 0xFFFFFFFF,
} BfOptimizationsFlags;


static void dump_in_file(List* list, int global_pass, int pass, char* comment)
{
  /* fprintf(stderr, "%d %d %s\n", global_pass, pass, comment); */
  /*
  while (list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
      {
        if (code->ou_expr->e_type == EXPR_ADD &&
            code->ou_expr->e_expr1->e_type == EXPR_CONSTANT &&
            code->ou_expr->e_expr2->e_type == EXPR_CONSTANT)
        {
          fprintf(stderr, "%s\n", comment);
          abort();
        }
        break;
      }
      
      default:
        break;
    }
    list = list->next;
  }
  */
  /*
    char buf[512];
    sprintf(buf, "dump%d_%d_%s.txt", global_pass, pass, comment);
    FILE* f = fopen(buf, "w");
    libbf_interpreter_optimizer_cdump_internal(f, list, "int", -1);
    fclose(f);
  */
  return;
}

static void libbf_optimize(List** p_list, const char* data_type, BfOptimizationsFlags flags)
{
  int ret;
  int simple_ret;
  int pass = 0;
  int global_pass = 0;

  libbf_very_simple_optimization(*p_list);
  
  if (DEBUG) libbf_interpreter_optimizer_cdump_internal(stderr, *p_list, data_type, -1);
  
  libbf_optimize_remove_inc_data_ptr(p_list);

  if (DEBUG) libbf_interpreter_optimizer_cdump_internal(stderr, *p_list, data_type, -1);

  if (flags & BF_OPTIMIZE_INIT_EXPLICIT_ZERO)
    libbf_add_initial_explicit_zero(p_list);
    
  while(TRUE)
  {
    ++global_pass;
    if (DEBUG) fprintf(stderr, "----------------------------------------\nNew global_pass (%d)\n", global_pass);
    while(TRUE)
    {
      ++pass;
      if (DEBUG) fprintf(stderr, "----------------------------------------\nNew pass (%d)\n", pass);
      /*if (pass == 30)
      {
        fprintf(stderr, "Warning : 30 optimization passes ! That's a bit unusual. Probably too much. Let's stop that now\n");
        return;
      }*/
            
      if (flags & BF_OPTIMIZE_LOOPS)
      {
        simple_ret = libbf_transform_while_loop_into_mul_expr(p_list);
        ret = simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_transform_while_loop_into_mul_expr");
      
        simple_ret = libbf_transform_while_loop_into_if(*p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_transform_while_loop_into_if");
        
        simple_ret = libbf_optimize_remove_useless_affect_in_loop(*p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_remove_useless_affect_in_loop");

        simple_ret = libbf_remove_useless_loop(*p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_remove_useless_loop");
        
        simple_ret = libbf_remove_useless_do_loop(*p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_remove_useless_do_loop");

        simple_ret = libbf_optimize_propagate_affect_in_loop(*p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_propagate_affect_in_loop");
        
        simple_ret = libbf_optimize_propagate_affect_outside_if(*p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_propagate_affect_outside_if");
        
        simple_ret = libbf_optimize_propagate_while_zero_after_loop(*p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_propagate_while_zero_after_loop");

        if (ret && (flags & BF_OPTIMIZE_INIT_EXPLICIT_ZERO))
          libbf_add_initial_explicit_zero(p_list);
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_add_initial_explicit_zero");
      }
      else
      {
        ret = 0;
      }
      
      if (flags & BF_OPTIMIZE_GROUP_EXPRESSIONS)
      {
        simple_ret = libbf_optimize_remove_following_affects(p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_remove_following_affects");
        
        simple_ret = libbf_optimize_x_eq_y_z_fct_x(p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_x_eq_y_z_fct_x");
        
        simple_ret = libbf_optimize_x_pluseq_y_y_eq_x_x_eq_stuff(p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_x_pluseq_y_y_eq_x_x_eq_stuff");
        
        simple_ret = libbf_optimize_expr_through_if(*p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_expr_through_if");
  
        simple_ret = libbf_optimize_group_expr(p_list);
        ret |= simple_ret;
        if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_optimize_group_expr");
      }
      
      if (!ret) break;
  
    }
    ret = 0;
    
    if (flags && BF_OPTIMIZE_MARK_WHILE_AS_IF)
    {
      simple_ret = libbf_mark_while_as_if(*p_list);
      ret |= simple_ret;
      if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_mark_while_as_if");
    }
    
    if (flags && BF_OPTIMIZE_MARK_WHILE_AS_DO)
    {
      simple_ret = libbf_mark_while_as_do(*p_list);
      ret |= simple_ret;
      if (simple_ret) dump_in_file(*p_list, global_pass, pass, "libbf_mark_while_as_do");
    }
    
    if (!ret) break;
    
  }

}


typedef enum
{
  FOR_INTERPRETER,
  FOR_COMPILER,
} BackEnd_t;

static void libbf_optimizer_transform_generic_expr_to_particular_expr(List* list, BackEnd_t backEnd)
{
  while(list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    if(code->ou_instr == BF_AFFECT_EXPR)
    {
      Expression* expr = code->ou_expr;
      if (expr->e_type == EXPR_CONSTANT)
      {
        OFFSET_t constant = expr->e_constant;
        expr_free(expr);
        code->ou_constant = constant;
        if (constant == 0 && backEnd == FOR_INTERPRETER)
          code->ou_instr = BF_SET_CST_0;
        else
          code->ou_instr = BF_SET_CST;
      }
      else if (is_inc_expr_by_constant(expr, code->ou_offset))
      {
        CONSTANT_t constant = expr->e_expr1->e_constant;
        expr_free(expr);
        code->ou_constant = constant;
        if (constant == 1 && backEnd == FOR_INTERPRETER)
          code->ou_instr = BF_INC_CST_1;
        else if (constant == -1 && backEnd == FOR_INTERPRETER)
          code->ou_instr = BF_INC_CST_M_1;
        else
          code->ou_instr = BF_INC_CST;
      }
      else if (expr->e_type == EXPR_DATA)
      {
        OFFSET_t offset2 = expr->e_offset;
        expr_free(expr);
        code->ou_instr = BF_SET_DATA;
        code->ou_offset2 = offset2;
      }
      else if (expr->e_type == EXPR_ADD &&
              expr->e_expr1->e_type == EXPR_DATA &&
              expr->e_expr2->e_type == EXPR_DATA &&
              expr->e_expr2->e_offset == code->ou_offset)
      {
        OFFSET_t offset2 = expr->e_expr1->e_offset;
        expr_free(expr);
        code->ou_instr = BF_INC_DATA;
        code->ou_offset2 = offset2;
      }
      else if (expr->e_type == EXPR_ADD &&
              expr->e_expr1->e_type == EXPR_ADD &&
              expr->e_expr1->e_expr1->e_type == EXPR_DATA &&
              expr->e_expr1->e_expr2->e_type == EXPR_CONSTANT &&
              expr->e_expr2->e_type == EXPR_DATA &&
              expr->e_expr2->e_offset == code->ou_offset)
      {
        CONSTANT_t constant = expr->e_expr1->e_expr2->e_constant;
        OFFSET_t offset2 = expr->e_expr1->e_expr1->e_offset;
        expr_free(expr);
        code->ou_instr = BF_INC_CST_ADD_DATA;
        code->ou_constant = constant;
        code->ou_offset2 = offset2;
      }
      else if (expr->e_type == EXPR_ADD &&
              expr->e_expr1->e_type == EXPR_DATA &&
              expr->e_expr2->e_type == EXPR_ADD &&
              expr->e_expr2->e_expr1->e_type == EXPR_CONSTANT &&
              expr->e_expr2->e_expr2->e_type == EXPR_DATA &&
              expr->e_expr2->e_expr2->e_offset == code->ou_offset)
      {
        CONSTANT_t constant = expr->e_expr2->e_expr1->e_constant;
        OFFSET_t offset2 = expr->e_expr1->e_offset;
        expr_free(expr);
        code->ou_instr = BF_INC_CST_ADD_DATA;
        code->ou_constant = constant;
        code->ou_offset2 = offset2;
      }
      else if (expr->e_type == EXPR_ADD &&
              expr->e_expr1->e_type == EXPR_MUL &&
              expr->e_expr1->e_expr1->e_type == EXPR_CONSTANT &&
              expr->e_expr1->e_expr2->e_type == EXPR_DATA &&
              expr->e_expr2->e_type == EXPR_DATA &&
              expr->e_expr2->e_offset == code->ou_offset)
      {
        CONSTANT_t constant = expr->e_expr1->e_expr1->e_constant;
        OFFSET_t offset2 = expr->e_expr1->e_expr2->e_offset;
        expr_free(expr);
        if (constant == -1)
        {
          code->ou_instr = BF_DEC_DATA;
        }
        else
        {
          code->ou_instr = BF_INC_CST_MUL_DATA;
          code->ou_constant = constant;
        }
        code->ou_offset2 = offset2;
      }
      else if (expr->e_type == EXPR_ADD &&
              expr->e_expr1->e_type == EXPR_MUL &&
              expr->e_expr1->e_expr1->e_type == EXPR_DATA &&
              expr->e_expr1->e_expr2->e_type == EXPR_DATA &&
              expr->e_expr2->e_type == EXPR_DATA &&
              expr->e_expr2->e_offset == code->ou_offset)
      {
        OFFSET_t offset2 = expr->e_expr1->e_expr1->e_offset;
        OFFSET_t offset3 = expr->e_expr1->e_expr2->e_offset;
        expr_free(expr);
        code->ou_instr = BF_INC_DATA_MUL_DATA;
        code->ou_offset2 = offset2;
        code->ou_offset3 = offset3;
      }
    }		      
    else if (code->ou_instr == BF_WHILE_DATA)
    {
      OptimizationUnit* next_code = (OptimizationUnit*)list->next->data;
      if (next_code->ou_instr == BF_INC_DATA_PTR)
      {
        OptimizationUnit* next_next_code = (OptimizationUnit*)list->next->next->data;
        if(next_next_code->ou_instr == BF_END_WHILE &&
           next_next_code->ou_offset == code->ou_offset)
        {
          code->ou_instr = BF_WHILE_INC_DATA_PTR;
          code->ou_offset2 = next_code->ou_offset;
  
          libbf_interpreted_code_list_free_elt(list->next);
          libbf_interpreted_code_list_free_elt(list->next);
        }
      }
    }
#ifdef USE_END_WHILE_INC_DATA_PTR
    else if (code->ou_instr == BF_INC_DATA_PTR)
    {
      OptimizationUnit* next_code = list->next->data;
      if (next_code->ou_instr == BF_END_WHILE)
      {
        code->ou_instr = BF_END_WHILE_INC_DATA_PTR;
        code->ou_offset2 = code->ou_offset;
        code->ou_offset = next_code->ou_offset;
        code->ou_loop_type = LIBBF_WHILE;
        
        libbf_interpreted_code_list_free_elt(list->next);
      }
    }
#endif    

    list = list->next;
  }
}

void libbf_interpreter_optimizer_cdump (const char* code,
                                        const char* data_type)
{

  List* list = libbf_translate_to_optimization_code(code);

  libbf_optimize(&list, data_type, BF_OPTIMIZE_ALL);
  
  libbf_optimizer_transform_generic_expr_to_particular_expr(list, FOR_INTERPRETER);

  libbf_interpreter_optimizer_cdump_internal(stdout, list, data_type, -1);

  libbf_interpreted_code_list_free(list);
}

void libbf_interpreter_optimizer_javadump(const char* code,
                                          const char* data_type)
{
  List* list = libbf_translate_to_optimization_code(code);

  libbf_optimize(&list, data_type, BF_OPTIMIZE_ALL);

  libbf_interpreter_optimizer_javadump_internal(stdout, list, data_type, -1);

  libbf_interpreted_code_list_free(list);
}    

#if 0
static
void libbf_add_inc_data_ptr(List** p_list)
{
  List* list = *p_list;
  OFFSET_t last_offset = 0;
  while (list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    
    OFFSET_t delta_offset = code->ou_offset - last_offset;
    
    switch(code->ou_instr)
    {
      case BF_INC_DATA_PTR:
        list = list->next;
        continue;
        break;
        
      case BF_AFFECT_EXPR:
      {
        expr_inc_offset_by(code->ou_expr, -code->ou_offset);
        break;
      }
      
      case   BF_SET_DATA:        /* data_ptr[offset] = data_ptr[offset2] */
      case   BF_INC_DATA:        /* data_ptr[offset] += data_ptr[offset2] */
      case   BF_DEC_DATA:        /* data_ptr[offset] -= data_ptr[offset2] */
      case   BF_INC_CST_MUL_DATA:/* data_ptr[offset] += cst * data_ptr[offset2] */
      case   BF_INC_CST_ADD_DATA:/* data_ptr[offset] += cst + data_ptr[offset2] */
        code->ou_offset2 -= code->ou_offset;
        break;
        
      case  BF_INC_DATA_MUL_DATA:/*data_ptr[offset] += data_ptr[offset2] * data_ptr[offset3] */
        code->ou_offset2 -= code->ou_offset;
        code->ou_offset3 -= code->ou_offset;
        break;
      
      default:
        break;
    }
    
    last_offset = code->ou_offset;
    if (delta_offset != 0)
    {
      OptimizationUnit* prev_code = (list->prev) ? list->prev->data : NULL;
      if (prev_code && prev_code->ou_instr == BF_INC_DATA_PTR)
      {
        prev_code->ou_offset += delta_offset;
      }
      else
      {
        OptimizationUnit* new_code = alloc0(OptimizationUnit, 1);
        new_code->ou_instr = BF_INC_DATA_PTR;
        new_code->ou_offset = delta_offset;
        list_insert_before(list, new_code);
        if (*p_list == list)
        {
          *p_list = list->prev;
        }
      }
    }
    code->ou_offset = 0;
    
    list = list->next;
  }
  
}
#endif

int libbf_detect_consecutive_min_max_delta_offset(List* list,
                                                  OFFSET_t* p_min_delta_offset,
                                                  OFFSET_t* p_max_delta_offset)
{
  
  OFFSET_t last_min_offset = 0, last_max_offset = 0;
  OFFSET_t min_offset, max_offset;
  OFFSET_t delta_min, delta_max;
  
  int detect_inc_data_ptr = FALSE;

  *p_min_delta_offset = 1;
  *p_max_delta_offset = 1;

  while (list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
      {
        List* offset_list = expr_get_list_of_ref_internal_but_ignore_offset(code->ou_expr, NULL, code->ou_offset);
        List* cur = offset_list;
        min_offset = max_offset = code->ou_offset;
        while(cur)
        {
          OFFSET_t offset = POINTER_TO_OFFSET(cur->data);
          if (offset < min_offset) min_offset = offset;
          else if (offset > max_offset) max_offset = offset;
          cur = cur->next;
        }
        list_free(offset_list, NULL);
        break;
      }

      case BF_INC_DATA_PTR:
      {
        List* next = list->next;
        OptimizationUnit* next_code;
        detect_inc_data_ptr = TRUE;
        
        assert(next);
        next_code = (OptimizationUnit*)next->data;
        assert(next_code->ou_instr == BF_END_WHILE);
        min_offset = max_offset = code->ou_offset + next_code->ou_offset;
        list = list->next; /* skip end while */
        break;
      }
        
      default:
        min_offset = max_offset = code->ou_offset;
        break;
        
    }
    delta_min = last_min_offset - min_offset;
    delta_max = max_offset - last_max_offset;
    if (delta_min > *p_min_delta_offset)
    {
      *p_min_delta_offset = delta_min;
      if (DEBUG)
      {
        fprintf(stderr, "min : %ld\n", (long)delta_min);
        libbf_interpreter_optimizer_cdump_internal(stderr, list->prev, "", 2);
      }
    }
    if (delta_max > *p_max_delta_offset)
    {
      *p_max_delta_offset = delta_max;
      if (DEBUG)
      {
        fprintf(stderr, "max : %ld\n",  (long)delta_max);
        libbf_interpreter_optimizer_cdump_internal(stderr, list->prev, "", 2);
      }
    }
    last_min_offset = min_offset;
    last_max_offset = max_offset;
    
    if (list)
     list = list->next;
  }
  
  return detect_inc_data_ptr;
}    

static
int libbf_remove_complex_expression(List* list)
{
  int successful = TRUE;
  
  while (list)
  {
    OptimizationUnit* code = (OptimizationUnit*) list->data;
    switch(code->ou_instr)
    {
      case BF_AFFECT_EXPR:
      {
        List* listNext = list->next;
        /* ptr[] = (cst * ptr[] ) + cst */
        if (code->ou_expr->e_type == EXPR_ADD &&
            code->ou_expr->e_expr1->e_type == EXPR_MUL &&
            code->ou_expr->e_expr1->e_expr1->e_type == EXPR_CONSTANT &&
            code->ou_expr->e_expr1->e_expr2->e_type == EXPR_DATA &&
            code->ou_expr->e_expr2->e_type == EXPR_CONSTANT)
        {
          OptimizationUnit instr1, instr2;
          OptimizationUnit* newCode;
          
          memset(&instr1, 0, sizeof(instr1));
          memset(&instr2, 0, sizeof(instr2));
          
          instr1.ou_instr = BF_SET_CST;
          instr1.ou_offset = code->ou_offset;
          instr1.ou_constant = code->ou_expr->e_expr2->e_constant;
          if (code->ou_expr->e_expr1->e_expr1->e_constant == -1)
          {
            instr2.ou_instr = BF_DEC_DATA;
          }
          else
          {
            instr2.ou_instr = BF_INC_CST_MUL_DATA;
            instr2.ou_constant = code->ou_expr->e_expr1->e_expr1->e_constant;
          }
          instr2.ou_offset = code->ou_offset;
          instr2.ou_offset2 = code->ou_expr->e_expr1->e_expr2->e_offset;
          expr_free(code->ou_expr);
          *code = instr1;
          
          newCode = alloc0(OptimizationUnit, 1);
          *newCode = instr2;
          
          list_insert_after(list, newCode);
          list = listNext;
          continue;
        }
        /* ptr[] = cst * ptr [] */
        else if (code->ou_expr->e_type == EXPR_MUL &&
                 code->ou_expr->e_expr1->e_type == EXPR_CONSTANT &&
                 code->ou_expr->e_expr2->e_type == EXPR_DATA)
        {
          OptimizationUnit instr1, instr2;
          OptimizationUnit* newCode;
          
          memset(&instr1, 0, sizeof(instr1));
          memset(&instr2, 0, sizeof(instr2));
          
          instr1.ou_instr = BF_SET_CST;
          instr1.ou_offset = code->ou_offset;
          instr1.ou_constant = 0;
          if (code->ou_expr->e_expr1->e_constant == -1)
          {
            instr2.ou_instr = BF_DEC_DATA;
          }
          else
          {
            instr2.ou_instr = BF_INC_CST_MUL_DATA;
            instr2.ou_constant = code->ou_expr->e_expr1->e_constant;
          }
          instr2.ou_offset = code->ou_offset;
          instr2.ou_offset2 = code->ou_expr->e_expr2->e_offset;
          
          expr_free(code->ou_expr);
          *code = instr1;
          
          newCode = alloc0(OptimizationUnit, 1);
          *newCode = instr2;
          
          list_insert_after(list, newCode);
          list = listNext;
          continue;
        }
        /* ptr[] = ptr[] * ptr [] */
        else if (code->ou_expr->e_type == EXPR_MUL &&
                 code->ou_expr->e_expr1->e_type == EXPR_DATA &&
                 code->ou_expr->e_expr2->e_type == EXPR_DATA)
        {
          OptimizationUnit instr1, instr2;
          OptimizationUnit* newCode;
          
          memset(&instr1, 0, sizeof(instr1));
          memset(&instr2, 0, sizeof(instr2));
          
          instr1.ou_instr = BF_SET_CST;
          instr1.ou_offset = code->ou_offset;
          instr1.ou_constant = 0;
          
          instr2.ou_instr = BF_INC_DATA_MUL_DATA;
          instr2.ou_offset = code->ou_offset;
          instr2.ou_offset2 = code->ou_expr->e_expr1->e_offset;
          instr2.ou_offset3 = code->ou_expr->e_expr2->e_offset;
          
          expr_free(code->ou_expr);
          *code = instr1;
          
          newCode = alloc0(OptimizationUnit, 1);
          *newCode = instr2;
          
          list_insert_after(list, newCode);
          list = listNext;
          continue;
        }
        /* ptr[] = ptr [] * cst */
        else if (code->ou_expr->e_type == EXPR_MUL &&
                 code->ou_expr->e_expr1->e_type == EXPR_DATA &&
                 code->ou_expr->e_expr2->e_type == EXPR_CONSTANT)
        {
          OptimizationUnit instr1, instr2;
          OptimizationUnit* newCode;
          
          memset(&instr1, 0, sizeof(instr1));
          memset(&instr2, 0, sizeof(instr2));
          
          instr1.ou_instr = BF_SET_CST;
          instr1.ou_offset = code->ou_offset;
          instr1.ou_constant = 0;
          if (code->ou_expr->e_expr1->e_constant == -1)
          {
            instr2.ou_instr = BF_DEC_DATA;
          }
          else
          {
            instr2.ou_instr = BF_INC_CST_MUL_DATA;
            instr2.ou_constant = code->ou_expr->e_expr2->e_constant;
          }
          instr2.ou_offset = code->ou_offset;
          instr2.ou_offset2 = code->ou_expr->e_expr1->e_offset;
          
          expr_free(code->ou_expr);
          *code = instr1;
          
          newCode = alloc0(OptimizationUnit, 1);
          *newCode = instr2;
          
          list_insert_after(list, newCode);
          list = listNext;
          continue;
        }
        /* ptr[] = ptr[] + cst */
        else if (code->ou_expr->e_type == EXPR_ADD &&
                 code->ou_expr->e_expr1->e_type == EXPR_DATA &&
                 code->ou_expr->e_expr2->e_type == EXPR_CONSTANT)
        {
          OptimizationUnit instr1, instr2;
          OptimizationUnit* newCode;
          
          memset(&instr1, 0, sizeof(instr1));
          memset(&instr2, 0, sizeof(instr2));
          
          instr1.ou_instr = BF_SET_CST;
          instr1.ou_offset = code->ou_offset;
          instr1.ou_constant = code->ou_expr->e_expr2->e_constant;
          instr2.ou_instr = BF_INC_DATA;
          instr2.ou_offset = code->ou_offset;
          instr2.ou_offset2 = code->ou_expr->e_expr1->e_offset;
          
          expr_free(code->ou_expr);
          *code = instr1;
          
          newCode = alloc0(OptimizationUnit, 1);
          *newCode = instr2;
          
          list_insert_after(list, newCode);
          list = listNext;
          continue;
        }
        /* ptr[] = cst + ptr[] */
        else if (code->ou_expr->e_type == EXPR_ADD &&
                 code->ou_expr->e_expr1->e_type == EXPR_CONSTANT &&
                 code->ou_expr->e_expr2->e_type == EXPR_DATA)
        {
          OptimizationUnit instr1, instr2;
          OptimizationUnit* newCode;
          
          memset(&instr1, 0, sizeof(instr1));
          memset(&instr2, 0, sizeof(instr2));
          
          instr1.ou_instr = BF_SET_CST;
          instr1.ou_offset = code->ou_offset;
          instr1.ou_constant = code->ou_expr->e_expr1->e_constant;
          instr2.ou_instr = BF_INC_DATA;
          instr2.ou_offset = code->ou_offset;
          instr2.ou_offset2 = code->ou_expr->e_expr2->e_offset;
          
          expr_free(code->ou_expr);
          *code = instr1;
          
          newCode = alloc0(OptimizationUnit, 1);
          *newCode = instr2;
          
          list_insert_after(list, newCode);
          list = listNext;
          continue;
        }
        /* ptr[X] = (cst * ptr[]) + (cst + ptr[X]) */
        else if (code->ou_expr->e_type == EXPR_ADD &&
                 code->ou_expr->e_expr1->e_type == EXPR_MUL &&
                 code->ou_expr->e_expr1->e_expr1->e_type == EXPR_CONSTANT &&
                 code->ou_expr->e_expr1->e_expr2->e_type == EXPR_DATA &&
                 code->ou_expr->e_expr2->e_type == EXPR_ADD &&
                 code->ou_expr->e_expr2->e_expr1->e_type == EXPR_CONSTANT &&
                 code->ou_expr->e_expr2->e_expr2->e_type == EXPR_DATA &&
                 code->ou_expr->e_expr2->e_expr2->e_offset == code->ou_offset)
        {
          OptimizationUnit instr1, instr2;
          OptimizationUnit* newCode;
          
          memset(&instr1, 0, sizeof(instr1));
          memset(&instr2, 0, sizeof(instr2));
          
          instr1.ou_instr = BF_INC_CST;
          instr1.ou_offset = code->ou_offset;
          instr1.ou_constant = code->ou_expr->e_expr2->e_expr1->e_constant;
          if (code->ou_expr->e_expr1->e_expr1->e_constant == -1)
          {
            instr2.ou_instr = BF_DEC_DATA;
          }
          else
          {
            instr2.ou_instr = BF_INC_CST_MUL_DATA;
            instr2.ou_constant = code->ou_expr->e_expr1->e_expr1->e_constant;
          }
          instr2.ou_offset = code->ou_offset;
          instr2.ou_offset2 = code->ou_expr->e_expr1->e_expr2->e_offset;
          
          expr_free(code->ou_expr);
          *code = instr1;
          
          newCode = alloc0(OptimizationUnit, 1);
          *newCode = instr2;
          
          list_insert_after(list, newCode);
          list = listNext;
          continue;
        }
        else
        {
          fprintf(stderr, "Cannot simplify : ");
          libbf_interpreter_optimizer_cdump_internal_instr(stderr, code, NULL, NULL);
          /* SHOULDNT_HAPPEN(); */
          successful = FALSE;
        }        
      }
      
      default:
        break;
    }
    
    list = list->next;
  }
  
  return successful;
}

#ifndef NO_COMPILER
static void libbf_gen_compiled_code_internal(TargetCpu_t cpu,
                                            int sizeofData, 
                                            const char* source_code,
                                            char** compiled_code,
                                            int* size_compiled_code,
                                            OptimizationUnit** p_code,
                                            int debug_compiled_code)
{
  List* list;
  int min_delta_offset, max_delta_offset;
  int successFull;
  OptimizationUnit* code;

  if (!libbf_is_sizeofData_compatible(cpu, sizeofData)) return;
  
  list = libbf_translate_to_optimization_code(source_code);
  libbf_optimize(&list, "", BF_OPTIMIZE_ALL);
    
  libbf_detect_consecutive_min_max_delta_offset(list, &min_delta_offset, &max_delta_offset);

#ifndef PROFILE  
  libbf_optimizer_transform_generic_expr_to_particular_expr(list, FOR_COMPILER);
#endif

  successFull = libbf_remove_complex_expression(list);
  if (successFull == FALSE)
  {
    libbf_interpreted_code_list_free(list);
    fprintf(stderr, "Cannot compile code\n");
    return;
  }

  code = libbf_interpreted_code_list_to_array(list);
  libbf_interpreted_code_list_free(list);
  
  libbf_gen_compile_internal_from_code_array(
      cpu, sizeofData, code,
      min_delta_offset, max_delta_offset,
      compiled_code, size_compiled_code, debug_compiled_code);
  
  if (p_code)
    *p_code = code;
  else
    libbf_interpreted_code_array_free(code);
}

void libbf_gen_compiled_code(TargetCpu_t cpu, int sizeofData, const char* source_code, char** compiled_code, int* size_compiled_code)
{
  if (compiled_code == NULL || size_compiled_code == NULL)
  {
    warning("compiled_code == NULL || size_compiled_code == NULL\n");
    return;
  }
  
  libbf_gen_compiled_code_internal(cpu, sizeofData, source_code, compiled_code, size_compiled_code, NULL, FALSE);
}


void libbf_execute_compiled_code(char* compiled_code, int size_compiled_code, AdvancedOptions_t* options)
{
  libbf_execute_compiled_code_internal(compiled_code, size_compiled_code, FALSE, options);
}

static void libbf_compile_execute_internal(const char* source_code, int sizeofData, AdvancedOptions_t* options)
{
#if defined(SUPPORT_EXECUTE_COMPILED_CODE)
  char* compiled_code = NULL;
  int size_compiled_code = 0;
  OptimizationUnit* code = NULL;

#ifdef DEBUG_COMPILER
  int debug_compiled_code = TRUE;
#else
  int debug_compiled_code = CHECK_ENV_VAR("DEBUG_COMPILER");
#endif
  
  libbf_gen_compiled_code_internal(TARGET_HOST_CPU, sizeofData, source_code, &compiled_code, &size_compiled_code, &code, debug_compiled_code);

  libbf_execute_compiled_code_internal(compiled_code, size_compiled_code, debug_compiled_code, options);
  
  free(compiled_code);
  
  libbf_interpreted_code_array_free(code);
#else
  warning("Only i386, x86_64, ppc and arm platforms supported\n");
#endif
}

#define DEFINE_LIBBF_COMPILE_EXECUTE(type, type_str) void CONCAT(libbf_compile_execute_, type_str) (const char* source_code, AdvancedOptions_t* options) {  libbf_compile_execute_internal(source_code, sizeof(type), options); }

DEFINE_LIBBF_COMPILE_EXECUTE(char, char);
DEFINE_LIBBF_COMPILE_EXECUTE(short, short);
DEFINE_LIBBF_COMPILE_EXECUTE(int, int);
DEFINE_LIBBF_COMPILE_EXECUTE(long long, long_long);

void libbf_compile_execute_int_n(const char* source_code, int sizeofData, AdvancedOptions_t* options)
{
  libbf_compile_execute_internal(source_code, sizeofData, options);
}
#endif

#ifndef NO_INTERPRETER

static void libbf_optimized_code_resolve_cond_jump(OptimizationUnit* code, int profile)
{
  OptimizationUnit* code_ptr = code;

  OptimizationUnit* stack_while[32768];
  OptimizationUnit** p_stack_while = stack_while;

  while(TRUE)
  {
    switch(code_ptr->ou_instr)
    {
      case BF_END:
        return;

      case BF_WHILE_DATA:
        *p_stack_while = code_ptr;
        p_stack_while++;
        break;

      case BF_END_WHILE:
#ifdef USE_END_WHILE_INC_DATA_PTR
      case BF_END_WHILE_INC_DATA_PTR:
#endif
        --p_stack_while;
        if ((*p_stack_while)->ou_loop_type == LIBBF_IF)
        {
          if (profile)
          {
            code_ptr->ou_instr = BF_NOP;
            (*p_stack_while)->ou_jump_to = code_ptr + 1;
          }
          else
          {
            OptimizationUnit* temp_ptr = code_ptr;
            (*p_stack_while)->ou_jump_to = code_ptr; /* make if false condition skip to instruction following } */
            while(temp_ptr[0].ou_instr != BF_END)
            {
              temp_ptr[0] = temp_ptr[1];
              temp_ptr++;
            }
            continue; /* do not increment code_ptr */
          }
        }
        else if ((*p_stack_while)->ou_loop_type == LIBBF_DO_WHILE)
        {
          if (profile)
          {
            (*p_stack_while)->ou_instr = BF_NOP;
            code_ptr->ou_jump_to = (*p_stack_while) + 1;
          }
          else
          {
            OptimizationUnit* temp_ptr = (*p_stack_while) + 1;
            while(temp_ptr != code_ptr)
            {
              if (temp_ptr->ou_instr == BF_WHILE_DATA ||
                  temp_ptr->ou_instr == BF_END_WHILE)
              {
                temp_ptr->ou_jump_to--;
              }
              temp_ptr++;
            }
            temp_ptr = (*p_stack_while);
            code_ptr->ou_jump_to = (*p_stack_while);
            while(temp_ptr[0].ou_instr != BF_END)
            {
              temp_ptr[0] = temp_ptr[1];
              temp_ptr++;
            }
            continue; /* do not increment code_ptr */
          }
        }
        else
        {
          (*p_stack_while)->ou_jump_to = code_ptr + 1;
          code_ptr->ou_jump_to = (*p_stack_while) + 1;
        }
        break;

      default:
        break;
    }

    code_ptr++;
  }
}

static void libbf_dump_profiled_code(OptimizationUnit* code)
{
  int num_line = 0;
  int inc = 0;
  fprintf(stderr, "Profiling listing\n");
  while(code->ou_instr != BF_END)
  {
    libbf_interpreter_optimizer_cdump_internal_instr(stderr, code, &inc, &num_line);
    code++;
  }
}

static void libbf_optimized_code_resolve_interpeter_addr(OptimizationUnit* code, const void** tab_addr)
{
  while(code->ou_instr != BF_END)
  {
    code->ou_interpreter_addr = (void*)tab_addr[code->ou_instr];
    code++;
  }
  code->ou_interpreter_addr = (void*)tab_addr[code->ou_instr];
}

#if defined(WIN32)
#include "libbf_posix_emulation.h"

static void libbf_interpreter_optimizer_win32_glue(void* fct, void* param0, int param1, int param2)
{
  long pagesize = libbf_getpagesize();
  char* glue_executable_code = libbf_alloc_aligned(pagesize, pagesize);
  unsigned char* c = glue_executable_code;
  libbf_mprotect(glue_executable_code, pagesize, PROT_WRITE | PROT_READ | PROT_EXEC);

  *(c++) = 0x68; *(void**)c = libbf_interpreter_dynalloc_handler_win32; c+= 4; /* push libbf_interpreter_dynalloc_handler_win32 */

  *(c++) = 0x64; *(c++) = 0xff; *(c++) = 0x35; *(int*)c = 0; c+= 4; /* pushl  %fs:0x0 */
  *(c++) = 0x64; *(c++) = 0x89; *(c++) = 0x25; *(int*)c = 0; c+= 4; /* mov    %esp,%fs:0x0 */

  *(c++) = 0xff; *(c++) = 0x74; *(c++) = 0x24; *(c++) = 20; /* pushl  20(%esp) = param2 */
  *(c++) = 0xff; *(c++) = 0x74; *(c++) = 0x24; *(c++) = 20; /* pushl  20(%esp) = param1 */
  *(c++) = 0xff; *(c++) = 0x74; *(c++) = 0x24; *(c++) = 20; /* pushl  20(%esp) = param0 */
  *(c++) = 0xb8; *(void**)c = fct; c+= 4; /*  mov    fct,%eax */

  *(c++) = 0xff; *(c++) = 0xd0; /* call   *%eax */

  *(c++) = 0x83; *(c++) = 0xc4; *(c++) = 12; /*  add    $12,%esp */

  *(c++) = 0x8b; *(c++) = 0x44; *(c++) = 0x24; *(c++) = 0x04; /* mov    0x4(%esp),%eax */
  *(c++) = 0x64; *(c++) = 0xa3; *(int*)c = 0; c+= 4; /* mov    %eax,%fs:0x0 */
  *(c++) = 0x83; *(c++) = 0xc4; *(c++) = 0x08; /* add    $0x8,%esp */
  *(c++) = 0xc3; /* retq */

  ((void (*)(void*, int, int))glue_executable_code)(param0, param1, param2);
  
  libbf_mprotect(glue_executable_code, pagesize, PROT_WRITE | PROT_READ);
  libbf_free_aligned(glue_executable_code);
}

#endif

static
void libbf_interpreter_optimizer_general_internal (const char* source_code, void* fct, int profile)
{
  List* list;
  OFFSET_t min_delta_offset, max_delta_offset;
  int need_dynalloc;
  OptimizationUnit* code;

  list = libbf_translate_to_optimization_code(source_code);

  libbf_optimize(&list, "", BF_OPTIMIZE_ALL);

  need_dynalloc =
      libbf_detect_consecutive_min_max_delta_offset(list, &min_delta_offset, &max_delta_offset);
  
  if (DEBUG)
  {
    fprintf(stderr, "need_dynalloc=%d\n", need_dynalloc);
    fprintf(stderr, "min_delta_offset=%ld\n", (long)min_delta_offset);
    fprintf(stderr, "max_delta_offset=%ld\n", (long)max_delta_offset);
  }
  
  /* libbf_add_inc_data_ptr(&list); */
  if (!profile)
  {
    libbf_optimizer_transform_generic_expr_to_particular_expr(list, FOR_INTERPRETER);
    libbf_remove_complex_expression(list);
  }

  code = libbf_interpreted_code_list_to_array(list);
  libbf_interpreted_code_list_free(list);
  
  libbf_optimized_code_resolve_cond_jump(code, profile);

#if defined(WIN32)
  libbf_interpreter_optimizer_win32_glue(fct, code, min_delta_offset, max_delta_offset);
#else
  ((void (*)(void*, int, int))fct) (code, min_delta_offset, max_delta_offset);
#endif

  libbf_interpreted_code_array_free(code);
}

#define MY_TYPE_T                         char
#define MY_TYPE_T_STR                     char
#include "libbf_interpreter_simple.c"
#include "libbf_interpreter_optimizer.c"
#undef MY_TYPE_T
#undef MY_TYPE_T_STR

#define MY_TYPE_T                         short
#define MY_TYPE_T_STR                     short
#include "libbf_interpreter_simple.c"
#include "libbf_interpreter_optimizer.c"
#undef MY_TYPE_T
#undef MY_TYPE_T_STR

#define MY_TYPE_T                         int
#define MY_TYPE_T_STR                     int
#include "libbf_interpreter_simple.c"
#include "libbf_interpreter_optimizer.c"
#undef MY_TYPE_T
#undef MY_TYPE_T_STR

#define MY_TYPE_T                         long
#define MY_TYPE_T_STR                     long
#include "libbf_interpreter_simple.c"
#include "libbf_interpreter_optimizer.c"
#undef MY_TYPE_T
#undef MY_TYPE_T_STR

#define MY_TYPE_T                         long long
#define MY_TYPE_T_STR                     long_long
#include "libbf_interpreter_simple.c"
#include "libbf_interpreter_optimizer.c"
#undef MY_TYPE_T
#undef MY_TYPE_T_STR

#endif
