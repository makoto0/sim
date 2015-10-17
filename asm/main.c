#include<stdio.h>
#include<stdint.h>

int main(int argc,char *argv[])
{
  FILE *fpin,*fpout;
  char buf[1024];
  uint32_t inst;
  int i;

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
    inst=0;

    for (i=0;i<32;i++) {
      if (buf[i]=='1') {
	inst+=1<<(31-i);
      }
    }

    fwrite(&inst,sizeof(uint32_t),1,fpout);
  }

  fclose(fpin);
  fclose(fpout);
  return 0;
}
