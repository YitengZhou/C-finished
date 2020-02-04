#include <stdio.h>

void change(int v);
void change2(int *p);

int main(void)
{
  int v=1,*p;
  change(v);
  printf("%d\n",v);
  p=&v;
  change2(p);
  printf("%d\n",v);
  return 0;
}

void change(int v)
{
  v=2;
}

void change2(int *p)
{
  *p=10;
}
