#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int i,j,k;
  int money=10,number;
  printf("Welcome to the roulette game!\n");
  printf("you money remain %d\n",money);
  while (money >0)
  {
    printf("choose your bet 1.particular number (0-35) or 2.odd/even)\n");
    number=rand()%36;
    if (scanf("%d",&i)==1)
    {
      switch (i)
      {
        case 1:
        printf("You choice particular number\n");
        printf("What particular number you want (0-35)?\n");
        scanf("%d",&j);
        if (j==number)
          {printf("This time number is %d\nYou win the game\n",number);
            money +=35;}
        else
        {printf("This time number is %d\nYou lose the game\n",number);
          money --;}
        printf("You money retain %d\n",money);
        break;

        case 2:
        printf("You choice odd/even\n");
        printf("What 1.odd or 2.even you want?\n");
        if (scanf("%d",&k)==1)
          switch (k)
          {
            case 1:
            if (number % 2==1)
            {printf("This time number is %d\nYou win the game\n",number);
              money ++;}
            else
            {printf("This time number is %d\nYou lose the game\n",number);
              money --;}
            break;
            case 2:
            if (number %2 ==0)
            {printf("This time number is %d\nYou win the game",number);
              money ++;}
            else
            {printf("This time number is %d\nYou lose the game",number);
              money --;}
            break;
            default:
            printf("Please enter number 1 or 2\n");
          }
        printf("You money retain %d\n",money);
        break;

        default:
        printf("Please enter number 1 or 2\n");
      }
    }
  }
  printf("You lose all money ,thank you for your play\n");
  return 0;
}
