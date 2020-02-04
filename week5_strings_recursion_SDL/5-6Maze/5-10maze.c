/* This maze.c program could read maze from file and find different way to escape */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* The max size of maze is 20*20 */
#define SIZE 20

void Run(char *file_name);
void Read_from_file(char *file_name,char maze[SIZE][SIZE],int input_maze[]);
void Initialize_maze(char maze[SIZE][SIZE],int input_maze[]);
int Find_way(char maze[SIZE][SIZE],int x,int y,int input_maze[]);
char Access(char maze[SIZE][SIZE],int x,int y,int input_maze[]);
void Print_maze(char maze[SIZE][SIZE],int input_maze[]);
void Result(int result);
void test(void);

int main(int argc,char **argv)
{
  if (argc==2) {
    Run(argv[1]);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s maze1.txt\n",argv[0]);
  }
  return 0;
}

/* Start to escape! */
void Run(char *file_name)
{
  char maze[SIZE][SIZE];
/* input_maze means the size of maze from file,
    [0] is width, [1] is height
    [2] is startx, [3] is starty */
  int input_maze[4];
  test();
  Read_from_file(file_name,maze,input_maze);
  Initialize_maze(maze,input_maze);
  printf("The maze from file:\n");
  Print_maze(maze,input_maze);
  Result(Find_way(maze,input_maze[2],input_maze[3],input_maze));
}

/* Read the first line of the file (width,height) and maze */
void Read_from_file(char *file_name,char maze[SIZE][SIZE],int input_maze[])
{
  FILE *fp;
  int i,j,m=0,n=0;
  char ch,new_line;
  for (i=0;i<SIZE;i++) {
    for (j=0;j<SIZE;j++) {
      maze[i][j]=' ';
    }
  }
  fp=fopen(file_name,"r");
  if (fp==NULL){
    printf("Failed to open the file\n");
    exit(0);
  }
  fscanf(fp,"%d %d%c",&input_maze[0],&input_maze[1],&new_line);
  while ((ch=getc(fp)) !=EOF) {
    if (ch=='\n') {
      m++;
      n=0;
    }
    else {
      maze[m][n]=ch;
      n++;
    }
  }
  fclose(fp);
}
/* Initialize maze with 'S' for start and storage this point in array */
void Initialize_maze(char maze[SIZE][SIZE],int input_maze[])
{
  int i=0,j=0;
  while (maze[i][j]!=' ')
  {
    i++;
    input_maze[2]=i; /* input_maze[2] is startx */
    input_maze[3]=j; /* input_maze[3] is starty */
  }
  maze[input_maze[2]][input_maze[3]]='S'; /*Start point marked 'S'*/
}

/* If the program succeeds with exit(0), no route exists with exit(1) */
void Result(int result)
{
  if (result>0) {
    printf("Totally, find %d ways to go out\n",result);
    exit(0);
  }
  else {
    printf("There is no route to go out\n");
    exit(1);
  }
}

/* Find the way out using recursion */
int Find_way(char maze[SIZE][SIZE],int x,int y,int input_maze[])
{
  static int cnt=0;
  if (y==input_maze[0]-1)
  {
    maze[x][y]='E'; /*The exit point marked 'S'*/
    printf("The %d way:\n",++cnt);
    Print_maze(maze,input_maze);
    maze[x][y]=' ';
  }
  if (Access(maze,x+1,y,input_maze)==' ') {maze[x+1][y]='.';Find_way(maze,x+1,y,input_maze);maze[x+1][y]=' ';}
  if (Access(maze,x-1,y,input_maze)==' ') {maze[x-1][y]='.';Find_way(maze,x-1,y,input_maze);maze[x-1][y]=' ';}
  if (Access(maze,x,y+1,input_maze)==' ') {maze[x][y+1]='.';Find_way(maze,x,y+1,input_maze);maze[x][y+1]=' ';}
  if (Access(maze,x,y-1,input_maze)==' ') {maze[x][y-1]='.';Find_way(maze,x,y-1,input_maze);maze[x][y-1]=' ';}
  return cnt;
}

/* Avoid crossing the boundary */
char Access(char maze[SIZE][SIZE],int x,int y,int input_maze[])
{
  if (x<0||y<0||x>=input_maze[1]||y>=input_maze[0]) return '#';
  else return maze[x][y];
}

/* Print the maze with size (width,height) */
void Print_maze(char maze[SIZE][SIZE],int input_maze[])
{
  int i,j;
  for (i=0;i<input_maze[1];i++) {
    for (j=0;j<input_maze[0];j++) {
      printf ("%c",maze[i][j]);
    }
    printf ("\n");
  }
}

void test(void)
{
  int a[4]={10,10,1,0};
  char b[SIZE][SIZE]={{'*'}};
  assert(Access(b,-1,12,a)=='#');
  assert(Access(b,10,-10,a)=='#');
  assert(Access(b,5,5,a)=='\0');
  assert(Access(b,0,0,a)=='*');

  assert(Access(b,15,15,a)=='#');
  a[0]=20;a[1]=20;
  assert(Access(b,15,15,a)=='\0');

  b[0][0]='#';b[0][1]='#';b[0][2]='#';
  b[1][0]=' ';b[1][1]=' ';b[1][2]=' ';
  b[2][0]='#';b[2][1]='#';b[2][2]='#';
/* b= { {'#','#','#'},
        {' ','#',' '},
        {'#','#','#'}}; */
  a[0]=3;a[1]=3;
  assert((b[1][0])==' ');
  Initialize_maze(b,a);
  assert(a[2]==1);
  assert(a[3]==0);
  assert((b[1][0])=='S');
}
