#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 40
void figure(int a[],int b[]);

int main(void)
{
  int i;
  int list2[N]={0};
  int list[N]={0};
  int select,number;
  srand(time(0));
  printf("Please selected 1.randomised first line or 2. single 1 in the first line\n");
  scanf("%d",&select);
  if (select==1)
  {
    for (i=0;i<N;i++)
    {
      number=rand()%2;
      list[i]=number;
    }
    figure(list,list2);
  }
  if (select==2)
  {
    number=rand()%N;
    list[number]=1;
    figure(list,list2);
  }

  return 0;
}

void figure(int a[],int b[])
{
  int i,j,cnt=1;
  for (i=0;i<N;i++)
  printf("%d ",a[i]);
  printf("\n");
  for (j=0;j<N;j++)
  {
      for (i=1;i<N-1;i++)
    {
      if (cnt%2==1)
      {
        if ((a[i-1]+a[i]+a[i+1])==0 || (a[i-1]+a[i]+a[i+1])==3||(a[i-1]-a[i]>a[i+1]))
          b[i]=0;
        else
          b[i]=1;
      }
      else
      {
        if ((b[i-1]+b[i]+b[i+1])==0 || (b[i-1]+b[i]+b[i+1])==3||(b[i-1]-b[i]>b[i+1]))
          a[i]=0;
        else
          a[i]=1;
      }
    }
    if (cnt%2==1)
      {for (i=0;i<N;i++)
      printf("%d ",b[i]);}
    else
      {for (i=0;i<N;i++)
      printf("%d ",a[i]);}
      printf("\n");
    cnt++;
  }
}
