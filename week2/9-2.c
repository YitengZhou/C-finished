#include <stdio.h>

int main(void)
{
  int a[]={10,12,6,7,2};
  int i;
  int sum=0;
  int *p;

  p=a;
  for (i=0;i<5;i++)
  {
    sum += *p;
    p++;
  }
  printf ("%d\n",sum);
  return 0;
}
