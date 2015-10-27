#include<stdio.h>
#include<stdint.h>

int main(int argc,char *argv[])
{
  FILE *fpin,*fpout;
  char buf[1024];
  uint32_t inst;
  int i,j;

  if (argc!=3) {
    printf("usage : ./asm [input file] [output file]\n");
    return 1;
  }

  fpin=fopen(argv[1],"r");
  if (fpin==NULL) {
    printf("can't open file : %s\n",argv[1]);
    return 1;
  }

  fpout=fopen(argv[2],"wb");
  if (fpout==NULL) {
    printf("can't open file : %s\n",argv[2]);
    return 1;
  }

  while (fscanf(fpin,"%s",buf)!=EOF) {
    for (j=0;j<4;j++) {
      inst=0;
      for (i=0;i<8;i++) {
	if (buf[8*j+i]=='1') {
	  inst+=1<<(7-i);
	}
      }
      
      fwrite(&inst,1,1,fpout);
    }
  }

  fclose(fpin);
  fclose(fpout);
  return 0;
}
