#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXCHARACTER 100
#define STRSIZE 50000
#define MAX_QUEUE 200
#define MAXSTRING 30

struct tree{
  char ch;
  int n;
  int flag;
  struct tree *left;
  struct tree *right;
  struct tree *parent;
};
typedef struct tree Tree;

struct queue{
  Tree *n[MAX_QUEUE];
  int front;
  int back;
};
typedef struct queue Queue;

void Performance(char *file);
void Initial(Tree c[]);
int ReadChar(Tree c[],char *file);
int Inqueue(Tree c[],char ch,int cnt);
void bubble_sort(Tree c[],int cnt);
void Copy_leaf(Tree c[],Tree leaf[]);
void Root(Tree c[],Tree b[],int cnt);
void Clear(Tree c[],int cnt);
void Insert(Tree c[],int cnt,int reorder);
void Huffman(Tree root[],Tree leat[],int cnt);
char *PrintTree(Tree *t);
int Depth(Tree leaf[],char a,int number);
char *Code(Tree leaf[],char a,int number);
char *Reverse(char code[],int cnt);

/* level order traversal */
void InitialiseQueue(Queue *q);
Tree *Remove(Queue *q);
void InsertQueue(Tree *t, Queue *q);
int Empty(Queue q);
void PrintLevelOrder(Tree *t);

int main(int argc,char **argv)
{
  if (argc==2){
      Performance(argv[1]);
  }
  else{
    fprintf(stderr,"ERROR: Incorrect usage. try e.g. "\
    "%s story.txt\n",argv[0]);
  }
  return 0;
}

void Performance(char *file)
{
  int i,cnt;
  Tree c[MAXCHARACTER];
  Tree root[MAXCHARACTER];
  Tree leaf[MAXCHARACTER];

  Initial(c);
  Initial(root);
  Initial(leaf);
  cnt=ReadChar(c,file);

  bubble_sort(c,cnt);
  Copy_leaf(c,leaf);

  Root(c,root,cnt);

  Huffman(root,leaf,cnt);

  printf("Total statistical characters:\n");
  for (i=0;i<cnt;i++){
    printf("%c:%d->%d\n",leaf[i].ch,leaf[i].n,leaf[i].flag);
  }
  printf("Creat tree root:\n");
  for (i=0;i<cnt-1;i++){
    printf("%c:%d->%d\n",root[i].ch,root[i].n,root[i].flag);
  }
  printf("Tree:\n");
  printf("%s\n",PrintTree(&root[0]));

  PrintLevelOrder(&root[0]);

  for (i=0;i<cnt;i++){
    printf("'%c' : %15s (%2d*%4d)\n",leaf[i].ch,Code(leaf,leaf[i].ch,cnt)\
    ,Depth(leaf,leaf[i].ch,cnt),leaf[i].n);
  }
  /* test the depth code and reverse */
  /*
  printf("%c: depth %d\n",' ',Depth(leaf,' ',cnt));
  printf("%c: depth %d\n",'a',Depth(leaf,'a',cnt));
  printf("%c: depth %d\n",'b',Depth(leaf,'b',cnt));
  printf("%c: depth %d\n",'c',Depth(leaf,'c',cnt));
  printf("%c: depth %d\n",'d',Depth(leaf,'d',cnt));
  printf("%c: depth %d\n",'e',Depth(leaf,'e',cnt));
  printf("%c: depth %d\n",'f',Depth(leaf,'f',cnt));
  printf("%c: depth %d\n",'g',Depth(leaf,'g',cnt));
  printf("%c: depth %d\n",'h',Depth(leaf,'h',cnt));
  printf("%c: depth %d\n",'z',Depth(leaf,'z',cnt));
  printf("%c: depth %d\n",'?',Depth(leaf,'?',cnt));
  printf("%c: code %s\n",'?',Code(leaf,'?',cnt));*/
}

void Initial(Tree c[])
{
  int i;
  for (i=0;i<MAXCHARACTER;i++){
    c[i].ch='\0';
    c[i].n=0;
    c[i].flag=0;
    c[i].left=NULL;
    c[i].right=NULL;
    c[i].parent=NULL;
  }
}

int ReadChar(Tree c[],char *file)
{
  FILE *fp;
  char ch;
  int i,cnt=0,length=0;
  fp=fopen(file,"r");
  if (fp==NULL){
    fprintf(stderr,"Failed to open the file.\n");
    exit(EXIT_FAILURE);
  }
  while ((ch=getc(fp))!=EOF){
    if (ch!='\n'){
      length++;
      i=Inqueue(c,ch,cnt);
      c[i].ch=ch;
      c[i].n++;
      if (i==cnt){
        cnt++;
      }
    }
  }
  printf("there are %d types character\n",cnt);
  printf("there are %d letters\n",length);
  fclose(fp);
  return cnt;
}

int Inqueue(Tree c[],char ch,int cnt)
{
  int i;
  for (i=0;i<=cnt;i++){
    if (c[i].ch==ch){
      return i;
    }
  }
  return cnt;
}

void bubble_sort(Tree c[],int cnt)
{
  int i,change;
  Tree temp;
  do{
    change=0;
    for (i=0;i<cnt-1;i++)
    {
      if (c[i].n<c[i+1].n)
      {
        temp=c[i];
        c[i]=c[i+1];
        c[i+1]=temp;
        change++;
      }
    }
  }while (change);
}

void Copy_leaf(Tree c[],Tree leaf[])
{
  int i;
  for (i=0;i<MAXCHARACTER;i++){
    leaf[i].ch=c[i].ch;
    leaf[i].n=c[i].n;
  }
}

