/* This program could resolve the Chinese puzzle using linked-list
  10 steps = "513276 48"
  20 steps = "8452 1736" (about 9s)
  28 steps = "87 216354" (about 2m20s) */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define ROW 3
#define WORD 10

struct node {
  int parent;
  char arr[ROW][ROW];
  char str[WORD];
  struct node *next;
};
typedef struct node Node;

/* Set up=0, down=1, right=2, left=3 */
enum direct {up, down, right, left};

void Performance(char *string);
Node* AllocateNode(char *string,int parent);
int Next_generation(Node *p,Node *f,Node *start,int parent,int space);
int Space_position(char arr[ROW][ROW]);
int In_array(Node *start,char *string,Node *p);
char Access(char arr[ROW][ROW],int movex,int movey);
void change(Node *change, int direct,int space);
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

/* Read in a board using argv[1], creat possible boards in linked-list,
  Print right order boards */
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
  x=(space-1)/ROW;
  y=(space-1)%ROW;
  /* up */
  if (Access(p->arr,x-1,y) !='0'){
    p->next=AllocateNode(f->str,parent);
    change(p->next,up,space);
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
    change(p->next,down,space);
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
    change(p->next,right,space);
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
    change(p->next,left,space);
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

/* Return where space ' ' position */
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

/* Identify repeated boards before put into linked-list */
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

/* Change son board string and array with relevant direction */
void change(Node *change, int direct,int space)
{
  int x=(space-1)/ROW,y=(space-1)%ROW;
  switch (direct)
  {
    case up:{
      Swap(&change->arr[x-1][y],&change->arr[x][y]);
      Swap(&change->str[space-1],&change->str[space-4]);
      break;
    }
    case down:{
      Swap(&change->arr[x+1][y],&change->arr[x][y]);
      Swap(&change->str[space-1],&change->str[space+2]);
      break;
    }
    case right:{
      Swap(&change->arr[x][y+1],&change->arr[x][y]);
      Swap(&change->str[space-1],&change->str[space]);
      break;
    }
    case left:{
      Swap(&change->arr[x][y-1],&change->arr[x][y]);
      Swap(&change->str[space-1],&change->str[space-2]);
      break;
    }
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

/* Print board in linked-list */
void Print(Node *p)
{
  int i,j;
  for (i=0;i<ROW;i++){
    for (j=0;j<ROW;j++){
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
  Node *tests,*testc,*testf,*testt;
  int flage;
  char a[ROW][ROW]={{'1','2','3'},{'4','5','6'},{'7','8',' '}};
  char b[ROW][ROW]={{'1','2','3'},{'4','5','6'},{'7','8','9'}};

  tests=testc=testf=AllocateNode("1234 5678",-1);
/* test enum direct (up,down,right,left) */
  assert(up==0);
  assert(down==1);
  assert(right==2);
  assert(left==3);

/* test function AllocateNode with right creat head linked-list */
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

/* test function Space_position with correct position */
  assert(Space_position(a)==9);
  assert(Space_position(b)==-1);
  assert(Space_position(tests->arr)==5);

/* test function Access */
  assert(Access(tests->arr,2,2)=='8');
  assert(Access(tests->arr,0,0)=='1');
  assert(Access(tests->arr,3,0)=='0');
  assert(Access(tests->arr,-1,0)=='0');
  assert(Access(tests->arr,1,1)==' ');

/* test function Next_generation (in this case, flage=4)
  NEXT will creat 4 boards because space in middle(5)
  parent = {{'1','2','3'},
            {'4',' ','5'},
            {'6','7','8'}} */
  flage=Next_generation(testc,testf,tests,0,Space_position(testf->arr));
  assert(flage==4);
  /* up "1 3425678" */
  testc=testc->next;
  assert(testc->arr[0][1]==' ');
  assert(testc->arr[1][1]=='2');
  /* down "1234756 8" */
  testc=testc->next;
  assert(testc->arr[2][1]==' ');
  assert(testc->arr[1][1]=='7');
  /* right "12345 678" */
  testc=testc->next;
  assert(testc->arr[1][2]==' ');
  assert(testc->arr[1][1]=='5');
  /* left "123 45678" */
  testc=testc->next;
  assert(testc->arr[1][0]==' ');
  assert(testc->arr[1][1]=='4');
  testt=testc; /* save pointer for next generation test */
  /* only creat 4 boards next point NULL */
  testc=testc->next;
  assert(testc==NULL);

/* test function In_array with correct return */
  assert(In_array(tests,"1234 5678",testc)==1);
  assert(In_array(tests,"1 3425678",testc)==1);
  assert(In_array(tests,"1234756 8",testc)==1);
  assert(In_array(tests,"12345 678",testc)==1);
  assert(In_array(tests,"123 45678",testc)==1);
  assert(In_array(tests," 12345678",testc)==0);
  assert(In_array(tests,"12345678 ",testc)==0);

/* continue to test function Next_generation (in this case, flage=2)
  NEXT will creat 2 boards because space in up(2)
    parent = {{'1',' ','3'},
              {'4','2','5'},
              {'6','7','8'}} */
  testf=testf->next;
  flage=Next_generation(testt,testf,tests,1,Space_position(testf->arr));
  assert(flage==2);
}
