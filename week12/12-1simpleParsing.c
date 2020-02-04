#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXEXPR 400

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
int Empty(Stack *s);

void CheckBrackets(char *str);
int MatchBracket(char c, char d);

int main(void)
{
    char name[MAXEXPR];
    if (!scanf("%s",name)){
        printf("Cannot read string ?\n");
        exit(2);
    }
    CheckBrackets(name);
    return 0;
}

int MatchBracket(char c, char d)
{
    if (c=='{' && d=='}') return 1;
    if (c=='[' && d==']') return 1;
    if (c=='(' && d==')') return 1;
    return 0;
}

void CheckBrackets(char *str)
{
    char c;
    Stack s;
    InitialiseStack(&s);

    while(*str){
        if (*str=='{' || *str == '(' || *str == '['){
            Push(&s,(int)*str);
        }
        if (*str=='}' || *str == ']' || *str == ')'){
            c=Pop(&s);
            if (!MatchBracket(c,*str)){
                printf("Parse Error !\n");
                exit(2);
            }
        }
        printf("%c\n",*str);
        str++;
    }
    if (!Empty(&s)){
        printf("Parse Error!\n");
        exit(2);
    }
    printf("Everything OK!\n");
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

int Empty(Stack *s)
{
    if (s->tp->i==0){
        return 0;
    }
    else{
        return 1;
    }
}
