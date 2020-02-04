#include <stdio.h>
#include <string.h>

#define SWAP(A,B) {char temp; temp=A; A=B;B=temp;}

void Reverse_String (char *a,int n);

int main(void)
{
  char str[]="Hello World";
  Reverse_String(str,strlen(str));
  printf("%s\n",str);
  return 0;
}

void Reverse_String (char *a,int n)
{
  int i,j;
  for (i=0,j=n-1;i<j;i++,j--) {
    SWAP(a[i],a[j]);
    printf("%s\n",a);
  }
}
