#include <stdio.h>

int main(void)
{
  int i,number;
  int max,cnt,maxcnt=1;
  int Nmax,Ncnt=0;
  max=1;
  Nmax=1;
  for (number=1;number<50000;number++)
  {
    cnt=1;
    i=number;

  while (i!=1)
    {
    cnt++;
    if (i%2==0)
    {i=i/2;
    printf("%d ",i);}
    else{
      i=3*i+1;
      printf("%d ",i);
      if (i>max)
        {max=i;
        Nmax=number;}}
    }

    printf("\n");
    if (cnt>maxcnt)
    {maxcnt=cnt;Ncnt=number;}

  }

  printf("the largest number is %d, largest is %d\n",Nmax,max);
  printf("the longest number is %d, long is %d\n",Ncnt,maxcnt);
  return 0;
}
