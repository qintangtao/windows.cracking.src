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

#include "libbf.h"

char* get_code(char* filename)
{
  FILE* f;
  int len;
  int read;
  int allocated;
  char* code;
  if (strcmp(filename, "-") == 0)
    f = stdin;
  else
    f = fopen(filename, "rb");
  if (f == NULL)
  {
    fprintf(stderr, "Cannot open %s\n", filename);
    abort();
  }
  allocated = 1024;
  code = (char*)malloc(1024);
  len = 0;
  do
  {
   if (len + 1024 > allocated)
   {
     allocated = 2 * len + 1024;
     code = (char*)realloc(code, allocated);
   }
   read = fread(code + len, 1, 1024, f);
   len += read;
  } while(read);
  code[len] = 0;

  if (f != stdin) fclose(f);
  return code;
}


typedef struct
{
  char* name;
  void* fct;
} Command;

Command commands[] =
{
#ifndef NO_INTERPRETER
  {"--interpreter-optimizer-profile-char", (void*)libbf_interpreter_optimizer_profile_char },
  {"--interpreter-optimizer-profile-short", (void*)libbf_interpreter_optimizer_profile_short },
  {"--interpreter-optimizer-profile-int", (void*)libbf_interpreter_optimizer_profile_int },
  {"--interpreter-optimizer-profile-long", (void*)libbf_interpreter_optimizer_profile_long },
  {"--interpreter-optimizer-profile-long-long", (void*)libbf_interpreter_optimizer_profile_long_long },
  
  {"--interpreter-optimizer-char", (void*)libbf_interpreter_optimizer_char },
  {"--interpreter-optimizer-short", (void*)libbf_interpreter_optimizer_short },
  {"--interpreter-optimizer-int", (void*)libbf_interpreter_optimizer_int },
  {"--interpreter-optimizer-long", (void*)libbf_interpreter_optimizer_long },
  {"--interpreter-optimizer-long-long", (void*)libbf_interpreter_optimizer_long_long },
  
  {"--interpreter-optimizer-safe-jump-char", (void*)libbf_interpreter_optimizer_safe_jump_char },
  {"--interpreter-optimizer-safe-jump-short", (void*)libbf_interpreter_optimizer_safe_jump_short },
  {"--interpreter-optimizer-safe-jump-int", (void*)libbf_interpreter_optimizer_safe_jump_int },
  {"--interpreter-optimizer-safe-jump-long", (void*)libbf_interpreter_optimizer_safe_jump_long },
  {"--interpreter-optimizer-safe-jump-long-long", (void*)libbf_interpreter_optimizer_safe_jump_long_long },
  
  {"--naive-interpreter-char", (void*)libbf_naive_interpreter_char },
  {"--naive-interpreter-short", (void*)libbf_naive_interpreter_short },
  {"--naive-interpreter-int", (void*)libbf_naive_interpreter_int },
  {"--naive-interpreter-long", (void*)libbf_naive_interpreter_long },
  {"--naive-interpreter-long-long", (void*)libbf_naive_interpreter_long_long },
  
  {"--interpreter-char", (void*)libbf_interpreter_char },
  {"--interpreter-short", (void*)libbf_interpreter_short },
  {"--interpreter-int", (void*)libbf_interpreter_int },
  {"--interpreter-long", (void*)libbf_interpreter_long },
  {"--interpreter-long-long", (void*)libbf_interpreter_long_long },
#endif

  {"--c-to-bf", (void*)libbf_c_to_bf },

#ifndef NO_COMPILER
  {"--compile-execute-char", (void*)libbf_compile_execute_char },
  {"--compile-execute-short", (void*)libbf_compile_execute_short },
  {"--compile-execute-int", (void*)libbf_compile_execute_int },
  {"--compile-execute-long-long", (void*)libbf_compile_execute_long_long },
#endif

  {"--interpreter-optimizer-cdump-char", NULL},
  {"--interpreter-optimizer-cdump-short", NULL},
  {"--interpreter-optimizer-cdump-int", NULL},
  {"--interpreter-optimizer-cdump-long", NULL},
  {"--interpreter-optimizer-cdump-long-long", NULL},
};

#define N_ELEMENTS(x) (sizeof(x) / sizeof(x[0]))

static void usage()
{
  unsigned int i;
  printf("libbf : Brainfuck interpreter/optimizer/compiler\n");
  printf("Usage : libbf command [fileName|-]\n");
  printf("where command is one of :\n");
  for(i=0;i<N_ELEMENTS(commands);i++)
  {
    printf("  %s\n", commands[i].name);
  }
}

int main(int argc, char *argv[])
{
  int i;

  AdvancedOptions_t options;
  options.stop_after_ms = 0;
  options.average_load = 0;
  options.suspend_file = "suspend_bf.dat";
  for(i=1;i<argc;i++)
  {
    if (strncmp(argv[i], "--stop_after_ms=", strlen("--stop_after_ms=")) == 0)
    {
      options.stop_after_ms = atoi(argv[i] +  strlen("--stop_after_ms="));
    }
    else if (strncmp(argv[i], "--average_load=", strlen("--average_load=")) == 0)
    {
      options.average_load = atoi(argv[i] +  strlen("--average_load="));
    }
    else if (strncmp(argv[i], "--suspend_file=", strlen("--suspend_file=")) == 0)
    {
      options.suspend_file = argv[i] +  strlen("--suspend_file=");
    }
  }

  for(i=1;i<argc;i++)
  {
    if (strcmp(argv[i], "--help") == 0)
    {
      usage();
    }
    else if (strcmp(argv[i], "--interpreter-optimizer-javadump") == 0)
    {
      char* filename = argv[i+1];
      char* code = get_code(filename);
      libbf_interpreter_optimizer_javadump(code, "int");
      free(code);
      break;
    }
    else if (strncmp(argv[i], "--interpreter-optimizer-cdump-", strlen("--interpreter-optimizer-cdump-")) == 0)
    {
      char* filename = argv[i+1];
      char* code = get_code(filename);
      libbf_interpreter_optimizer_cdump(code, argv[i] +  strlen("--interpreter-optimizer-cdump-"));
      free(code);
      break;
    }
#ifndef NO_COMPILER
    else if (strncmp(argv[i], "--compile-execute-int-", strlen("--compile-execute-int-")) == 0)
    {
      int num_of_int = atoi(argv[i] +  strlen("--compile-execute-int-")) / 8;
      char* filename = argv[i+1];
      char* code = get_code(filename);
      libbf_compile_execute_int_n(code, num_of_int, &options);
      free(code);
      break;
    }
    else if (strcmp(argv[i], "--resume") == 0)
    {
      libbf_resume(&options);
      break;
    }
#endif
    else
    {
      unsigned int j;
      for(j=0;j<N_ELEMENTS(commands);j++)
      {
        if (strcmp(argv[i], commands[j].name) == 0)
        {
          char* filename = argv[i+1];
          char* code = get_code(filename);
          if (strncmp(argv[i], "--compile-execute", strlen("--compile-execute")) == 0)
            ((void (*)(const char*, AdvancedOptions_t*))commands[j].fct)(code, &options);
          else
            ((void (*)(const char*))commands[j].fct)(code);
          free(code);
          break;
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
