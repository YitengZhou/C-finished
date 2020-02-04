#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

void passwp(char *a,char *b);

int main(void)
{
  char b[100]={'\0'};
  passwp("AAss",b);
  assert(strcmp(b,"@@55")==0);
  passwp("ABCdefghijklmnopqrstuvwxyz",b);
  assert(strcmp(b,"@BCdefghijklmn0pqr5tuvwxyz")==0);
  passwp("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",b);
  assert(strcmp(b,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")==0);
  return 0;
}

void passwp(char *a,char *b)
{
  int l,i;
  l=strlen(a);
  for (i=0;i<l;i++) {
    switch (tolower(a[i]))
    {
      case 'a': b[i]='@';break;
      case 'o': b[i]='0';break;
      case 's': b[i]='5';break;
      default: b[i]=a[i];
    }
  }
}
