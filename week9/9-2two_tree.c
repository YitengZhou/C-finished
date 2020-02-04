#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define STRSIZE 5000
#define START 'A'
#define END 'D'

struct node{
  char c;
  struct node *left;
  struct node *right;
};
typedef struct node Node;

Node * MakeNode(char c);
void InsertRandom(Node *t,Node *n);
char *PrintTree(Node *t);
int IsSame(Node *first,Node *second);
int MaxDepth(Node *t);

int main(void)
{
  char c;
  Node *fristhead=MakeNode(START);
  Node *secondhead=MakeNode(START);
  Node *n;
  srand(time(NULL));

  for (c=START+1;c<END;c++){
    n=MakeNode(c);
    InsertRandom(fristhead,n);
  }

  for (c=START+1;c<END;c++){
    n=MakeNode(c);
    InsertRandom(secondhead,n);
  }

  printf("%s\n",PrintTree(fristhead));
  printf("%s\n",PrintTree(secondhead));

  if (strcmp(PrintTree(fristhead),PrintTree(secondhead))==0){
    printf("those trees are same!(string)\n");
  }
  else {
    printf("those trees not same!(string)\n");
  }

  /* finished -- but I feel not good */
  if (IsSame(fristhead,secondhead)){
    printf("those trees are same!(recursion)\n");
  }
  else {
    printf("those trees not same!(recursion)\n");
  }


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

/* finished but not good */
int IsSame(Node *first,Node *second)
{
  int i;
  if (first==NULL && second==NULL){
    return 1;
  }
  if (first==NULL && second!=NULL){
    return 0;
  }
  if (first!=NULL && second==NULL){
    return 0;
  }
  if (first->c!=second->c){
    return 0;
  }
  if (first->c==second->c){
    i=IsSame(first->left,second->left)*IsSame(first->right,second->right);
    if (i==1){
      return 1;
    }
    else{
      return 0;
    }
  }
  return 1;
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