void Root(Tree c[],Tree b[],int cnt)
{
  int i;
  while (cnt>=2){
    b[cnt-2].ch='/';
    i=b[cnt-2].n=c[cnt-2].n+c[cnt-1].n;
    Clear(c,cnt--);
    Clear(c,cnt--);
    Insert(c,cnt++,i);
  }
}

void Clear(Tree c[],int cnt)
{
  c[cnt-1].ch='\0';
  c[cnt-1].n=0;
}

void Insert(Tree c[],int cnt,int reorder)
{
  int i=0,j;
  while (c[i].n>reorder){
    i++;
  }
  for (j=cnt;j>i;j--){
    c[j]=c[j-1];
  }
  c[i].ch='/';
  c[i].n=reorder;
}

void Huffman(Tree root[],Tree leaf[],int cnt)
{
  int i,j,k,m,n,mark=0;
  for (i=0;i<cnt-1;i++){
    mark=0;
    for (j=0;j<cnt-1;j++){
      for (k=j+1;k<cnt-1;k++){
        if (mark!=1 && root[j].flag!=1 && root[k].flag!=1 &&\
           root[i].n==root[j].n+root[k].n){
          root[i].left=&root[k];
          root[i].right=&root[j];
          root[k].parent=&root[i];
          root[j].parent=&root[i];
          root[k].flag=1;
          root[j].flag=1;
          mark=1;
        }
      }
    }
    if (root[i].left==NULL){
      for (m=0;m<cnt-1;m++){
        for (n=0;n<cnt;n++){
          if (mark!=1 && root[m].flag!=1 && leaf[n].flag!=1 &&\
             root[i].n==root[m].n+leaf[n].n){
            if (root[m].n>leaf[n].n){
              root[i].left=&leaf[n];
              root[i].right=&root[m];
            }
            else{
              root[i].left=&root[m];
              root[i].right=&leaf[n];
            }
            leaf[n].parent=&root[i];
            root[m].parent=&root[i];
            leaf[n].flag=1;
            root[m].flag=1;
            mark=1;
          }
        }
      }
    }
    if (root[i].left==NULL){
      for (j=0;j<cnt;j++){
        for (k=j+1;k<cnt;k++){
          if (mark!=1 &&leaf[j].flag!=1 && leaf[k].flag!=1 &&\
           root[i].n==leaf[j].n+leaf[k].n){
          root[i].left=&leaf[k];
          root[i].right=&leaf[j];
          leaf[k].parent=&root[i];
          leaf[j].parent=&root[i];
          leaf[k].flag=1;
          leaf[j].flag=1;
          mark=1;
          }
        }
      }
    }
  }

/* identify whether have empty roots */
  for (i=0;i<cnt-1;i++){
    if (root[i].left==NULL){
      printf("leaf is empty %c:%d\n",root[i].ch,root[i].n);
    }
    if (root[i].right==NULL){
      printf("right is empty %c:%d\n",root[i].ch,root[i].n);
    }
  }
}

char *PrintTree(Tree *t)
{
  char *str;
  assert ((str=calloc(STRSIZE,sizeof(char)))!=NULL);
  if (t==NULL){
    strcpy(str,"*");
    return str;
  }
  sprintf(str,"%d%c(%s)(%s)",t->n,t->ch,PrintTree(t->left),PrintTree(t->right));
  return str;
}

int Depth(Tree leaf[],char a,int number)
{
  int i=0,cnt=0;
  Tree *p;
  while (leaf[i].ch!=a && i<number){
    i++;
  }
  if (i==number){
    fprintf(stderr,"This '%c' character not in tree\n",a);
    exit(EXIT_FAILURE);
  }
  p=&leaf[i];
  while (p!=NULL){
    /* show path */
    /*printf("%c(%d)->%d",p->ch,p->n,cnt);*/
    cnt++;
    p=p->parent;
  }
  return cnt-1;
}

char *Code(Tree leaf[],char a,int number)
{
  char code[MAXSTRING];
  int i=0,cnt=0;
  char *c;
  Tree *p;
  while (leaf[i].ch!=a && i<number){
    i++;
  }
  if (i==number){
    fprintf(stderr,"This '%c' character not in tree\n",a);
    exit(EXIT_FAILURE);
  }
  p=&leaf[i];
  while (p->parent!=NULL){
    if (p->parent->left==p){
      code[cnt++]='0';
    }
    else{
      code[cnt++]='1';
    }
    p=p->parent;
  }
  code[cnt]='\0';
  c=Reverse(code,cnt);
  return c;
}

char* Reverse(char code[],int cnt)
{
  int i;
  char temp;
  for (i=0;i<cnt/2;i++){
    temp=code[i];
    code[i]=code[cnt-1-i];
    code[cnt-1-i]=temp;
  }
  return code;
}

/* level order traversal */
void InitialiseQueue(Queue *q)
{
  q->front=0;
  q->back=0;
}

Tree *Remove(Queue *q)
{
  Tree *n;
  n=q->n[q->front];
  q->front =(q->front+1)%MAX_QUEUE;
  return n;
}

void InsertQueue(Tree *t, Queue *q)
{
  q->n[q->back]=t;
  q->back=(q->back+1)%MAX_QUEUE;
}

int Empty(Queue q)
{
  if (q.front == q.back) return 1;
  return 0;
}

void PrintLevelOrder(Tree *t)
{
  Queue q;
  Tree *n;
  InitialiseQueue(&q);
  InsertQueue(t,&q);
  while (!Empty(q)){
    n=Remove(&q);
    if (n!=NULL){
      if (n->ch!='/'){
        printf("'%c' : %d\n",n->ch,n->n);
      }
      InsertQueue(n->left,&q);
      InsertQueue(n->right,&q);
    }
  }
}
