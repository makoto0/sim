#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdint.h>
#include"cpu.h"

#define MAXBUF 1024

int command_input()
{
  return 0;
}

void run()
{
  while(1) {
    if (memory[pc]==OP_NOP) {
      break;
    }

    exec_inst(memory[pc]);

    while (command_input()) ;
  }
}

void printbin(uint32_t i) /* debug */
{
  int k;

  for (k=31;k>=0;k--) {
    printf("%d",(i>>k)&1);
  }
  printf("\n");
}

void print_reg()
{
  int i;

  for (i=0;i<GPR_NUM;i++) {
    printf("GPR %2d : %d\n",i,gpr[i]);
  }

  for (i=0;i<FPR_NUM;i++) {
    printf("FPR %2d : ",i);
    printbin(fpr[i].i);
  }
}

void readinst(FILE* fp)
{
  uint32_t inst;
  uint32_t instnum=0;

  while (fread(&inst,sizeof(uint32_t),1,fp)>0) {
    printbin(inst); /* debug */
    memory[instnum]=inst;
    instnum++;
  }
}

int main(int argc,char* argv[])
{
  FILE* fp;
  int option;

  if (argc<2) {
    printf("usage: %s [options] filename\n",argv[0]);
    return 1;
  }

  while ((option=getopt(argc,argv,"h"))!=-1) {
    switch (option) {
    case 'h':
      printf("usage: %s [options] filename\n",argv[0]);
      printf("option:\n");
      printf("         -h:help\n");
      return 0;
    default:
      printf("Unknown option\n");
    }
  }

  if (argv[optind]==NULL) {
    printf("No input file\n");
    return 1;
  }

  fp=fopen(argv[optind],"rb");
  if (fp==NULL) {
    printf("can't open file : %s\n",argv[optind]);
    return 1;
  }

  readinst(fp);

  fclose(fp);

  run();

  print_reg();

  return 0;
}
