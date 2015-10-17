#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include"cpu.h"

#define MAXBUF 1024

int stepflag=0;
int breakpoint[MEM_NUM]={};

void printbin(uint32_t i)
{
  int k;

  for (k=31;k>=0;k--) {
    printf("%d",(i>>k)&1);
  }
  printf("\n");
}

void printfloat(uint32_t i)
{
  int k;

  for (k=31;k>=0;k--) {
    printf("%d",(i>>k)&1);
    if (k==31 || k==23) {
      printf(" ");
    }
  }
  printf("\n");
}

void command_input()
{
  char buf[MAXBUF];
  char *tok;
  int regnum;
  uint32_t addr;

  while (1) {
    printf(">");

    fgets(buf,MAXBUF,stdin);

    tok=strtok(buf," \n");

    if (tok==NULL || strcmp(tok,"s")==0 || strcmp(tok,"step")==0) {
      stepflag=1;
      break;
    } else if (strcmp(tok,"r")==0 || strcmp(tok,"run")==0) {
      stepflag=0;
      break;
    } else if (strcmp(tok,"b")==0) {
      tok=strtok(NULL," \n");
      if (tok==NULL) {
	puts("Please enter breakpoint address.");
      } else {
	addr=atoi(tok);
	if (addr>=0 && addr<MEM_NUM) {
	  breakpoint[addr]=1;
	  printf("set breakpoint : ");
	  printbin(addr);
	} else {
	  puts("Invalid memory address.");
	}
      }
    } else if (strcmp(tok,"db")==0) {
      tok=strtok(NULL," \n");
      if (tok==NULL) {
	puts("Please enter breakpoint address.");
      } else {
	addr=atoi(tok);
	if (addr>=0 && addr<MEM_NUM) {
	  breakpoint[addr]=0;
	  printf("delete breakpoint : ");
	  printbin(addr);
	} else {
	  puts("Invalid memory address.");
	}
      }
    } else if (strcmp(tok,"pg")==0) {
      tok=strtok(NULL," \n");
      if (tok==NULL) {
	puts("Please enter the register number.");
      } else {
	regnum=atoi(tok);
	if (regnum>=0 && regnum<GPR_NUM) {
	  printf("GPR %d : %d\n",regnum,gpr[regnum]);
	} else {
	  puts("Invalid register number.");
	}
      }
    } else if (strcmp(tok,"pf")==0) {
      tok=strtok(NULL," \n");
      if (tok==NULL) {
	puts("Please enter the register number.");
      } else {
	regnum=atoi(tok);
	if (regnum>=0 && regnum<FPR_NUM) {
	  printf("FPR %d : ",regnum);
	  printfloat(fpr[regnum].i);
	} else {
	  puts("Invalid register number.");
	}
      }
    } else if (strcmp(tok,"pm")==0) {
      tok=strtok(NULL," \n");
      if (tok==NULL) {
	puts("Please enter the memory address.");
      } else {
	addr=atoi(tok);
	if (addr>=0 && addr<MEM_NUM) {
	  printf("memory %d : ",addr);
	  printbin(memory[addr]);
	} else {
	  puts("Invalid memory address.");
	}
      }
    } else if (strcmp(tok,"h")==0 || strcmp(tok,"help")==0) {
      puts("commands");
      puts("h : help");
      puts("r : run");
      puts("s : step");
      puts("b [addr] : set breakpoint [addr]");
      puts("db [addr] : delete breakpoint [addr]");
      puts("pg [n] : print GPR [n]");
      puts("pf [n] : print FPR [n]");
      puts("pm [addr] : print memory [addr]");
    } else {
      puts("Unknown command.");
    }
  }
}

void run()
{
  while(1) {
    if (memory[pc]==HALT) {
      break;
    }

    if (breakpoint[pc]==1) {
      stepflag=1;
    }

    if (stepflag==1) {
      command_input();
    }

    exec_inst(memory[pc]);
  }
}

void print_reg()
{
  int i;

  for (i=0;i<GPR_NUM;i++) {
    printf("GPR %2d : %d\n",i,gpr[i]);
  }

  for (i=0;i<FPR_NUM;i++) {
    printf("FPR %2d : ",i);
    printfloat(fpr[i].i);
  }
}

void readinst(FILE* fp)
{
  uint32_t inst;
  uint32_t instnum=0;

  while (fread(&inst,sizeof(uint32_t),1,fp)>0) {
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

  while ((option=getopt(argc,argv,"hs"))!=-1) {
    switch (option) {
    case 'h':
      printf("usage: %s [options] filename\n",argv[0]);
      printf("options\n");
      printf("-h : help\n");
      printf("-s : step exec\n");
      return 0;
    case 's':
      stepflag=1;
      break;
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
