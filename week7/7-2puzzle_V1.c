#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define ROW 3
#define WORD 10
#define MAX 500000

struct board {
  int number;
  char arr[ROW][ROW];
  char str[WORD];
  int parent;
};
typedef struct board Board;

/*20 steps = "8452 1736" (about 9s), 28 steps = "87 216354" (about 2m 20s)*/

void Performance(char *string);
void Initial(Board *a,char *string);
int Space_position(char arr[ROW][ROW]);
int Next_generation(Board *a,int parent,int empty,int space);
void Creat_board(Board *a,int parent,int number);
void Creat_string(Board *a,int present);
int In_array(Board *a,char *string,int empty);
char Access(char arr[ROW][ROW],int movex,int movey);
void Swap(char *a,char *b);
void Print(Board *a,int n);
void Print_Moving(Board *a,int n);
void test(void);

int main(int argc,char **argv)
{
  test();
  if (argc==2)
  {
    Performance(argv[1]);
  }
  else {
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s \"513276 48\"\n",argv[0]);
  }
  return 0;
}

void Performance(char *string)
{
  static Board a[MAX];
  int parent=0,empty=1,flage=0;
  Initial(a,string);
  while (strcmp(a[empty].str,"12345678 ")!=0){
    if (flage==0){
      flage = Next_generation(a,parent,empty,Space_position(a[parent].arr));
      parent++;
    }
    else {
      empty++;
      flage--;
    }
  }
  Print_Moving(a,empty);
}

void Initial(Board *a,char *string)
{
  int i,j,n=0;
  for (i=0;i<3;i++){
    for (j=0;j<3;j++){
      a[0].arr[i][j]=(string[n++]);
    }
  }
  strcpy(a[0].str,string);
  a[0].number=0;
  a[0].parent=-1;
}

void Creat_board(Board *a,int parent,int cnt)
{
  int i,j;
  for (i=0;i<3;i++){
    for (j=0;j<3;j++){
      a[cnt].arr[i][j]=a[parent].arr[i][j];
    }
  }
  a[cnt].number=cnt;
  a[cnt].parent=parent;
}

void Creat_string(Board *a,int present)
{
  int i,j,k=0;
  for (i=0;i<3;i++){
    for (j=0;j<3;j++){
      a[present].str[k++]=a[present].arr[i][j];
    }
  }
  a[present].str[k]='\0';
}

int In_array(Board *a,char *string,int empty)
{
  int i;
  for (i=0;i<empty;i++){
    if (strcmp(string,a[i].str)==0){
      return 1;
    }
  }
  return 0;
}

int Next_generation(Board *a,int parent,int empty,int space)
{
  /*left*/
  int x,y,flage=0;
  x=(space-1)/3;
  y=(space-1)%3;
  if (Access(a[parent].arr,x-1,y) !='0'){
    Creat_board(a,parent,empty);
    Swap(&a[empty].arr[x-1][y],&a[empty].arr[x][y]);
    Creat_string(a,empty);
    if (!In_array(a,a[empty].str,empty)){
      empty++;
      flage++;
    }
  }
  /*right*/
  if (Access(a[parent].arr,x+1,y) !='0'){
    Creat_board(a,parent,empty);
    Swap(&a[empty].arr[x+1][y],&a[empty].arr[x][y]);
    Creat_string(a,empty);
    if (!In_array(a,a[empty].str,empty)){
      empty++;
      flage++;
    }
  }
  /*down*/
  if (Access(a[parent].arr,x,y+1) !='0'){
    Creat_board(a,parent,empty);
    Swap(&a[empty].arr[x][y+1],&a[empty].arr[x][y]);
    Creat_string(a,empty);
    if (!In_array(a,a[empty].str,empty)){
      empty++;
      flage++;
    }
  }
  /*up*/
  if (Access(a[parent].arr,x,y-1) !='0'){
    Creat_board(a,parent,empty);
    Swap(&a[empty].arr[x][y-1],&a[empty].arr[x][y]);
    Creat_string(a,empty);
    if (!In_array(a,a[empty].str,empty)){
      empty++;
      flage++;
    }
  }
  return flage;
}

void Swap(char *a,char *b)
{
  char temp;
  temp=*a;
  *a=*b;
  *b=temp;
}

char Access(char arr[ROW][ROW],int movex,int movey)
{
  if (movex<0||movey<0||movex>2||movey>2){
    return '0';
  }
  else {
    return arr[movex][movey];
  }
}

int Space_position(char arr[ROW][ROW])
{
  int i,j;
  for (i=0;i<3;i++){
    for (j=0;j<3;j++){
      if (arr[i][j]==' ')
      {
        return i*3+j+1;
      }
    }
  }
  return -1;
}

void Print(Board *a,int n)
{
  int i,j;
  for (i=0;i<3;i++){
    for (j=0;j<3;j++){
      printf("%c ",a[n].arr[i][j]);
    }
    printf("\n");
  }
  printf("my string is \"%s\"\n",a[n].str);
  /*printf("my number is %d\n",a[n].number);
  printf("my parent is %d\n",a[n].parent);*/
}

void Print_Moving(Board *a,int n)
{
  static int cnt=0;
  if (n==0){
    printf("The %d step is\n",cnt++);
    Print(a,n);
  }
  else {
    Print_Moving(a,a[n].parent);
    printf("\nThe %d step is\n",cnt++);
    Print(a,n);
  }
}

void test(void)
{
  char b[3][3]={{'1','2','3'},{'4','5','6'},{'7','8',' '}};
  char c[3][3]={{'1','2','3'},{'4',' ','5'},{'6','7','8'}};
  assert(Space_position(b)==9);
  assert(Space_position(c)==5);
}
