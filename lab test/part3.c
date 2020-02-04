#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

char* pwdextend(char *a);

int main(void)
{
  char *p2,*p1;
  p1=pwdextend("password");
  p2=pwdextend("hackable");
  printf("%s-%s\n",p1,p2);
  assert(strcmp(p1,"password808")==0);
  assert(strcmp(p2,"hackable808")==0);
  return 0;
}

char* pwdextend(char *a)
{
  char list[20]={'\0'};
  char *p;
  int l;
  l=strlen(a);
  p=(char *)malloc(sizeof(list));
  strcat(p,a);
  strcat(p,"808");
  return p;
}

/*
char* pwdextend(char *a)
{
  char list[20]={'\0'};
  char *p;
  int i,l;
  l=strlen(a);
  p=(char *)malloc(sizeof(list));
  for (i=0;i<l;i++){
    *p++=a[i];
  }
  *p++='8';
  *p++='0';
  *p++='8';
  return p-l-3;
}*/
