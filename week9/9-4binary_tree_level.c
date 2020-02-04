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

void Performance(char *string);
/* 左为左括号的位置，右为右括号的位置 */
Node* CreateTree(char *string,int left,int right);
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
  Node *top;
  int length=strlen(string);
  /* level-left and right together
    20(10(5(*)(*))(17(*)(*)))(30(21(*)(*))(*))*/
  top=CreateTree(string,-1,length);
  printf("%s\n",PrintTree(top));
  assert(strcmp(PrintTree(top),string)==0);
}

Node* CreateTree(char *string,int left,int right)
{
  int i,leftl,leftr,rightl,rightr,flag1=1,flag2=0;
  Node *new;
  /*printf("*left:%d->*right:%d\n",left,right);*/
   if (right-left==2 && string[left+1]=='*'){
    return NULL;
   }
   else{
    i=left+1;
    while (string[i]!='('){
     i++;
    }
    new=CreateNode(StoN(string,left+1,i));
    leftl=i;
    while (flag1!=flag2){
      i++;
      if (string[i]=='('){
        flag1++;
      }
      if (string[i]==')'){
        flag2++;
      }
    }
    leftr=i;
    rightl=leftr+1;
    rightr=right-1;
    /*printf("leftl=%d,leftr=%d,rightl=%d,rightr=%d\n",leftl,leftr,rightl,rightr);*/
    new->left=CreateTree(string,leftl,leftr);
    new->right=CreateTree(string,rightl,rightr);
   }
   return new;
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

  if (t==NULL){
    strcpy(str,"*");
    return str;
  }

  sprintf(str,"%d(%s)(%s)",t->num,PrintTree(t->left),PrintTree(t->right));
  return str;
}
