#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int s;
  s=(('P'-'A')*26*26+('H'-'A')*26+('L'-'A'))%11;
  printf("%d\n",s);
  s=(('G'-'A')*26*26+('C'-'A')*26+('M'-'A'))%11;
  printf("%d\n",s);
  s=(('O'-'A')*26*26+('R'-'A')*26+('Y'-'A'))%11;
  printf("%d\n",s);
  s=(('H'-'A')*26*26+('K'-'A')*26+('G'-'A'))%11;
  
  printf("%d\n",s);

  return 0;
}
