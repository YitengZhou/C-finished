#include <stdio.h>
#include <assert.h>

#define TALL 90
#define WIDE 150
#define GENERATION 100

void next_generation(int arr[TALL][WIDE]);
void print_out(int arr[TALL][WIDE]);
int inhabited_cells(int arr[TALL][WIDE],int m,int n);
int access(int arr[TALL][WIDE],int m,int n);
void test(void);

int main(void)
{
  int board[TALL][WIDE]=
            {{0,0,0,0,0},
             {0,0,1,0,0},
             {0,0,1,0,0},
             {0,0,1,0,0},
             {0,0,0,0,0},
  };
  int i;
  test();
  print_out(board);
  for (i=0;i<GENERATION;i++){
    next_generation(board);
    print_out(board);
  }
  return 0;
}

void print_out(int arr[TALL][WIDE])
{
  int i,j;
  static int cnt=0;
  printf("This %d generation\n",cnt);
  for (i=0;i<TALL;i++){
    for (j=0;j<WIDE;j++){
      printf("%d",arr[i][j]);
    }
    printf("\n");
  }
  cnt++;
}

void next_generation(int arr[TALL][WIDE])
{
  int next[TALL][WIDE];
  int i,j,live;
  for (i=0;i<TALL;i++){
    for (j=0;j<WIDE;j++){
      live=inhabited_cells(arr,i,j);
      switch (arr[i][j]){
      case 1:
        if (live==2 ||live==3){
          next[i][j]=1;}
        else {
          next[i][j]=0;
        }
      break;
      case 0:
        if (live==3)
          next[i][j]=1;
        else
          next[i][j]=0;
        }
    }
  }
  for (i=0;i<TALL;i++){
    for (j=0;j<WIDE;j++){
      arr[i][j]=next[i][j];
    }
  }
}

int inhabited_cells(int arr[TALL][WIDE],int m,int n)
{
  int i,j,number=0;
  for (i=-1;i<=1;i++){
    for (j=-1;j<=1;j++){
      if ((access(arr,i+m,j+n)==1) && (i!=0 || j!=0))
      {number++;}
    }
  }
  return number;
}

int access(int arr[TALL][WIDE],int m,int n)
{
  if (m<0) return 0;
  if (m>=TALL) return 0;
  if (n<0) return 0;
  if (n>=WIDE) return 0;
  return arr[m][n];
}

void test(void)
{
  int a[TALL][WIDE]={{0,0,0,0,0},
                     {0,0,1,0,0},
                     {0,0,1,0,0},
                     {0,0,1,0,0},
                     {0,0,0,0,0},};
  assert(inhabited_cells(a,0,0)==0);
  assert(inhabited_cells(a,0,1)==1);
  assert(inhabited_cells(a,0,2)==1);
  assert(inhabited_cells(a,1,2)==1);
  assert(inhabited_cells(a,2,2)==2);
  assert(inhabited_cells(a,2,3)==3);
  assert(inhabited_cells(a,3,3)==2);
}
