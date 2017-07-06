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

void CONCAT(libbf_naive_interpreter_,MY_TYPE_T_STR) (const char* code)
{
  char* code_ptr = (char*)code;
  unsigned MY_TYPE_T data[32768] = {0};
  long data_indice = 0;
  char* stack[32768];
  char** p_stack = stack;
  int level;

  while(TRUE)
  {
    switch(*code_ptr)
    {
      case 0: return; break;
      case '+': data[data_indice]++; break;
      case '-': data[data_indice]--; break;
      case '>': data_indice++; break;
      case '<': data_indice--; break;
      case '.': putchar(data[data_indice]); break;
      case ',': data[data_indice] = getchar(); break;
      case '[':
      {
        if (data[data_indice] != 0)
          *(p_stack++) = code_ptr;
        else
        {
          level = 1;
          do
          {
            code_ptr++;
            if (*code_ptr == '[') level++;
            else if (*code_ptr == ']') level--;
          } while(level);
        }
      }
      break;
      case ']': code_ptr = *(--p_stack) - 1; break;
      default: break;
    }
    code_ptr++;
  }
}


void CONCAT(libbf_interpreter_,MY_TYPE_T_STR) (const char* code)
{
  InterpreterUnit* new_code = libbf_gen_interpreted_code(code);
  InterpreterUnit* new_code_ptr = new_code;
  unsigned MY_TYPE_T data[32768] = {0};
  long data_indice = 0;

  while(TRUE)
  {
    switch(new_code_ptr->instr)
    {
      case BF_SIMPLE_END     : free(new_code); return; break;
      case BF_SIMPLE_PLUS    : data[data_indice]++; break;
      case BF_SIMPLE_MINUS   : data[data_indice]--; break;
      case BF_SIMPLE_GREATER : data_indice++; break;
      case BF_SIMPLE_LESSER  : data_indice--; break;
      case BF_SIMPLE_DOT     : putchar(data[data_indice]); break;
      case BF_SIMPLE_COMMA   : data[data_indice] = getchar(); break;
      case BF_SIMPLE_LBRACKET: if (data[data_indice] == 0)
          new_code_ptr = new_code_ptr->jump_to; break;
          case BF_SIMPLE_RBRACKET: new_code_ptr = new_code_ptr->jump_to-1; break;
      
      default:
        SHOULDNT_HAPPEN();
        break;
    }
    new_code_ptr++;
  }
}
