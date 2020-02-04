/* Can test with move 1 row, (3 0) - (3 6)
  Can test with move 2 rows, (2 0) - (2 6)
  Can test with move 3 rows, (1 0) - (1 6)
  boards: 7 step-(1 2)15273 boards,(1 3)14399 boards,(1 4)17479 boards;
          9 step-(1 0),(1 1),(1 5),(1 6) special range within 4000 boards
  Can test with move 4 rows, 19 step (0 2) and (0 4),
  Cannot get result with others move 4 rows, (0 0),(0 1),(0 3),(0 5),(0 6) */
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 7
#define HEIGHT 8

/* when Repeat is HEIGHT HEIGHT, 1 0, 1 2, 1 3 will resolve within 16000 boards */

/* Set up=1, down=2, right=3, left=4 */
enum direct {up=1, down, left, right};

struct queue{
  char board[HEIGHT][WIDTH];
  struct queue *parent;
  struct queue *next;
};
typedef struct queue Que;

void Performance(char *x,char *y);
Que* Initial(char *x,char *y);
void PrintBoard(Que *show);
int Next_generation(Que *s,Que *p,Que *c);
int Next_generation01(Que *s,Que *p,Que *c,int column);
int Next_generation56(Que *s,Que *p,Que *c,int column);
int Ismove(Que *s,Que *p,Que *c, int height,int width);
int Up(Que *p,Que *c, int height,int width);
int Down(Que *p,Que *c, int height,int width);
int Left(Que *p,Que *c, int height,int width);
int Right(Que *p,Que *c, int height,int width);
int Repeat(Que *s,Que *c);
int Creat(Que *p, Que *c);
void Printresult(Que *s,Que *c);

int main(int argc,char **argv)
{
  if (argc==3)
  {
    Performance(argv[1],argv[2]);
  }
  else {
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s 0 0\n",argv[0]);
  }
  return 0;
}

void Performance(char *x,char *y)
{
  int flag=0,cnt=0,p=0,row,column;
  Que *start,*parent,*current;
  row=x[0]-'0';
  column=y[0]-'0';
  start=Initial(x,y);
  parent=current=start->next;
  /* optimise5 for move 4 rows: (0 2) and (0 4) */
  if (row==0 && (column == 2||column == 4)){
    parent=parent->next;
    current=current->next;
  }

  while (current->board[row][column]!='#'){
    if (flag==0){
      printf("\n------------%d parent--------------\n",p++);
      PrintBoard(parent);

      /* 4 for move 3 rows with special range: (1 0) and (1 1) */
      if (column==0 || column==1){
        flag=Next_generation01(start,parent,current,column);
      }

      /* optimise4 for move 3 rows with special range: (1 5) and (1 6) */
      else if (column==5 || column==6){
        flag=Next_generation56(start,parent,current,column);
      }

      else{
        flag=Next_generation(start,parent,current);
      }
      parent=parent->next;
    }
    else{
      current=current->next;
      flag--;
      cnt++;
      printf("%d boards:\n",cnt);
      PrintBoard(current);
    }
  }
  printf("Creat %d board total\n",cnt);
  Printresult(start,current);
}

