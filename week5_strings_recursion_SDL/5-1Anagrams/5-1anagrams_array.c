#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void performance(char *b);
int isAnagrams(char *a,char *b);

int main(int argc,char **argv)
{
  if (argc==2)
  {
    performance(argv[1]);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s sternaig\n",argv[0]);
  }
  return 0;
}

void performance(char *b)
{
  FILE *fp;
  char ch;
  char a[50];
  int i;
  fp=fopen("eng_370k_shuffle.txt","r");
  if (fp==NULL){
    printf("Fail to open the file\n");
    exit(0);
  }

  while ((ch=getc(fp))!=EOF){
    if (ch!='\n'){
      a[i]=ch;
      i++;
    }
    else{
      a[i]='\0';
      i=0;
      if (isAnagrams(a,b)==1){
        printf("%s\n",a);
      }
    }
  }
  fclose(fp);
}

int isAnagrams(char *a,char *b)
{
  int origin[26]={0};
  int word[26]={0};
  int i,j;
  for (i=0;i<(int)strlen(a);i++){
    origin[a[i]-'a']++;
  }
  for (j=0;j<(int)strlen(b);j++){
    word[b[j]-'a']++;
  }
  for (i=0;i<26;i++){
    if (origin[i]!=word[i])
    return 0;
  }
  return 1;
}
