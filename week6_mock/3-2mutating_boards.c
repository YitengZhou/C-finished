#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 30

void mutate(int a[N][N]);
void min(int a[N][N],int m,int n);

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

  for (i=0;i<N*N*N*N;i++) {
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
  int x1,y1,x2,y2,dis1,dis2,temp;
  x1=rand()%N;
  y1=rand()%N;
  dis1=(x1-N/2)*(x1-N/2)+(y1-N/2)*(y1-N/2);
  x2=rand()%N;
  y2=rand()%N;
  dis2=(x2-N/2)*(x2-N/2)+(y2-N/2)*(y2-N/2);
  if (dis1<dis2 && a[x1][y1]>a[x2][y2]) {
    temp=a[x1][y1];
    a[x1][y1]=a[x2][y2];
    a[x2][y2]=temp;
  }
}
