#ifndef CPU_H
#define CPU_H

#define GPR_NUM 32
#define FPR_NUM 32
#define MEM_NUM (1<<20)

#define OP_NOP   0
#define OP_ADD   1
#define OP_ADDI  2
#define OP_SUB   3
#define OP_SUBI  4
#define OP_BEQ   5
#define OP_ST    6
#define OP_LD    7
#define OP_JR    8
#define OP_JAL   9
#define OP_SEND 10
#define OP_HALT 11
#define OP_SLL  12
#define OP_SRL  13
#define OP_FADD 14
#define OP_FMUL 15
#define OP_FINV 16
#define OP_FABS 17
#define OP_FNEG 18
#define OP_FCMP 19

typedef union {
  uint32_t i;
  float f;
} IF;

extern uint32_t pc;

extern int32_t gpr[GPR_NUM];
extern IF fpr[FPR_NUM];

extern uint32_t memory[MEM_NUM];

void exec_inst();

#endif
