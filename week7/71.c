/* This program could resolve the Chinese puzzle with right order
  10 steps = "513276 48"
  20 steps = "8452 1736" (about 9s)
  28 steps = "87 216354" (about 2m 20s) */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* Max length of queue set 500000 */
#define ROW 3
#define WORD 10
#define MAX 500000

struct board {
  char arr[ROW][ROW];
  char str[WORD];
  int parent;
};
typedef struct board Board;

void Performance(char *string);
void Initial(Board *queue,char *string);
void Creat_board(Board *queue,int parent,int empty);
void Creat_string(Board *queue,int present);
int Next_generation(Board *queue,int parent,int empty,int space);
int Space_position(char arr[ROW][ROW]);
int In_array(Board *queue,char *string,int empty);
char Access(char arr[ROW][ROW],int movex,int movey);
void Swap(char *a,char *b);
void Print(Board *queue,int n);
void Print_Moving(Board *queue,int n);
void test(void);

int main(int argc,char **argv)
{
  test();
  if (argc==2){
    Performance(argv[1]);
  }
  else{
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s \"513276 48\"\n",argv[0]);
  }
  return 0;
}

/* Read in a board using argv[1], Make all possible boards,
  Print right order boards */
void Performance(char *string)
{
  static Board queue[MAX];
  int parent=0,target=1,flage=0,space;
  Initial(queue,string);
  while (strcmp(queue[target].str,"12345678 ")!=0){
    if (flage==0){
      space = Space_position(queue[parent].arr);
      flage = Next_generation(queue,parent,target,space);
      parent++;
    }
    else{
      target++;
      flage--;
    }
  }
  Print_Moving(queue,target);
}

/* Put the initial board into this queue */
void Initial(Board *queue,char *string)
{
  int i,j,n=0;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
      queue[0].arr[i][j]=string[n++];
    }
  }
  strcpy(queue[0].str,string);
  queue[0].parent=-1;
}

/* Creat a son board from the parent board */
void Creat_board(Board *queue,int parent,int empty)
{
  int i,j;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
      queue[empty].arr[i][j]=queue[parent].arr[i][j];
    }
  }
  queue[empty].parent=parent;
}

/* Creat string from board to use strcmp easier */
void Creat_string(Board *queue,int present)
{
  int i,j,k=0;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
      queue[present].str[k++]=queue[present].arr[i][j];
    }
  }
  queue[present].str[k]='\0';
}

/* Get next generation from parent board, flage range 0-4;
  flage(0): all son boards in cycles occur
  flage(4): all son boards (up,down,left,right) not in cycles occur */
int Next_generation(Board *queue,int parent,int empty,int space)
{
  int x,y,flage=0;
  x=(space-1)/ROW;
  y=(space-1)%ROW;
  /* up */
  if (Access(queue[parent].arr,x-1,y) !='0'){
    Creat_board(queue,parent,empty);
    Swap(&queue[empty].arr[x-1][y],&queue[empty].arr[x][y]);
    Creat_string(queue,empty);
    if (!In_array(queue,queue[empty].str,empty)){
      empty++;
      flage++;
    }
  }
  /* down */
  if (Access(queue[parent].arr,x+1,y) !='0'){
    Creat_board(queue,parent,empty);
    Swap(&queue[empty].arr[x+1][y],&queue[empty].arr[x][y]);
    Creat_string(queue,empty);
    if (!In_array(queue,queue[empty].str,empty)){
      empty++;
      flage++;
    }
  }
  /* right */
  if (Access(queue[parent].arr,x,y+1) !='0'){
    Creat_board(queue,parent,empty);
    Swap(&queue[empty].arr[x][y+1],&queue[empty].arr[x][y]);
    Creat_string(queue,empty);
    if (!In_array(queue,queue[empty].str,empty)){
      empty++;
      flage++;
    }
  }
  /* left */
  if (Access(queue[parent].arr,x,y-1) !='0'){
    Creat_board(queue,parent,empty);
    Swap(&queue[empty].arr[x][y-1],&queue[empty].arr[x][y]);
    Creat_string(queue,empty);
    if (!In_array(queue,queue[empty].str,empty)){
      empty++;
      flage++;
    }
  }
  return flage;
}

