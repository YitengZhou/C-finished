#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
  double x,y,w;
  int cnt_in=0,cnt_out=0,i;
  srand(time(0)); /*every time different*/
  for (i=0;i<10000000;i++)
  {
    x=(double)rand()/RAND_MAX; /*uniform*/
    y=(double)rand()/RAND_MAX; /*uniform*/

    if ((x*x+y*y)>1)
      cnt_out++;
    else
      cnt_in++;}

  printf("%d %d\n",cnt_out,cnt_in);
  w=4.0*cnt_in/(cnt_in+cnt_out);
  printf("Approximation to pi is %.10f\n",w);
  return 0;
}
