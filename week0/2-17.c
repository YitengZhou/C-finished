#include <stdio.h>
#include <stdlib.h>
int main (void)
{
  int median,minus_cnt=0,plus_cnt=0,i;
  median = RAND_MAX/2;
  for (i=0;i<500;i++) {
    if (rand()>median)
      plus_cnt++;
    else
      minus_cnt++;
  }
  printf("minus_cnt is %d, plus_cnt is %d \nAnd different is %d\n",minus_cnt,plus_cnt,minus_cnt-plus_cnt);
  return 0;
}
