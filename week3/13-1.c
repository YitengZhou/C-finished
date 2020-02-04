#include <stdio.h>

int main (void)
{
  char s[50];
  int i=8;
  float f=17.041;
  char str[100],str2[]="123";
  sprintf(str,"%d %f",i,f);
  scanf("%[zhou]s",s);
  printf("%s\n",s);
  printf("%s\n",str);
  sscanf(str2,"%d",&i);
  printf("%d",i);
  return 0;
}
