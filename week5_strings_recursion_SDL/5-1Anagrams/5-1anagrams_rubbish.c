#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void permute(char str[],int l,int r);
void swap(char *a,char *b);
int compare(char *b);
void test(void);

int main(int argc,char **argv)
{
  int n=0;
  test();
  if (argc==2)
  {
    n=(int)strlen(argv[1]);
    permute(argv[1],0,n-1);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s sternaig\n",argv[0]);
  }
  return 0;
}

void permute(char str[],int l,int r)
{
  int i;
  if (l==r) {
    if (compare(str)==1){
    printf("%s\n",str);}
    return;
  }
  else{
    for (i=l;i<=r;i++) {
      if (str[i]==str[l]&& i!=l)
        continue;
      swap(str+l,str+i);
      permute(str,l+1,r);
      swap(str+l,str+i);
    }
  }
}

void swap(char *a,char *b)
{
  char temp;
  temp=*a;
  *a=*b;
  *b=temp;
}

int compare(char *b)
{
  FILE *fp;
  char ch;
  char a[30]={'\0'};
  int i=0;
  fp=fopen("5-1anagrams_rubbish.txt","r");
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
      if (strcmp(a,b)==0) {
        return 1;
      }
    }
  }
  fclose(fp);
  return 0;
}

void test(void)
{
  assert(compare("rangiest")==1);
}
