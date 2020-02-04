#include <stdio.h>

#define M 3
#define N 4

int main (void)
{
  int a[M][N],i,j,sum=0;
  putchar('\n');
  for (i=0;i<M;i++)
    for (j=0;j<N;j++)
      a[i][j]=i+j;
  for (i=0;i<M;i++)
    {for (j=0;j<N;j++)
      printf("a[%d][%d]=%d ",i,j,a[i][j]);
      printf("\n");
    }
    for (i=0;i<M;i++)
      for (j=0;j<N;j++)
        sum+=a[i][j];
    printf("\nsum=%d\n\n",sum);
  return 0;
}
