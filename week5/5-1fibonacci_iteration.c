#include <stdio.h>

int fibon(int n);

int main(void)
{
  int i;
  for (i=1;i<=20;i++) {
    printf("%d = %d\n",i,fibon(i));
  }
  return 0;
}

int fibon(int n)
{
  int i, a, b, temp;
  if (n==1) return 1;
  if (n==2) return 1;
  a=1;b=1;
  for (i=3;i<=n;i++) {
    temp=a;
    a=b;
    b=a+temp;
  }
  return b;
}
