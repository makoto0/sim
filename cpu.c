#include<stdio.h>
#include<stdint.h>
#include"cpu.h"

uint32_t pc=0;

int32_t gpr[GPR_NUM]={};
IF fpr[FPR_NUM]={};

uint32_t memory[MEM_NUM]={};

void decode(uint32_t inst,uint32_t *opcode,uint32_t *rs,uint32_t *rt,uint32_t *rd,uint32_t *shamt,uint32_t *funct,int16_t *imm,uint32_t *addr)
{
  *opcode=inst>>26;
  *rs=(inst>>21)&0x1f;
  *rt=(inst>>16)&0x1f;
  *rd=(inst>>11)&0x1f;
  *shamt=(inst>>6)&0x1f;
  *funct=inst&0x3f;
  *imm=inst&0xffff;
  *addr=inst&0x3ffffff;
}

void exec_inst(uint32_t inst)
{
  uint32_t opcode,rs,rt,rd,shamt,funct,addr;
  int16_t imm;

  decode(inst,&opcode,&rs,&rt,&rd,&shamt,&funct,&imm,&addr);

  switch (opcode) {
  case OP_NOP:
    pc++;
    break;
  case OP_ADD:
    gpr[rd]=gpr[rs]+gpr[rt];
    pc++;
    break;
  case OP_ADDI:
    gpr[rt]=gpr[rs]+imm;
    pc++;
    break;
  case OP_SUB:
    gpr[rd]=gpr[rs]-gpr[rt];
    pc++;
    break;
  case OP_SUBI:
    gpr[rt]=gpr[rs]-imm;
    pc++;
    break;
  case OP_BEQ:
    if (gpr[rs]==gpr[rt]) {
      pc=pc+1+imm;
    } else {
      pc++;
    }
    break;
  case OP_ST:
    memory[gpr[rs]]=gpr[rt];
    pc++;
    break;
  case OP_LD:
    gpr[rt]=memory[gpr[rs]];
    pc++;
    break;
  case OP_JR:
    pc=gpr[rs];
    break;
  case OP_JAL:
    gpr[30]=pc+1;
    pc=addr;
    break;
  case OP_SEND:
    printf("send %2d : %d\n",rs,gpr[rs]);
    pc++;
    break;
  case OP_HALT:
    break;
  case OP_SLL:
    gpr[rd]=gpr[rt]<<shamt;
    pc++;
    break;
  case OP_SRL:
    gpr[rd]=gpr[rt]>>shamt;
    pc++;
    break;
  case OP_FADD:
    fpr[rd].f=fpr[rs].f+fpr[rt].f;
    pc++;
    break;
  case OP_FMUL:
    fpr[rd].f=fpr[rs].f*fpr[rt].f;
    pc++;
    break;
  case OP_FINV:
    fpr[rt].f=1.0/fpr[rs].f;
    pc++;
    break;
  case OP_FABS:
    if (fpr[rs].f<0) {
      fpr[rt].f=-fpr[rs].f;
    } else {
      fpr[rt].f=fpr[rs].f;
    }
    pc++;
    break;
  case OP_FNEG:
    fpr[rt].f=-fpr[rs].f;
    break;
  case OP_FCMP:
    break;
  default:
    printf("Unknown instruction\n");
    break;
  }
}
