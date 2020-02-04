#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  FILE *fp;
  int i;
  char arr[6];
  if ((fp = fopen("file.txt","w"))==NULL){
    printf("Cannot open file\n");
    return 1;
  }
  fprintf(fp,"Hello world\n");
  fprintf(stdout,"zhouyiteng");
  fclose(fp);
  return 0;
}
