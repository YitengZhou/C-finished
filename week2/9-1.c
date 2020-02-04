#include <stdio.h>

#define N 5

int main(void)
{
  int a[N];
  int i,sum=0;

  for (i=0;i<N;i++)
  a[i]=7+i*i;

  for (i=0;i<N;i++)
  printf("a=[%d] = %d ",i,a[i]);

  for (i=0;i<N;i++)
  sum+=a[i];

  printf("\nsum=%d\n",sum);

  return 0;
}
