#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define STRSIZE 5000
#define START 'A'
#define END 'G'

struct node{
  char c;
  struct node *left;
  struct node *right;
};
typedef struct node Node;

Node * MakeNode(char c);
void InsertRandom(Node *t,Node *n);
char *PrintTree(Node *t);
int MaxDepth(Node *t);

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
