#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 20

void shuffle(int a[],int number);

int main (void)
{
  int i;
  int list[N]={0};
  srand(time(0));
  for (i=0;i<N;i++)
  list[i]=i+1;
  shuffle(list,N);
  for (i=0;i<N;i++)
  printf("%d ",list[i]);
  return 0;
}

void shuffle(int a[],int number)
{
  int *p,temp,i,random_number;
  p=a;
  for (i=0;i<number;i++)
  {
    random_number=rand()%number;
    temp=*(p+random_number);
    *(p+random_number)=*(p+i);
    *(p+i)=temp;
  }
}
