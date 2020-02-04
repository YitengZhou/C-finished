/*copying FILES*/
#include <stdio.h>
#include <stdlib.h>

#define MAXFILENAME 123

int main (void)
{
  int sum=0,val,i;
  FILE *ifp,*ofp;
  char inpname[MAXFILENAME];
  char oupname[MAXFILENAME];
  printf("Enter two filename:\n");
  if (scanf("%s %s",inpname,oupname)!=2){
    printf("Failed to scan two filenames.");
    exit(1);
  }

  ifp=fopen(inpname,"rb");
  ofp=fopen(oupname,"wb");
  if (ifp==NULL ||ofp==NULL){
    printf("Failed to open two filenames.");
    exit(1);
  }

  while ((i=getc(ifp))!=EOF){
    putc(i,ofp);
  }

  fclose(ifp);
  fclose(ofp);

  return 0;
}
