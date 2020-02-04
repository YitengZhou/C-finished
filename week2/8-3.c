#include <stdio.h>

int f(void);

int main(void)
{
  int i;
  static int cnt;
  for (i=0;i<10;i++) cnt=f();
  printf("%d\n",cnt);
  return 0;
}


int f(void)
{
  static int cnt=0;
  cnt+=2;
  return cnt;
}
