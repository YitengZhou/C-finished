#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define MAX 300000
#define SCORE 20

/* MIN[SCORE]={7,18,5,9,19,2,10,20,1,12,17,3,13,14,4,15,11,6,16,8};*/

int total(int a[]);
void test(void);
void swap(int *a,int *b);

int main(void)
{
  int board[SCORE];
  int i,change1,change2,min=MAX;
  int mid[SCORE]={7,18,5,9,19,3,10,20,1,12,17,2,14,13,4,15,11,6,16,8};
  srand(time(0));
  for (i=0;i<SCORE;i++) {
    board[i]=i+1;
  }
  test();

  for (i=0;i<5000000;i++){
    change1=rand()%20;
    change2=rand()%20;
    swap(board+change1,board+change2);
    if (total(board)<min){
      min=total(board);
      printf("%d\n",min);
    }
    else
    {
      swap(board+change1,board+change2);
    }
  }
  for (i=0;i<SCORE;i++) {
    printf("%d ",board[i]);
  }
  printf("%d\n",min);
  printf("%d\n",total(mid));

  for (i=0;i<5000000;i++){
    change1=rand()%20;
    change2=rand()%20;
    swap(mid+change1,mid+change2);
    if (total(mid)<min){
      min=total(mid);
      printf("%d\n",min);
    }
    else
    {
      swap(mid+change1,mid+change2);
    }
  }

  printf("%d\n",min);
  for (i=0;i<SCORE;i++) {
    printf("%d ",mid[i]);
  }
  return 0;
}

int total(int a[])
{
  int i,n,sum=0;
  for (i=0;i<SCORE;i++){
    n=a[i]+a[(i+1)%SCORE]+a[(i+2)%SCORE];
    sum +=n*n;
  }
  return sum;
}

void test(void)
{
  int a[SCORE]={3,19,11,2,18,12,1,20,10,4,16,8,14,5,13,15,6,7,17,9};
  int b[SCORE]={3,18,10,5,16,9,8,14,11,4,19,6,7,20,2,13,15,1,17,12};
  assert(total(a)==19966);
  assert(total(b)==19910);
}

void swap(int *a,int *b)
{
  int temp;
  temp=*a;
  *a=*b;
  *b=temp;
}
