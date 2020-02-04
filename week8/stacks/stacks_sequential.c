#include <stdio.h>
#include <assert.h>

#define STACKSIZE 200

struct stacklem{
  int i;
};
typedef struct stacklem Elem;

struct thestack{
  Elem stk[STACKSIZE];
  int top;
};
typedef struct thestack Stack;

void InitialiseStack(Stack *s);
void Push(Stack *s,int n);
int Pop(Stack *s);

int main(void)
{
  Stack s;
  InitialiseStack(&s);
  Push(&s,12);
  Push(&s,117);
  Push(&s,13);
  Push(&s,15);
  Push(&s,20);
  Push(&s,57);

  printf("%d\n",s.top);
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",Pop(&s));
  printf("%d\n",s.top);
  
/* empty stacks */
  printf("%d\n",Pop(&s));

/* function pop() doesn't delete data */
/*
  printf("%d\n",s.stk[0].i);
  printf("%d\n",s.stk[1].i);
  printf("%d\n",s.stk[2].i);
  printf("%d\n",s.stk[3].i);
  printf("%d\n",s.stk[4].i);
  printf("%d\n",s.stk[5].i);

  Push(&s,15);
  printf("%d\n",Pop(&s));
  printf("%d\n",s.stk[0].i);
  */
  return 0;
}

void InitialiseStack(Stack *s)
{
  s->top=0;
}

void Push(Stack *s,int n)
{
  /* make sure stack doesnt overflow */
  assert(s->top<STACKSIZE);
  s->stk[s->top].i=n;
  s->top++;
}

int Pop(Stack *s)
{
  /* Can't pop empty stack */
  assert(s->top>0);
  s->top--;
  return s->stk[s->top].i;
}