Que* Initial(char *x,char *y)
{
  Que *first,*second,*third;
  int i,j,row,column;
  row=x[0]-'0';
  column=y[0]-'0';
  if ((row>=HEIGHT/2||column>=WIDTH)){
    fprintf(stderr, "ERROR: Incorrent input, please try again\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    first=(Que *)malloc(sizeof(Que));
    second=(Que *)malloc(sizeof(Que));
    third=(Que *)malloc(sizeof(Que));
    if (first==NULL){
      fprintf(stderr,"Cannot Allocate Node\n");
      exit(EXIT_FAILURE);
    }
    for (i=0;i<HEIGHT/2;i++){
      for (j=0;j<WIDTH;j++){
        first->board[i][j]='.';
        second->board[i][j]='.';
        third->board[i][j]='.';
      }
    }
    for (;i<HEIGHT;i++){
      for (j=0;j<WIDTH;j++){
        first->board[i][j]='#';
        second->board[i][j]='#';
        third->board[i][j]='#';
      }
    }

    /* optimise3: input column need move first to arrive target position */
    second->board[4][column]='.';
    second->board[5][column]='.';
    second->board[3][column]='#';

    /* optimise5 for move 4 rows: (0 2) */
    if (column==2 && row ==0){
    third->board[1][column]='#';
    third->board[4][column-2]='.';
    third->board[4][column-1]='.';
    third->board[4][column]='.';
    third->board[4][column+1]='.';
    third->board[4][column+2]='.';
    third->board[5][column-2]='.';
    third->board[5][column-1]='.';
    third->board[5][column]='.';
    }

    /* optimise5 for move 4 rows: (0 4) */
    if (column==4 && row ==0){
    third->board[1][column]='#';
    third->board[4][column-2]='.';
    third->board[4][column-1]='.';
    third->board[4][column]='.';
    third->board[4][column+1]='.';
    third->board[4][column+2]='.';
    third->board[5][column]='.';
    third->board[5][column+1]='.';
    third->board[5][column+2]='.';
    }

    first->parent=NULL;
    second->parent=first;
    third->parent=second;

    first->next=second;
    second->next=third;
    third->next=NULL;
  }
  return first;
}

int Next_generation(Que *s,Que *p,Que *c)
{
  int i,j,k,flag;
  int cnt=0;
  for (i=0;i<HEIGHT;i++){
    for (j=0;j<WIDTH;j++){
      flag=Ismove(s,p,c,i,j);
      if (flag){
        printf("this board (%d,%d) can move %d times\n",i+1,j+1,flag);
        for (k=0;k<flag;k++){
          c=c->next;
          cnt++;
        }
      }
    }
  }
  return cnt;
}

int Next_generation01(Que *s,Que *p,Que *c,int column)
{
  int i,j,k,flag;
  int cnt=0;
  for (i=0;i<HEIGHT;i++){
    for (j=0;j<WIDTH/2+column;j++){
      flag=Ismove(s,p,c,i,j);
      if (flag){
        printf("this board (%d,%d) can move %d times\n",i+1,j+1,flag);
        for (k=0;k<flag;k++){
          c=c->next;
          cnt++;
        }
      }
    }
  }
  return cnt;
}

int Next_generation56(Que *s,Que *p,Que *c,int column)
{
  int i,j,k,flag;
  int cnt=0;
  for (i=0;i<HEIGHT;i++){
    for (j=column/2+1;j<WIDTH;j++){
      flag=Ismove(s,p,c,i,j);
      if (flag){
        printf("this board (%d,%d) can move %d times\n",i+1,j+1,flag);
        for (k=0;k<flag;k++){
          c=c->next;
          cnt++;
        }
      }
    }
  }
  return cnt;
}

int Ismove(Que *s,Que *p,Que *c, int height,int width)
{
  int flag=0;
  if (p->board[height][width]=='.'){
    return 0;
  }
  else{
    if (Up(p,c,height,width)) {
      if (!Repeat(s,c)){
        flag++;
        c=c->next;
      }
      else{
        free(c->next);
      }
    }
    /* optimise1: this program does not need to move down */
    /*if (Down(p,c,height,width)) {
      if (!Repeat(s,c)){
        flag++;
        c=c->next;
      }
      else{
        free(c->next);
      }
    }*/
    if (Left(p,c,height,width)) {
      if (!Repeat(s,c)){
        flag++;
        c=c->next;
      }
      else{
        free(c->next);
      }
    }
    if (Right(p,c,height,width)) {
      if (!Repeat(s,c)){
        flag++;
        c=c->next;
      }
      else{
        free(c->next);
      }
    }
    return flag;
  }
  return 0;
}

int Up(Que *p,Que *c, int height,int width)
{
  char path[10]={'0'};
  int i,j,cnt=0;
  for (i=height;i>=0;i--){
    path[cnt++]=p->board[i][width];
  }
  for (i=0;i<=height;i++){
    if (path[i]=='.'){
      /* optimise2: this program does not need to move more than one soldier */
      if (i==2){
        Creat(p,c);
        for (j=0;j<i;j++){
          (c->next)->board[height-j][width]='.';
        }
        (c->next)->board[height-i][width]='#';
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  return 0;
}

int Down(Que *p,Que *c, int height,int width)
{
  char path[10]={'0'};
  int i,j,cnt=0;
  for (i=height;i<HEIGHT;i++){
    path[cnt++]=p->board[i][width];
  }
  for (i=0;i<=HEIGHT-height;i++){
    if (path[i]=='.'){
      /* optimise2: this program does not need to move more than one soldier */
      if (i==2){
        Creat(p,c);
        for (j=0;j<i;j++){
          (c->next)->board[height+j][width]='.';
        }
        (c->next)->board[height+i][width]='#';
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  return 0;
}

int Left(Que *p,Que *c, int height,int width)
{
  char path[10]={'0'};
  int i,j,cnt=0;
  for (i=width;i>=0;i--){
    path[cnt++]=p->board[height][i];
  }
  for (i=0;i<=width;i++){
    if (path[i]=='.'){
      /* optimise2: this program does not need to move more than one soldier */
      if (i==2){
        Creat(p,c);
        for (j=0;j<i;j++){
          (c->next)->board[height][width-j]='.';
        }
        (c->next)->board[height][width-i]='#';
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  return 0;
}

int Right(Que *p,Que *c, int height,int width)
{
  char path[10]={'0'};
  int i,j,cnt=0;
  for (i=width;i<WIDTH;i++){
    path[cnt++]=p->board[height][i];
  }
  for (i=0;i<=WIDTH-width;i++){
    if (path[i]=='.'){
      /* optimise2: this program does not need to move more than one soldier */
      if (i==2){
        Creat(p,c);
        for (j=0;j<i;j++){
          (c->next)->board[height][width+j]='.';
        }
        (c->next)->board[height][width+i]='#';
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  return 0;
}

int Creat(Que *p, Que *c)
{
  int i,j;
  Que *new;
  new= (Que *)malloc(sizeof(Que));
  if (new==NULL){
    fprintf(stderr,"Cannot Allocate Node\n");
    exit(EXIT_FAILURE);
  }
  for (i=0;i<HEIGHT;i++){
    for (j=0;j<WIDTH;j++){
      new->board[i][j]=p->board[i][j];
    }
  }
  new->parent=p;
  new->next=NULL;
  c->next=new;
  return 0;
}

int Repeat(Que *s,Que *c)
{
  int i,j,cnt;
  while (s!=c){
    cnt=0;
    for (i=0;i<HEIGHT;i++){
      for (j=0;j<WIDTH;j++){
        if (s->board[i][j]==(c->next)->board[i][j]){
          cnt++;
        }
      }
    }
    if (cnt==HEIGHT*WIDTH){
      return 1;
    }
    else{
      s=s->next;
    }
  }
  return 0;
}

void PrintBoard(Que *show)
{
  int i,j;
  for (i=0;i<HEIGHT;i++){
    for (j=0;j<WIDTH;j++){
      printf("%c ",show->board[i][j]);
    }
    printf("\n");
  }
}

void Printresult(Que *s,Que *c)
{
  static cnt=0;
  if (c==s){
    printf("The %d step is:\n",cnt++);
    PrintBoard(s);
  }
  else{
    Printresult(s,c->parent);
    printf("The %d step is:\n",cnt++);
    PrintBoard(c);
  }
}