/* Return where space ' ' position (1-9) */
int Space_position(char arr[ROW][ROW])
{
  int i,j;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
      if (arr[i][j]==' '){
        return i*ROW+j+1;
      }
    }
  }
  return -1;
}

/* Identify repeated boards before put into the queue */
int In_array(Board *queue,char *string,int target)
{
  int i;
  for (i=0;i<target;i++){
    if (strcmp(string,queue[i].str)==0){
      return 1;
    }
  }
  return 0;
}

/* Avoid crossing the boundary */
char Access(char arr[ROW][ROW],int movex,int movey)
{
  if (movex<0||movey<0||movex>2||movey>2){
    return '0';
  }
  else {
    return arr[movex][movey];
  }
}

/* Swap the character a, b in board */
void Swap(char *a,char *b)
{
  char temp;
  temp=*a;
  *a=*b;
  *b=temp;
}

/* Print board in queue */
void Print(Board *queue,int n)
{
  int i,j;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
      printf("%c ",queue[n].arr[i][j]);
    }
    printf("\n");
  }
}

/* Step by step print the order board using recursion */
void Print_Moving(Board *queue,int n)
{
  static int cnt=0;
  if (n==0){
    printf("The %d step is\n",cnt++);
    Print(queue,n);
  }
  else {
    Print_Moving(queue,queue[n].parent);
    printf("The %d step is\n",cnt++);
    Print(queue,n);
  }
}

void test(void)
{
  Board test[10];
  char a[ROW][ROW]={{'1','2','3'},{'4','5','6'},{'7','8',' '}};
  char b[ROW][ROW]={{'1','2','3'},{'4',' ','5'},{'6','7','8'}};
  char c[ROW][ROW]={{'1','2','3'},{'4','5','6'},{'7','8','9'}};
  int flage;
/* test function Initial with correct initial board */
  Initial(test,"1234 5678");
  assert(test[0].arr[0][0]=='1');
  assert(test[0].arr[0][1]=='2');
  assert(test[0].arr[0][2]=='3');
  assert(test[0].arr[1][0]=='4');
  assert(test[0].arr[1][1]==' ');
  assert(test[0].arr[1][2]=='5');
  assert(test[0].arr[2][0]=='6');
  assert(test[0].arr[2][1]=='7');
  assert(test[0].arr[2][2]=='8');
  assert(test[0].parent==-1);

/* test function Space_position with correct position */
  assert(Space_position(a)==9);
  assert(Space_position(b)==5);
  assert(Space_position(c)==-1);
  assert(Space_position(test[0].arr)==5);

/* test function Next_generation (in this case, flage=4)
  NEXT queue will creat 4 boards because space in middle(5)
  parent = {{'1','2','3'},
            {'4',' ','5'},
            {'6','7','8'}} */
  flage=Next_generation(test,0,1,5);
  assert(flage==4);
  /* up */
  assert(Space_position(test[1].arr)==2);
  assert(test[1].arr[0][1]==' ');
  assert(test[1].arr[1][1]=='2');
  assert(test[1].parent==0);
  /* down */
  assert(Space_position(test[2].arr)==8);
  assert(test[2].arr[2][1]==' ');
  assert(test[2].arr[1][1]=='7');
  assert(test[2].parent==0);
  /* right */
  assert(Space_position(test[3].arr)==6);
  assert(test[3].arr[1][2]==' ');
  assert(test[3].arr[1][1]=='5');
  assert(test[3].parent==0);
  /* left */
  assert(Space_position(test[4].arr)==4);
  assert(test[4].arr[1][0]==' ');
  assert(test[4].arr[1][1]=='4');
  assert(test[4].parent==0);

/* test function In_array with correct return */
  assert(In_array(test,"1234 5678",5)==1);
  assert(In_array(test,"1 3425678",5)==1);
  assert(In_array(test,"1234756 8",5)==1);
  assert(In_array(test,"123 45678",5)==1);
  assert(In_array(test,"12345 678",5)==1);
  assert(In_array(test," 12345678",5)==0);
  assert(In_array(test,"12345678 ",5)==0);

/* test function Access */
  assert(Access(test[0].arr,0,0)=='1');
  assert(Access(test[0].arr,2,2)=='8');
  assert(Access(test[0].arr,-1,0)=='0');
  assert(Access(test[0].arr,-1,-1)=='0');
  assert(Access(test[0].arr,3,2)=='0');
}
