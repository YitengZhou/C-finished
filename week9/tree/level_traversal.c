#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define STRSIZE 5000
#define START 'A'
#define END 'G'
#define MAX_QUEUE 100

struct node{
  char c;
  struct node *left;
  struct node *right;
};
typedef struct node Node;

struct queue{
  Node *n[MAX_QUEUE];
  int front;
  int back;
};
typedef struct queue Queue;

Node * MakeNode(char c);
void InsertRandom(Node *t,Node *n);
char *PrintTree(Node *t);
Node *InTree(char l,Node *t);
int MaxDepth(Node *t);

/* level traversal */
void InitialiseQueue(Queue *q);
Node *Remove(Queue *q);
void Insert(Node *t, Queue *q);
int Empty(Queue q);
void PrintLevelOrder(Node *t);

int main(void)
{
  char c;
  Node *head=MakeNode(START);
  Node *n;
  srand(time(NULL));

  for (c=START+1;c<END;c++){
    n=MakeNode(c);
    InsertRandom(head,n);
  }
  printf("%s\n",PrintTree(head));

  printf("%d\n",MaxDepth(head));

  printf("In the tree: *%c*\n",InTree('C',head)->c);
  PrintLevelOrder(head);

  return 0;
}

Node *MakeNode(char c)
{
  /* 用calloc无需初始化 */
  Node *n=(Node *)calloc(1,sizeof(Node));
  assert(n!=NULL);
  n->c=c;
  return n;
}

void InsertRandom(Node *t,Node *n)
{
  /* left */
  if ((rand()%2)==0){
    if ((t->left)==NULL){
      t->left=n;
    }
    else{
      InsertRandom(t->left,n);
    }
  }
  else {
    if ((t->right)==NULL){
      t->right=n;
    }
    else{
      InsertRandom(t->right,n);
    }
  }
}

char *PrintTree(Node *t)
{
  char *str;
  assert ((str=calloc(STRSIZE,sizeof(char)))!=NULL);
  if (t==NULL){
    strcpy(str,"*");
    return str;
  }
  sprintf(str,"%c(%s)(%s)",t->c,PrintTree(t->left),PrintTree(t->right));
  return str;
}

/* Searching in tree */
Node *InTree(char l,Node *t)
{
  Node *p;
  if (t==NULL) return NULL;
  if (t->c==l) return t;
  if ((p=InTree(l,t->left))!=NULL){
    return p;
  }
  if ((p=InTree(l,t->right))!=NULL){
    return p;
  }
  return NULL;
}

int MaxDepth(Node *t)
{
  int Left_depth,Right_depth;
  if (t==NULL){
    return 0;
  }
  else{
    Left_depth=MaxDepth(t->left);
    Right_depth=MaxDepth(t->right);
    if (Left_depth>Right_depth){
      return (Left_depth+1);
    }
    else{
      return (Right_depth+1);
    }
  }
}

/* level order traversal */
void InitialiseQueue(Queue *q)
{
  q->front=0;
  q->back=0;
}

Node *Remove(Queue *q)
{
  Node *n;
  n=q->n[q->front];
  q->front =(q->front+1)%MAX_QUEUE;
  return n;
}

void Insert(Node *t, Queue *q)
{
  q->n[q->back]=t;
  q->back=(q->back+1)%MAX_QUEUE;
}

int Empty(Queue q)
{
  if (q.front == q.back) return 1;
  return 0;
}

void PrintLevelOrder(Node *t)
{
  Queue q;
  Node *n;

  InitialiseQueue(&q);
  Insert(t,&q);
  while (!Empty(q)){
    n=Remove(&q);
    if (n!=NULL){
      printf("%c\n",n->c);
      Insert(n->left,&q);
      Insert(n->right,&q);
    }
  }
}
