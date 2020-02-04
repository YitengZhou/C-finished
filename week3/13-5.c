/*Number the lines of a file*/
#include <stdio.h>

int main (void)
{
  FILE *fp;
  char fname[128];
  char str[500];
  int i,c,line;

  printf("What file would you like to see?\n");
  scanf("%s",fname);
  if ((fp=fopen(fname,"r"))==NULL) {
    printf("Cannot open file\n");
    return 1;
  }
  line =1;
  do {
    i=0;
    do {
      c = (str[i++]=getc(fp));
    } while (c!=EOF && c!='\n');
    str[i]='\0';
    if (c!=EOF) {
    printf("%5d %s",line,str);
    line++;}
  } while (c!=EOF);
  fclose(fp);
  return 0;
}
