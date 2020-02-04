#include <stdio.h>

int scode (int a);

int main(void)
{
  char c;
  while ((c=getchar())!=EOF)
  putchar(scode(c));
  return 0;
}

int scode (int a)
{
  if (a>='a'&& a<='z')
  return ('z'-a+'a');
  else if (a>='A'&& a<='Z')
  return ('Z'+'A'-a);
  else
  return a;
}
