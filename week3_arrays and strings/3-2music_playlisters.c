#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

int main(void)
{
  int i,number,pop_number=0;
  int a[N]={0};
  srand(time(0));
  printf("How many songs required ?\n");
  scanf("%d",&number);
  if (number<N)
  {
    for (i=0;i<N;i++)
      a[i]=i;
    a[0]=number;
    while (a[0]!=0)
    {
      pop_number= rand()%number+1;
      if (a[pop_number]!=0){
      printf("%d ",pop_number);
      a[pop_number]=0;
      a[0]--;}
    }
  }
  else
  printf("Too much songs need shuffle, please reduce\n");
  return 0;
}
