#include <stdio.h>

void swap(int *a,int *b);

int main(void)
{
  int a =3,b=7;
  printf("%d %d\n",a,b);
  swap(&a,&b);
  printf("%d %d\n",a,b);
  return 0;
}

void swap(int *p,int *q)
{
  int temp;
  temp = *p;
  *p =*q;
  *q=temp;
}
