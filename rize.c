#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdint.h>

#define MAXBUF 1024
#define LINE 33

FILE* fp;

int step()
{
  uint32_t inst;

  while (fscanf(fp,"%x",&inst)!=EOF) {
    printf("%x\n",inst);
  }

  return 0;
}
    


void run()
{
  char buf[MAXBUF]={};

  while(1) {
    while (step()) ;


    scanf("%s",buf);
    if (strcmp(buf,"exit")==0) {
      break;
    }
  }
}



int main(int argc,char* argv[])
{
  int option;

  if (argc<2) {
    printf("usage: %s [option] filename\n",argv[0]);
    return 1;
  }

  while ((option=getopt(argc,argv,"h"))!=-1) {
    switch (option) {
    case 'h':
      printf("usage: %s [option] filename\n",argv[0]);
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

  fp=fopen(argv[optind],"r");
  if (fp==NULL) {
    printf("can't open file %s\n",argv[optind]);
    return 1;
  }

  run();

  fclose(fp);
  return 0;
}
