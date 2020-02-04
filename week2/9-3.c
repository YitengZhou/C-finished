#include<stdio.h>

void zero1(int b[],int s);
void zero2(int *b,int s);

int main(void)
{
  int a[5],i;
  for (i=0;i<5;i++)
  printf("a[%d]=%d\n",i,a[i]);
  zero1(a,5);
  for (i=0;i<5;i++)
  printf("a[%d]=%d\n",i,a[i]);
  zero2(&a[1],3);
  for (i=0;i<5;i++)
  printf("a[%d]=%d\n",i,a[i]);
  return 0;
}

void zero1(int b[],int s)
{
int i;
  for (i=0;i<s;i++)
  b[i]=0;
}
void zero2(int *b,int s)
{
    int i;
    for (i=0;i<s;i++)
    *b++=2;
}
