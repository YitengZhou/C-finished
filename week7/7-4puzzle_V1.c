/* This program could resolve the Chinese puzzle using linked-list
  20 steps = "8452 1736" (about 9s)
  28 steps = "87 216354" (about 2m20s) */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* Max length of queue set 500000 */
#define ROW 3
#define WORD 10

struct node {
  int parent;
  char arr[ROW][ROW];
  char str[WORD];
  struct node *next;
};
typedef struct node Node;

void Performance(char *string);
Node* AllocateNode(char *string,int parent);
int Space_position(char arr[ROW][ROW]);
int Next_generation(Node *p,Node *start,int parent,int space);
int In_array(Node *start,char *string,Node *p);
char Access(char arr[ROW][ROW],int movex,int movey);
void Swap(char *a,char *b);
void Print(Node *p);
void Print_Moving(Node *p,Node *start);
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

/* Read in a board using argv[1], Make all possible boards,
  Print right order boards */
void Performance(char *string)
{
  Node *start,*current;
  int parent=0,flage=0,space;
  start=current=AllocateNode(string,-1);
  while (strcmp(current->str,"12345678 ")!=0){
    if (flage==0){
      space = Space_position(current->arr);
      flage = Next_generation(current,start,parent,space);
      parent++;
    }
    else {
      current=current->next;
      flage--;
    }
  }
  Print_Moving(current,start);
}

/* Put board into linked-list */
Node* AllocateNode(char *string,int parent)
{
  Node *p;
  int i,j,k=0;
  p=(Node *)malloc(sizeof(Node));
  if (p==NULL){
    fprintf(stderr,"Cannot Allocate Node\n");
    exit(EXIT_FAILURE);
  }
  p->parent=parent;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
      p->arr[i][j]=string[k++];
    }
  }
  strcpy(p->str,string);
  p->next=NULL;
  return p;
}

/* Identify repeated boards before put into the queue */
int In_array(Node *start,char *string,Node *p)
{
  Node *a;
  a=start;
  do{
    if (strcmp(a->str,string)==0){
      return 1;
    }
    a=a->next;
  } while (a!=p);
  return 0;
}

/* Get next generation from parent board, flage range 0-4;
  flage(0): all son boards in cycles occur
  flage(4): all son boards (up,down,left,right) not in cycles occur */
int Next_generation(Node *p,Node *start,int parent,int space)
{
  /* up */
  int x,y,flage=0;
  x=(space-1)/3;
  y=(space-1)%3;
  /* up */
  if (Access(p->arr,x-1,y) !='0'){
    p->next=AllocateNode(p->str,parent);
    Swap(&((p->next)->arr[x-1][y]),&((p->next)->arr[x][y]));
    if (!In_array(start,(p->next)->str,p)){
      p=p->next;
      flage++;
    }
  }
  /* down */
  if (Access(p->arr,x+1,y) !='0'){
    p->next=AllocateNode(p->str,parent);
    Swap(&((p->next)->arr[x+1][y]),&((p->next)->arr[x][y]));
    if (!In_array(start,(p->next)->str,p)){
      p=p->next;
      flage++;
    }
  }
  /* right */
  if (Access(p->arr,x,y+1) !='0'){
    p->next=AllocateNode(p->str,parent);
    Swap(&((p->next)->arr[x][y+1]),&((p->next)->arr[x][y]));
    if (!In_array(start,(p->next)->str,p)){
      p=p->next;
      flage++;
    }
  }
  /* left */
  if (Access(p->arr,x,y-1) !='0'){
    p->next=AllocateNode(p->str,parent);
    Swap(&((p->next)->arr[x][y-1]),&((p->next)->arr[x][y]));
    if (!In_array(start,(p->next)->str,p)){
      p=p->next;
      flage++;
    }
  }
  return flage;
}

/* Swap the character a, b in board */
void Swap(char *a,char *b)
{
  char temp;
  temp=*a;
  *a=*b;
  *b=temp;
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

/* Return where space ' ' position */
int Space_position(char arr[ROW][ROW])
{
  int i,j;
  for (i=0;i<3;i++){
    for (j=0;j<3;j++){
      if (arr[i][j]==' '){
        return i*3+j+1;
      }
    }
  }
  return -1;
}

/* Print board in queue */
void Print(Node *p)
{
  int i,j;
  for (i=0;i<3;i++){
    for (j=0;j<3;j++){
      printf("%c ",p->arr[i][j]);
    }
    printf("\n");
  }
}

/* Step by step print the order board using recursion */
void Print_Moving(Node *p,Node *start)
{
  static int cnt=0;
  Node *print;
  int i;
  if (p==start){
    printf("The %d step is\n",cnt++);
    Print(p);
  }
  else {
    print=start;
    for (i=0;i<(p->parent);i++){
      print=print->next;
    }
    Print_Moving(print,start);
    printf("The %d step is\n",cnt++);
    Print(p);
  }
}

void test(void)
{
  Node *tests,*testc;
  tests=testc=AllocateNode("1234 5678",-1);
  assert(tests->arr[0][0]=='1');
  assert(tests->arr[0][1]=='2');
  assert(tests->arr[0][2]=='3');
  assert(tests->arr[1][0]=='4');
  assert(tests->arr[1][1]==' ');
  assert(tests->arr[1][2]=='5');
  assert(tests->arr[2][0]=='6');
  assert(tests->arr[2][1]=='7');
  assert(tests->arr[2][2]=='8');
  assert(tests->parent==-1);
  assert(strcmp(tests->str,"1234 5678")==0);
}
