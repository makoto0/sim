#ifndef CPU_H
#define CPU_H

#define GPR_NUM 32
#define FPR_NUM 32
#define MEM_NUM (1<<20)

#define OP_NOP  0x0
#define OP_ADD  0x1
#define OP_ADDI 0x2
#define OP_SUB  0x3
#define OP_SUBI 0x4
#define OP_BEQ  0x10
#define OP_ST   0x8
#define OP_LD   0x9
#define OP_JR   0x12
#define OP_JAL  0x13
#define OP_SEND 0x20
#define OP_HALT 0x21
#define OP_SLL  0x5
#define OP_SRL  0x6
#define OP_FADD 0x30
#define OP_FMUL 0x31
#define OP_FINV 0x32
#define OP_FABS 0x34
#define OP_FNEG 0x33
#define OP_SLT
#define OP_BCLT
#define OP_BCLF
#define OP_SEND8
#define OP_READ8
#define OP_FST
#define OP_FLD
#define OP_BNEQ 0x11
#define OP_FMOV


#define HALT (OP_HALT<<26)

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
