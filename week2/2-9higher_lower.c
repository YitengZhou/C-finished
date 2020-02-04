#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void)
{
  int random_number;
  int guess_number;
  srand(time(NULL));
  random_number=rand()%1000+1;
  printf("Welcome the game!\n");
  while ((scanf("%d",&guess_number))!=random_number)
  {
  if (guess_number>random_number)
  printf("You number is higher\n");
  else if (guess_number<random_number)
  printf("You number is lower\n");
  else
  {printf("You number is correct, the number is %d\n",random_number);
break;}}
  return 0;
}
