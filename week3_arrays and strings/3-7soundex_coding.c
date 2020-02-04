#include <stdio.h>
#include <ctype.h>

void Soundex(char ar[]);

int main(void)
{
  int i=0;
  char c;
  char surname[40];
  while ((c=getchar())!='\n')
  {
    if (i==0&&isalpha(c))
    {surname[i]=toupper(c);
    i++;}
    else
    {surname[i]=c;
    i++;}
  }
  printf("%c",surname[0]);
  Soundex(surname);
  return 0;
}

void Soundex(char ar[])
{
  int i,cnt=0;
  for (i=1;(ar[i]&&cnt<3);i++)
  {
    if (ar[i]=='b'||ar[i]=='f'||ar[i]=='p'||ar[i]=='v')
    {printf("1");cnt++;}
    else if (ar[i]=='c'||ar[i]=='g'||ar[i]=='j'||ar[i]=='k'||ar[i]=='q'||ar[i]=='s'||ar[i]=='x'||ar[i]=='z')
    {printf("2");cnt++;}
    else if (ar[i]=='d'||ar[i]=='t')
    {printf("3");cnt++;}
    else if (ar[i]=='l')
    {printf("4");cnt++;}
    else if (ar[i]=='m'||ar[i]=='n')
    {printf("5");cnt++;}
    else if (ar[i]=='r')
    {printf("6");cnt++;}
    else
    continue;
  }
  switch (cnt)
  {case 0: printf("000");break;
  case 1: printf("00");break;
  case 2: printf("0");break;}
}
