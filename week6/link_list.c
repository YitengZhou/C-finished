#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 20

struct node {
  char name[MAXNAME];
  struct node *next;
};
typedef struct node Node;

Node* AllocateNode(char *s);
void PrintList(Node *l);
void Print_recursion(Node *l);
Node *Inlist(Node *n,char *s);
Node *Inlist_recursion(Node *n,char *s);
Node * DeleteNode(Node *start,char *s);

int main(void)
{
  char name[MAXNAME];
  Node *start,*current;
  printf("Enter the first name:");
  if (scanf("%s",name)==1) {
    start=current=AllocateNode(name);
  }
  else return 1;

  printf("Enter more name:");
  while (scanf("%s",name)==1){
    current->next=AllocateNode(name);
    current=current->next;
  }
  PrintList(start);

  start=DeleteNode(start,"zhouyiteng");
  /* recursion */
  Print_recursion(start);

  printf("%p\n",(void *)Inlist(start,"zhouyiteng"));
  printf("%p\n",(void *)Inlist(start,"maxiaoyuan"));
  printf("%p\n",(void *)Inlist(start,"???"));

  /* recursion */
  printf("%p\n",(void *)Inlist_recursion(start,"zhouyiteng"));
  printf("%p\n",(void *)Inlist_recursion(start,"maxiaoyuan"));
  printf("%p\n",(void *)Inlist_recursion(start,"???"));

  return 0;
}

Node* AllocateNode(char *s)
{
  Node *p;
  p=(Node *)malloc(sizeof(Node));
  if (p==NULL){
    printf("Cannot Allocate Node\n");
    exit(EXIT_FAILURE);
  }

  strcpy(p->name,s);
  p->next=NULL;
  return p;
}

void PrintList(Node *l)
{
  printf("\n");
  do {
    printf("Name:%s\n",l->name);
    l=l->next;
  } while (l!=NULL);
  printf("END\n");
}

Node *Inlist(Node *n,char *s)
{
  do{
    if (strcmp(n->name,s)==0){
      return n;
    }
    n=n->next;
  } while (n!=NULL);
  return NULL;
}

/* recursion */
void Print_recursion(Node *l)
{
  if (l==NULL){
    printf("END\n");
    return;
  }
  printf("Name : %s\n",l->name);
  Print_recursion(l->next);
}

/* recursion */
Node *Inlist_recursion(Node *n,char *s)
{
  if (n==NULL){
    return NULL;
  }
  if (strcmp(n->name,s)==0){
    return n;
  }
  return Inlist_recursion(n->next,s);
}

Node * DeleteNode(Node *start,char *s)
{
  Node *prev,*l;
  /* 1st in list?*/
  if (!strcmp(start->name,s))
  return start->next;

  l= start;
  do {
    prev=l;
    l=l->next;
    if (strcmp(l->name,s)==0){
      prev->next=l->next;
      free(l);
      return start;
    }
  } while (l!=NULL);
  return start;
}
