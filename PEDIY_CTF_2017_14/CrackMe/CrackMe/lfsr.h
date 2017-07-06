#ifndef LFSR
#define LFSR

#define OP_END          0
#define OP_INC_DP       1
#define OP_DEC_DP       2
#define OP_INC_VAL      3
#define OP_DEC_VAL      4
#define OP_OUT          5
#define OP_IN           6
#define OP_JMP_FWD      7
#define OP_JMP_BCK      8

#define SUCCESS         0
#define FAILURE         1

#define PROGRAM_SIZE    65535000
#define STACK_SIZE      4096000
#define DATA_SIZE       65535000

struct instruction_t {
    unsigned int bfoperator;
    unsigned int operand;
};

int lfsr_bf(char* key);
#endif
