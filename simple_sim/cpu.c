#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include"cpu.h"
#include"fpu.h"

uint32_t pc=0;
uint32_t fpcond=0;

int32_t gpr[GPR_NUM]={};
IF fpr[FPR_NUM]={};

uint32_t sram[SRAM_NUM]={};
uint32_t bram[BRAM_NUM]={};

void decode(uint32_t inst,uint32_t *opcode,uint32_t *r1,uint32_t *r2,uint32_t *r3,uint32_t *shamt,uint32_t *funct,int16_t *imm,uint16_t *uimm,int16_t *addr)
{
  *opcode=inst>>26;
  *r1=(inst>>21)&0x1f;
  *r2=(inst>>16)&0x1f;
  *r3=(inst>>11)&0x1f;
  *shamt=(inst>>6)&0x1f;
  *funct=inst&0x3f;
  *imm=inst&0xffff;
  *uimm=inst&0xffff;
  *addr=inst&0x3ffffff;
}

void exec_inst(uint32_t inst)
{
  uint32_t opcode,r1,r2,r3,shamt,funct;
  int16_t imm;
  uint16_t uimm;
  int16_t addr;

  uint32_t recvdata=0;
  uint8_t senddata=0;

  int finv_addr,fsqrt_addr;

  decode(inst,&opcode,&r1,&r2,&r3,&shamt,&funct,&imm,&uimm,&addr);

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
    sram[gpr[r1]+imm]=gpr[r2];
    pc++;
    st_count++;
    break;
  case OP_LD:
    gpr[r2]=sram[gpr[r1]+imm];
    pc++;
    ld_count++;
    break;
  case OP_JR:
    pc=gpr[r1];
    jr_count++;
    break;
  case OP_JAL:
    gpr[31]=pc+1;
    pc=addr;
    jal_count++;
    break;
  case OP_SEND:
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
    if (x86flag) {
      fpr[r1].f=fpr[r2].f+fpr[r3].f;
    } else {
      fpr[r1].i=fadd(fpr[r2].i,fpr[r3].i);
    }
    pc++;
    fadd_count++;
    break;
  case OP_FMUL:
    if (x86flag) {
      fpr[r1].f=fpr[r2].f*fpr[r3].f;
    } else {
      fpr[r1].i=fmul(fpr[r2].i,fpr[r3].i);
    }
    pc++;
    fmul_count++;
    break;
  case OP_FINV:
    if (x86flag) {
      fpr[r1].f=1.0/fpr[r2].f;
    } else {
      finv_addr=(fpr[r2].i & 0x7FE000)>>13;
      fpr[r1].i=finv(fpr[r2].i,finv_table1[finv_addr],finv_table2[finv_addr]);
    }
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
    if (fpr[r1].f==fpr[r2].f) {
      fpcond=1;
    } else {
      fpcond=0;
    }
    pc++;
    fseq_count++;
    break;
  case OP_FSLT:
    if (fpr[r1].f<fpr[r2].f) {
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
    if (send8flag) {
      senddata=gpr[r1]&0xff;
      fwrite(&senddata,1,1,fpsend8);
    }
    pc++;
    send8_count++;
    break;
  case OP_RECV8:
    if (recv8flag && fread(&recvdata,1,1,fprecv8)==0) {
      printf("recv8(hexで入力)>");
      scanf("%x",&recvdata);
    }
    gpr[r1]=(gpr[r1]&0xffffff00)|recvdata;
    pc++;
    recv8_count++;
    break;
  case OP_FST:
    sram[gpr[r1]+imm]=fpr[r2].i;
    pc++;
    fst_count++;
    break;
  case OP_FLD:
    fpr[r2].i=sram[gpr[r1]+imm];
    pc++;
    fld_count++;
    break;
  case OP_FMOV:
    fpr[r1].i=fpr[r2].i;
    pc++;
    fmov_count++;
    break;
  case OP_ADDIU:
    gpr[r1]=gpr[r2]+uimm;
    pc++;
    addiu_count++;
    break;
  case OP_FSQRT:
    if (x86flag) {
      fpr[r1].f=sqrt(fpr[r2].f);
    } else {
      fsqrt_addr=(fpr[r2].i & 0xFFC000)>>14;
      fpr[r1].i=fsqrt(fpr[r2].i,fsqrt_table1[fsqrt_addr],fsqrt_table2[fsqrt_addr]);
    }
    pc++;
    fsqrt_count++;
    break;
  default:
    pc++;
    break;
  }
}
