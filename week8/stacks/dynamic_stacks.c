#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct stackelem{
  int i;
  struct stackelem *prev;
};
typedef struct stackelem Elem;

struct thestack{
  Elem *tp;
};
typedef struct thestack Stack;

void InitialiseStack(Stack *s);
void Push(Stack *s,int n);
int Pop(Stack *s);
void StringReverse(char *s);

int main(void)
{
  char str[]="Hello world";
  Stack s;
  InitialiseStack(&s);
  printf("%d\n",s.tp->i); /*calloc初始化为0*/

  Push(&s,12);
  Push(&s,117);
  Push(&s,13);
  Push(&s,15);
  Push(&s,20);
  Push(&s,57);

  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));

/* empty stacks */
/*  printf("%d\n",Pop(&s));*/

  StringReverse(str);
  printf("%s\n",str);

  return 0;
}

void InitialiseStack(Stack *s)
{
  s->tp=(Elem *)calloc(1,sizeof(Elem));
  s->tp->prev = NULL; /* calloc应该可以删了*/
}

void Push(Stack *s,int n)
{
  Elem *e;
  e=(Elem *)calloc(1,sizeof(Elem));
  e->prev=s->tp; /*向前link*/
  s->tp->i=n;
  s->tp=e;
}

int Pop(Stack *s)
{
  s->tp=s->tp->prev;
  assert(s->tp!=NULL);
  return s->tp->i;
}

void StringReverse(char *str)
{
  char *k;
  int i;
  int cnt=0;
  Stack s;
  InitialiseStack(&s);
  k=str;
  while (*k!='\0'){
    Push(&s,(int)*k);
    k++;
    cnt++;
  }

  k=str;
  for (i=0;i<cnt;i++){
    *k=Pop(&s);
    k++;
  }
}
