#include <stdio.h>
#include <ctype.h>

int main(void)
{
  char str[100];
  char c;
  int flag=1,i=0,len;
  while ((c=getchar())!='\n')
  {
    if (isalpha(c))
    {str[i]=tolower(c);
    i++;}
  }
  str[i]='\0';
  len=i;
  printf("You put %d characters\n",len);
  
  for (i=0;i<len/2;i++)
  {if (str[i]!=str[len-i-1])
    flag=0;}

  if (flag==1)
  printf("This sentence is palindromes\n");
  else
  printf("This sentence NOT palindromes!\n");
  return 0;
}
