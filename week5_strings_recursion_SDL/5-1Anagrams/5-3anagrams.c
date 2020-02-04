#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

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
  FILE *fp1,*fp2;
  char ch1,ch2;
  char first[100];
  int i=0,j=0,position=0,m;
  fp1=fopen("34words.txt","r");
  if (fp1==NULL){
    printf("Fail to open the file\n");
    exit(0);
  }

  while ((ch1=getc(fp1))!=EOF){
    if (ch1!='\n'){
      first[i]=ch1;
      i++;
    }
    else{
      j=i;
      position=j;
      i=0;
      fp2=fopen("34words.txt","r");
      while ((ch2=getc(fp2))!=EOF){
        if (ch2!='\n'){
          first[position]=ch2;
          position++;
        }
        else{
          first[position]='\0';
          position=j;
          if (isAnagrams(b,first))
          {
            first[i]=toupper(first[i]);
            first[position]=toupper(first[position]);
            printf("%s\n",first);
          }
          for (m=position;m<100;m++)
          {
            first[m]='\0';
          }
        }
      }
      fclose(fp2);
    }
  }
  fclose(fp1);
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
