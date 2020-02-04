#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STRSIZE 500

struct node{
  int num;
  struct node *left;
  struct node *right;
};
typedef struct node Node;

Node *CreateNode(void);
char *PrintTree(Node *t);
void InsertBinTree(Node *t,int i);
int InTree(Node *t,int i);

int main(void)
{
  Node *top;
  int i;
  assert(scanf("%d",&i)==1);
  top=CreateNode();
  top->num=i;
  while (scanf("%d",&i)==1){

/* rubbish flag = 999 */
    if (i==999){
      break;
    }
    InsertBinTree(top,i);
  }
  printf("%s\n",PrintTree(top));

  /* 是否在树中 */
  while (scanf("%d",&i)==1){
    printf("%d is %s in Tree\n",i,InTree(top,i)?"":" not");
  }
}

void InsertBinTree(Node *t,int i)
{
  Node *p;
  if (t==NULL||i==t->num) return;
  if (i<t->num){
    if (t->left==NULL){
      p=CreateNode();
      p->num=i;
      t->left=p;
    }
    else {
      InsertBinTree(t->left,i);
    }
  }
  else {
    if (t->right==NULL){
      p=CreateNode();
      p->num=i;
      t->right=p;
    }
    else {
      InsertBinTree(t->right,i);
    }
  }
}

char *PrintTree(Node *t){
  char *str;

  str=calloc(STRSIZE,sizeof(char));
  assert(str!=NULL);

  if (t==NULL){
    strcpy(str,"*");
    return str;
  }

  sprintf(str,"%d(%s)(%s)",t->num,PrintTree(t->left),PrintTree(t->right));
  return str;
}

int InTree(Node *t,int i)
{
  if (t==NULL) return 0;
  if (t->num==i) return 1;
  if (i<t->num){
    return InTree(t->left,i);
  }
  else{
    return InTree(t->right,i);
  }
}

Node *CreateNode(void)
{
  Node *n=(Node *)calloc(1,sizeof(Node));
  return n;
}
