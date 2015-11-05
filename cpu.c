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
  uint8_t senddata;

  decode(inst,&opcode,&r1,&r2,&r3,&shamt,&funct,&imm,&addr);

  if (!noprintflag) {
    printinst(inst);
  }

  switch (opcode) {
  case OP_NOP:
    if (!noprintflag) {
      printf("nop\n");
    }
    pc++;
    nop_count++;
    break;
  case OP_ADD:
    gpr[r1]=gpr[r2]+gpr[r3];
    if (!noprintflag) {
      printf("add : r%d <- r%d + r%d\n",r1,r2,r3);
    }
    pc++;
    add_count++;
    break;
  case OP_ADDI:
    gpr[r1]=gpr[r2]+imm;
    if (!noprintflag) {
      printf("addi : r%d <- r%d + %d\n",r1,r2,imm);
    }
    pc++;
    addi_count++;
    break;
  case OP_SUB:
    gpr[r1]=gpr[r2]-gpr[r3];
    if (!noprintflag) {
      printf("sub : r%d <- r%d - r%d\n",r1,r2,r3);
    }
    pc++;
    sub_count++;
    break;
  case OP_SUBI:
    gpr[r1]=gpr[r2]-imm;
    if (!noprintflag) {
      printf("subi : r%d <- r%d - %d\n",r1,r2,imm);
    }
    pc++;
    subi_count++;
    break;
  case OP_BEQ:
    if (gpr[r1]==gpr[r2]) {
      pc=pc+1+imm;
    } else {
      pc++;
    }
    if (!noprintflag) {
      printf("beq : pc <- (r%d = r%d) ? pc + %d + 1 : pc + 1\n",r1,r2,imm);
    }
    beq_count++;
    break;
  case OP_BNEQ:
    if (gpr[r1]!=gpr[r2]) {
      pc=pc+1+imm;
    } else {
      pc++;
    }
    if (!noprintflag) {
      printf("bneq : pc <- (r%d != r%d) ? pc + %d + 1 : pc + 1\n",r1,r2,imm);
    }
    bneq_count++;
    break;
  case OP_ST:
    memory[gpr[r1]]=gpr[r2];
    if (!noprintflag) {
      printf("st : mem[r%d] <- r%d\n",r1,r2);
    }
    pc++;
    st_count++;
    break;
  case OP_LD:
    gpr[r2]=memory[gpr[r1]];
    if (!noprintflag) {
      printf("st : r%d <- mem[r%d]\n",r2,r1);
    }
    pc++;
    ld_count++;
    break;
  case OP_JR:
    pc=gpr[r1];
    if (!noprintflag) {
      printf("jr : pc <- r%d\n",r1);
    }
    jr_count++;
    break;
  case OP_JAL:
    gpr[31]=pc+1;
    pc=addr;
    if (!noprintflag) {
      printf("jal : r31 <- pc + 1; pc <- %d\n",addr);
    }
    jal_count++;
    break;
  case OP_SEND:
    if (!noprintflag) {
      printf("send : r%d = %d\n",r1,gpr[r1]);
    }
    pc++;
    send_count++;
    break;
  case OP_HALT:
    halt_count++;
    break;
  case OP_SLL:
    gpr[r1]=gpr[r2]<<gpr[r3];
    if (!noprintflag) {
      printf("sll : r%d <- r%d << r%d\n",r1,r2,r3);
    }
    pc++;
    sll_count++;
    break;
  case OP_SRL:
    gpr[r1]=gpr[r2]>>gpr[r3];
    if (!noprintflag) {
      printf("srl : r%d <- r%d >> r%d\n",r1,r2,r3);
    }
    pc++;
    srl_count++;
    break;
  case OP_FADD:
    fpr[r1].f=fpr[r2].f+fpr[r3].f;
    if (!noprintflag) {
      printf("fadd : r%d <- r%d + r%d\n",r1,r2,r3);
    }
    pc++;
    fadd_count++;
    break;
  case OP_FMUL:
    fpr[r1].f=fpr[r2].f*fpr[r3].f;
    if (!noprintflag) {
      printf("fmul : r%d <- r%d * r%d\n",r1,r2,r3);
    }
    pc++;
    fmul_count++;
    break;
  case OP_FINV:
    fpr[r1].f=1.0/fpr[r2].f;
    if (!noprintflag) {
      printf("finv : r%d <- 1/r%d\n",r1,r2);
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
    if (!noprintflag) {
      printf("fabs : r%d <- abs(r%d)\n",r1,r2);
    }
    pc++;
    fabs_count++;
    break;
  case OP_FNEG:
    fpr[r1].f=-fpr[r2].f;
    if (!noprintflag) {
      printf("fneg : r%d <- -r%d\n",r1,r2);
    }
    pc++;
    fneg_count++;
    break;
  case OP_SLT:
    if (gpr[r2]<gpr[r3]) {
      gpr[r1]=1;
    } else {
      gpr[r1]=0;
    }
    if (!noprintflag) {
      printf("slt : r%d <- (r%d < r%d) ? 1 : 0\n",r1,r2,r3);
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
    if (!noprintflag) {
      printf("fseq : fpcond <- (f%d = f%d) ? 1 : 0\n",r1,r2);
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
    if (!noprintflag) {
      printf("fslt : fpcond <- (f%d < f%d) ? 1 : 0\n",r1,r2);
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
    if (!noprintflag) {
      printf("bclt : pc <- (fpcond = 1) ? pc + %d + 1 : pc + 1\n",addr);
    }
    bclt_count++;
    break;
  case OP_BCLF:
    if (fpcond==0) {
      pc=pc+addr+1;
    } else {
      pc++;
    }
    if (!noprintflag) {
      printf("bclf : pc <- (fpcond = 0) ? pc + %d + 1 : pc + 1\n",addr);
    }
    bclf_count++;
    break;
  case OP_SEND8:
    if (send8flag) {
      senddata=gpr[r1]&0xff;
      fwrite(&senddata,1,1,fpsend8);
    }
    if (!noprintflag) {
      printf("send8 : r%d = ",r1);
      print8(gpr[r1]);
    }
    pc++;
    send8_count++;
    break;
  case OP_RECV8:
    printf("recv8(hexで入力)>");
    scanf("%x",&recvdata);
    print8(recvdata);
    gpr[r1]=(gpr[r1]&0xffffff00)|recvdata;
    if (!noprintflag) {
      printf("recv8 : r%d\n",r1);
    }
    pc++;
    recv8_count++;
    break;
  case OP_FST:
    memory[gpr[r1]]=fpr[r2].i;
    if (!noprintflag) {
      printf("fst : mem[r%d] <- f%d\n",r1,r2);
    }
    pc++;
    fst_count++;
    break;
  case OP_FLD:
    fpr[r2].i=memory[gpr[r1]];
    if (!noprintflag) {
      printf("fld : f%d <- mem[r%d]\n",r2,r1);
    }
    pc++;
    fld_count++;
    break;
  case OP_FMOV:
    fpr[r1].i=fpr[r2].i;
    if (!noprintflag) {
      printf("fmov : f%d <- f%d\n",r1,r2);
    }
    pc++;
    fmov_count++;
    break;
  default:
    printf("Unknown instruction\n");
    pc++;
    break;
  }
}
