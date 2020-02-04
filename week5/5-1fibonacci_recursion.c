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
  if (n==1) return 1;
  if (n==2) return 1;
  return (fibon(n-1)+fibon(n-2));
}
