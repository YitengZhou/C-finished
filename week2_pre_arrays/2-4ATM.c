#include <stdio.h>


int main(void)
{
  int money;
  printf("How much money would you like?\n");
  while (scanf("%d",&money)==1)
  {if (money >0)
    {
      if (money%20==0)
      printf("OK, dispensing...\n");
      else
      printf("I can give you %d or %d, try again\n", 20*(money/20),20*(money/20+1));
    }
  else
  printf("Please enter positive numbers\n");}

  return 0;
}
