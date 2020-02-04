#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 30

void mutate(int a[N][N]);

int main(void)
{
  int random_board[N][N]={0};
  int i,j;
  srand(time(0));
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      random_board[i][j]=rand()%10;
    }
  }

  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      printf("%d",random_board[i][j]);
    }
    printf("\n");
  }

  for (i=0;i<N;i++) {
    mutate(random_board);
  }

  printf("\n");
  for (i=0;i<N;i++){
    for (j=0;j<N;j++){
      printf("%d",random_board[i][j]);
    }
    printf("\n");
  }

  return 0;
}

void mutate(int a[N][N])
{
  int i,j,temp=0;
  for (i=0;i<N;i++) {
    for (j=0;j<N-1;j++) {
      if (a[i][j]>a[i][j+1]) {
        temp=a[i][j];
        a[i][j]=a[i][j+1];
        a[i][j+1]=temp;
      }
    }
  }
  for (i=0;i<N-1;i++) {
    for (j=0;j<N;j++) {
      if (a[i][j]>a[i+1][j]) {
        temp=a[i][j];
        a[i][j]=a[i+1][j];
        a[i+1][j]=temp;
      }
    }
  }
}
