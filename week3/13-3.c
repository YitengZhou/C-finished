#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  FILE *fp;
  int i;
  char arr[6];
  if ((fp = fopen("doof","rb"))==NULL){
    printf("Cannot open file\n");
    return 1;
  }
  fread (arr,sizeof(char),6,fp);
  for (i=0;i<5;i++)
    printf("Character %d was %d\n",i,arr[i]);
  fclose(fp);
  return 0;
}
