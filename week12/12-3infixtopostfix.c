#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define MAX_QUEUE 100

struct stackelem{
  int i;
  struct stackelem *prev;
};
typedef struct stackelem Elem;

struct thestack{
  Elem *tp;
};
typedef struct thestack Stack;

struct queue{
  char n[MAX_QUEUE];
  int front;
  int back;
};
typedef struct queue Queue;

void InitialiseStack(Stack *s);
void Push(Stack *s,int n);
int Pop(Stack *s);
int EmptyS(Stack *s);

void InitialiseQueue(Queue *q);
void Insert(char t, Queue *q);
int EmptyQ(Queue q);
char RemoveQueue(Queue *q);

void PrintPostFix(char *infix);
int isoperator(char c);

int main(void)
{
    char infx[]="(((A/(B*C))+(D*E))-(A*C))";
    PrintPostFix(infx);
    return 0;
}

int isoperator(char c)
{
    if (c=='+'||c=='-'|| c=='/'||c=='*'){
        return 1;
    }
    else{
        return 0;
    }
}

void PrintPostFix(char *infix)
{
    Queue q;
    Stack s;
    int ip=0;
    char c,d;
    int priority[256];

    InitialiseStack(&s);
    InitialiseQueue(&q);
    Push(&s,'\0');

    priority['*']=2;priority['/']=2;
    priority['+']=1;priority['-']=1;
    priority['(']=3;priority[')']=0;

    do {
        c=infix[ip++];
        if (isupper(c)){
            Insert(c,&q);
        }
        else if (c==')'){
            d = Pop(&s);
            while (d!='('){
                Insert(d,&q);
                d=Pop(&s);
            }
        }
        else if (c=='\0'){
            while (!EmptyS(&s)){
                d =Pop(&s);
                Insert(d,&q);
            }
        }
        else if (c=='('||isoperator(c)){
            d =Pop(&s);
            while (priority[(int)d]>=priority[(int)c] && isoperator(d)){
                Insert(d,&q);
                d=Pop(&s);
            }
            Push(&s,d);
            Push(&s,c);
        }
        else{
            printf("Invalid Token \"%c\"\n",c);
            exit(2);
        }
    } while (c!='\0');
    for (ip=0;!EmptyQ(q);ip++){
        printf("%c",RemoveQueue(&q));
    }
    printf("\n");
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

int EmptyS(Stack *s)
{
    if (s->tp->i==0){
        return 0;
    }
    else{
        return 1;
    }
}

void InitialiseQueue(Queue *q)
{
  q->front=0;
  q->back=0;
}

void Insert(char t, Queue *q)
{
  q->n[q->back]=t;
  q->back=(q->back+1)%MAX_QUEUE;
}

char RemoveQueue(Queue *q)
{
  char n;
  n=q->n[q->front];
  q->front =(q->front+1)%MAX_QUEUE;
  return n;
}

int EmptyQ(Queue q)
{
  if (q.front == q.back) return 1;
  return 0;
}
