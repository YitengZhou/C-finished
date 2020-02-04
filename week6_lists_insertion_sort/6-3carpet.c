#include <stdio.h>

#define ROW 81

void Print(char board[ROW][ROW]);
char change(int x,int y);

int main(void)
{
  char board[ROW][ROW]={{'0'}};
  int i,j;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
      board[i][j]=change(i,j);
    }
  }
  Print(board);
  return 0;
}

void Print(char board[ROW][ROW])
{
  int i,j;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
      printf("%c",board[i][j]);
    }
    printf("\n");
  }
}

char change(int x,int y)
{
  while (x > 0 || y > 0){
    if (x % 3 == 1 && y % 3 == 1) return '#';
    x/=3;
    y/=3;
  }
  return ' ';
}
