#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STRSIZE 500

struct node{
  int num;
  struct node *parent;
  struct node *left;
  struct node *right;
};
typedef struct node Node;

void Performance(char *string);
int Number(char *string,int i);
int StoN(char *string,int start,int end);
Node *CreateNode(int number);
char *PrintTree(Node *t);

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

void Performance(char *string)
{
  /* vertical-one char by one char
  20(10(5(*)(*))(17(*)(*)))(30(21(*)(*))(*))
  17(2(*)(3(*)(4(*)(*))))(6(8(*)(*))(*))*/
  Node *current,*top,*parent;
  int i=0,length=strlen(string);
  current=top=CreateNode(StoN(string,i,Number(string,i)));
  i=Number(string,i);

  while (i<length){
    if (string[i]=='('){
      i++;
      /* 记录母结点位置 */
      parent=current;
      if (string[i]>='0'&&string[i]<='9'){
        if (current->left==NULL){
          current->left=CreateNode(StoN(string,i,Number(string,i)));
          current=current->left;
        }
        else{
          current->right=CreateNode(StoN(string,i,Number(string,i)));
          current=current->right;
        }
        current->parent=parent;
        i=Number(string,i);
      }
      /* 若碰到空结点时额外新建一个-1结点，用于current处于正确位置 */
      if (string[i]=='*'){
        if (current->left==NULL){
          current->left=CreateNode(-1);
          current=current->left;
        }
        else{
          current->right=CreateNode(-1);
          current=current->right;
        }
        current->parent=parent;
        i++;
      }
    }

    if (string[i]==')'){
      current=current->parent;
      i++;
    }
  }
  printf("%s\n",PrintTree(top));
  assert(strcmp(PrintTree(top),string)==0);
}

int Number(char *string,int i)
{
  while (string[i]!='('){
    i++;
  }
  return i;
}

int StoN(char *string,int start,int end)
{
  int i,sum=0;
  for (i=start;i<end;i++){
    sum=10*sum+(string[i]-'0');
  }
  return sum;
}

Node *CreateNode(int number)
{
  Node *n=(Node *)calloc(1,sizeof(Node));
  n->num=number;
  return n;
}


char *PrintTree(Node *t)
{
  char *str;

  str=calloc(STRSIZE,sizeof(char));
  assert(str!=NULL);

/* 本处由t==NULL修改，碰到-1结点时输出* */
  if (t->num==-1){
    strcpy(str,"*");
    return str;
  }

  sprintf(str,"%d(%s)(%s)",t->num,PrintTree(t->left),PrintTree(t->right));
  return str;
}
