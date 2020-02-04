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
int Next_generation(Node *p,Node *flage,Node *start,int parent,int space);
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

void Performance(char *string)
{
  Node *start,*current,*flage;
  int cnt=0,n=0;
  start=current=flage=AllocateNode(string,-1);
  while (strcmp(current->str,"12345678 ")!=0){
    if (n==0){
      n=Next_generation(current,flage,start,cnt,Space_position(flage->arr));
      cnt++;
      flage=flage->next;
    }
    else{
      current=current->next;
      n--;
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

/* Get next generation from parent board, flage range 0-4;
  flage(0): all son boards in cycles occur
  flage(4): all son boards (up,down,left,right) not in cycles occur */
int Next_generation(Node *p,Node *f,Node *start,int parent,int space)
{
  /* up */
  int x,y,flage=0;
  x=(space-1)/3;
  y=(space-1)%3;
  /* up */
  if (Access(p->arr,x-1,y) !='0'){
    p->next=AllocateNode(f->str,parent);
    Swap(&((p->next)->arr[x-1][y]),&((p->next)->arr[x][y]));
    Swap(&(p->next)->str[space-1],&(p->next)->str[space-4]);
    if (!In_array(start,(p->next)->str,p)){
      p=p->next;
      flage++;
    }
    else {
      free(p->next);
    }
  }
  /* down */
  if (Access(p->arr,x+1,y) !='0'){
    p->next=AllocateNode(f->str,parent);
    Swap(&((p->next)->arr[x+1][y]),&((p->next)->arr[x][y]));
    Swap(&(p->next)->str[space-1],&(p->next)->str[space+2]);
    if (!In_array(start,(p->next)->str,p)){
      p=p->next;
      flage++;
    }
    else {
      free(p->next);
    }
  }
  /* right */
  if (Access(p->arr,x,y+1) !='0'){
    p->next=AllocateNode(f->str,parent);
    Swap(&((p->next)->arr[x][y+1]),&((p->next)->arr[x][y]));
    Swap(&(p->next)->str[space-1],&(p->next)->str[space]);
    if (!In_array(start,(p->next)->str,p)){
      p=p->next;
      flage++;
    }
    else {
      free(p->next);
    }
  }
  /* left */
  if (Access(p->arr,x,y-1) !='0'){
    p->next=AllocateNode(f->str,parent);
    Swap(&((p->next)->arr[x][y-1]),&((p->next)->arr[x][y]));
    Swap(&(p->next)->str[space-1],&(p->next)->str[space-2]);
    if (!In_array(start,(p->next)->str,p)){
      p=p->next;
      flage++;
    }
    else {
      free(p->next);
    }
  }
  return flage;
}

/* Identify repeated boards before put into the queue */
int In_array(Node *start,char *string,Node *p)
{
  Node *a;
  a=start;
  while (a!=p){
    if (strcmp(a->str,string)==0){
      return 1;
    }
    a=a->next;
  }
  return 0;
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

  assert(Space_position(tests->arr)==5);

  assert(Access(tests->arr,2,2)=='8');
  assert(Access(tests->arr,0,0)=='1');
  assert(Access(tests->arr,3,0)=='0');
  assert(Access(tests->arr,-1,0)=='0');
  assert(Access(tests->arr,1,1)==' ');
}
