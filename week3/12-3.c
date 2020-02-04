#include <stdio.h>

struct list
{
  int value;
  struct list *next;
};

int main (void)
{
  struct list a,b,c;
  a.value=1;
  b.value=2;
  c.value=3;
  a.next=b.next=c.next=NULL;
  a.next=&b;
  b.next=&c;
  printf("%d\n",a.value);
  printf("%d\n",a.next->value);
  printf("%d\n",a.next->next->value);
  return 0;
}
