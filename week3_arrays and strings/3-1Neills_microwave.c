#include <stdio.h>

int main(void)
{
  int time[3]={10,60,600};
  int input_time,press=0;
  printf("Type the time required\n");
  scanf("%d",&input_time);
  if (input_time>0)
  {
    if (input_time/time[2]>0)
    {press+=input_time/time[2];
    input_time=input_time%time[2];}
    if (input_time/time[1]>0)
    {press+=input_time/time[1];
    input_time=input_time%time[1];}
    if (input_time/time[0]>0)
    {press+=input_time/time[0];
    input_time=input_time%time[0];}
    if (input_time!=0)
    press++;
    printf("Number of button presses=%d \n",press);
  }
  else
  printf("Please enter positive number!\n");
  return 0;
}
