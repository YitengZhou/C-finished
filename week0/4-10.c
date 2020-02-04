#include <stdio.h>

int main(void)
{
  int number;
  scanf("%d",&number);
  while (number >1)
  {

  if (number%2==0)
    number =number/2;
  else
    number = number *3 +1;

  printf("%3d\n",number);
  }
  return 0;
}
