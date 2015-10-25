#include<stdio.h>
#include<stdint.h>
#include"cpu.h"

uint32_t pc=0;
uint32_t fpcond=0;

int32_t gpr[GPR_NUM]={};
IF fpr[FPR_NUM]={};

uint32_t memory[MEM_NUM]={};

void printinst(uint32_t i) /* debug */
{
  int k;

  for (k=31;k>=0;k--) {
    printf("%d",(i>>k)&1);
    if (k==26 || k==21 || k==16 || k==11 || k==6) {
      printf(" ");
    }
  }
  printf("\n");
}

void print8(uint32_t i)
{
  int k;

  for (k=7;k>=0;k--) {
    printf("%d",(i>>k)&1);
  }
  printf("\n");
}

void decode(uint32_t inst,uint32_t *opcode,uint32_t *r1,uint32_t *r2,uint32_t *r3,uint32_t *shamt,uint32_t *funct,int16_t *imm,uint32_t *addr)
{
  *opcode=inst>>26;
  *r1=(inst>>21)&0x1f;
  *r2=(inst>>16)&0x1f;
  *r3=(inst>>11)&0x1f;
  *shamt=(inst>>6)&0x1f;
  *funct=inst&0x3f;
  *imm=inst&0xffff;
  *addr=inst&0x3ffffff;
}

void exec_inst(uint32_t inst)
{
  uint32_t opcode,r1,r2,r3,shamt,funct,addr;
  int16_t imm;

  uint32_t recvdata;

  decode(inst,&opcode,&r1,&r2,&r3,&shamt,&funct,&imm,&addr);

  printinst(inst);

  switch (opcode) {
  case OP_NOP:
    pc++;
    nop_count++;
    break;
  case OP_ADD:
    gpr[r1]=gpr[r2]+gpr[r3];
    pc++;
    add_count++;
    break;
  case OP_ADDI:
    gpr[r1]=gpr[r2]+imm;
    pc++;
    addi_count++;
    break;
  case OP_SUB:
    gpr[r1]=gpr[r2]-gpr[r3];
    pc++;
    sub_count++;
    break;
  case OP_SUBI:
    gpr[r1]=gpr[r2]-imm;
    pc++;
    subi_count++;
    break;
  case OP_BEQ:
    if (gpr[r1]==gpr[r2]) {
      pc=pc+1+imm;
    } else {
      pc++;
    }
    beq_count++;
    break;
  case OP_BNEQ:
    if (gpr[r1]!=gpr[r2]) {
      pc=pc+1+imm;
    } else {
      pc++;
    }
    bneq_count++;
    break;
  case OP_ST:
    memory[gpr[r1]]=gpr[r2];
    pc++;
    st_count++;
    break;
  case OP_LD:
    gpr[r2]=memory[gpr[r1]];
    pc++;
    ld_count++;
    break;
  case OP_JR:
    pc=gpr[r1];
    jr_count++;
    break;
  case OP_JAL:
    gpr[30]=pc+1;
    pc=addr;
    jal_count++;
    break;
  case OP_SEND:
    printf("send r%d : %d\n",r1,gpr[r1]);
    pc++;
    send_count++;
    break;
  case OP_HALT:
    halt_count++;
    break;
  case OP_SLL:
    gpr[r1]=gpr[r2]<<gpr[r3];
    pc++;
    sll_count++;
    break;
  case OP_SRL:
    gpr[r1]=gpr[r2]>>gpr[r3];
    pc++;
    srl_count++;
    break;
  case OP_FADD:
    fpr[r1].f=fpr[r2].f+fpr[r3].f;
    pc++;
    fadd_count++;
    break;
  case OP_FMUL:
    fpr[r1].f=fpr[r2].f*fpr[r3].f;
    pc++;
    fmul_count++;
    break;
  case OP_FINV:
    fpr[r1].f=1.0/fpr[r2].f;
    pc++;
    finv_count++;
    break;
  case OP_FABS:
    if (fpr[r2].f<0) {
      fpr[r1].f=-fpr[r2].f;
    } else {
      fpr[r1].f=fpr[r2].f;
    }
    pc++;
    fabs_count++;
    break;
  case OP_FNEG:
    fpr[r1].f=-fpr[r2].f;
    pc++;
    fneg_count++;
    break;
  case OP_SLT:
    if (gpr[r2]<gpr[r3]) {
      gpr[r1]=1;
    } else {
      gpr[r1]=0;
    }
    pc++;
    slt_count++;
    break;
  case OP_FSEQ:
    if (fpr[r1].i==fpr[r2].i) {
      fpcond=1;
    } else {
      fpcond=0;
    }
    pc++;
    fseq_count++;
    break;
  case OP_FSLT:
    if (fpr[r1].i<fpr[r2].i) {
      fpcond=1;
    } else {
      fpcond=0;
    }
    pc++;
    fslt_count++;
    break;
  case OP_BCLT:
    if (fpcond==1) {
      pc=pc+addr+1;
    } else {
      pc++;
    }
    bclt_count++;
    break;
  case OP_BCLF:
    if (fpcond==0) {
      pc=pc+addr+1;
    } else {
      pc++;
    }
    bclf_count++;
    break;
  case OP_SEND8:
    printf("send8 r%d : ",r1);
    print8(gpr[r1]);
    pc++;
    send8_count++;
    break;
  case OP_RECV8:
    printf("recv8(hexで入力)>");
    scanf("%x",&recvdata);
    print8(recvdata);
    gpr[r1]=(gpr[r1]&0xffffff00)|recvdata;
    pc++;
    recv8_count++;
    break;
  case OP_FST:
    memory[gpr[r1]]=fpr[r2].i;
    pc++;
    fst_count++;
    break;
  case OP_FLD:
    fpr[r2].i=memory[gpr[r1]];
    pc++;
    fld_count++;
    break;
  case OP_FMOV:
    fpr[r1].i=fpr[r2].i;
    pc++;
    fmov_count++;
    break;
  default:
    printf("Unknown instruction\n");
    pc++;
    break;
  }
}
