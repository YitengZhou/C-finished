#include <stdio.h>

void explore(int x, int y, char mz[5][5]);

int main(void)
{
  char maze[6][5]={
    {'#','.','#','#','#'},
    {'#','.','.','.','#'},
    {'#','#','#','.','#'},
    {'#','.','.','.','#'},
    {'#','.','.','#','#'},
    {'#','X','#','#','#'},
  };
  int i,j,startx=0,starty=0;
  for (i=0;i<6;i++){
    for (j=0;j<5;j++){
      printf("%c",maze[i][j]);
      if (maze[i][j]=='X'){
        startx=i;
        starty=j;
      }
    }
    printf("\n");
  }
  explore(startx,starty,maze);
  for (i=0;i<6;i++){
    for (j=0;j<5;j++){
      printf("%c",maze[i][j]);
    }
    printf("\n");
  }
  return 0;
}

void explore(int x, int y, char mz[6][5])
{
  static cnt=0;
  if (x==0&&y==1) {printf("%d Find the way out\n",cnt++); return;}
  if (mz[x-1][y]=='.') {mz[x-1][y]='-';explore(x-1,y,mz); printf("The %d step is top\n",cnt++); return;}
  if (mz[x][y+1]=='.') {mz[x][y+1]='-';explore(x,y+1,mz); printf("The %d step is right\n",cnt++); return;}
  if (mz[x][y-1]=='.') {mz[x][y-1]='-';explore(x,y-1,mz); printf("The %d step is left\n",cnt++); return;}
  if (mz[x+1][y]=='.') {mz[x+1][y]='-';explore(x+1,y,mz); printf("The %d step is down\n",cnt++); return;}
}
