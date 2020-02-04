#include <stdio.h>

#define DEPTH 25
#define GRID 5
#define STARTX 0
#define STARTY 0

int unlock(int a[GRID][GRID],int startx,int starty,int depth);
int access(int a[GRID][GRID],int x,int y);

int main(void)
{
  int board[GRID][GRID]={{0}};
  printf("There are %d patterns\n",unlock(board,STARTX,STARTY,1));
  return 0;
}

int unlock(int a[GRID][GRID],int x,int y,int depth)
{
  int i,j;
  static int cnt=0;

  if (depth==DEPTH){
    a[x][y]=DEPTH;
      printf("The %d pattern is:\n",++cnt);
      for (i=0;i<GRID;i++){
        for (j=0;j<GRID;j++){
          printf("%3d",a[i][j]);
        }
        printf("\n");
      }
    a[x][y]=0;
  }
  if (access(a,x-1,y)==0) {a[x][y]=depth;unlock(a,x-1,y,depth+1);a[x][y]=0;}
  if (access(a,x+1,y)==0) {a[x][y]=depth;unlock(a,x+1,y,depth+1);a[x][y]=0;}
  if (access(a,x,y+1)==0) {a[x][y]=depth;unlock(a,x,y+1,depth+1);a[x][y]=0;}
  if (access(a,x,y-1)==0) {a[x][y]=depth;unlock(a,x,y-1,depth+1);a[x][y]=0;}

  return cnt;
}

int access(int a[GRID][GRID],int x,int y)
{
  if (x<0||y>=GRID||x>=GRID||y<0){return 1;}
  else {return a[x][y];}
}
