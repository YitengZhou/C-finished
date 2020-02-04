#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

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

void InitialiseQueue(Queue *q);
Node *Remove(Queue *q);
void Insert(Node *t, Queue *q);
int Empty(Queue q);
void PrintLevelOrder(Node *t);

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
