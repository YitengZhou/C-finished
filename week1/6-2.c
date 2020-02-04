#include<stdio.h>
#include <math.h>

int main(void)
{
  int i,k,j=3;
  double x,y;

  printf("Enter a Number:");
  scanf("%lf",&x);
  y=x;
  for (i=0;i<1000000;i++)
  {
    y=pow(y,80.0);
    y=pow(y,1.0/80.0);
  }
  printf("%.10f %.10f\n",x,y);

  k = (int)pow ((double)j,3.0);
  printf("new is %d\n",k);
  return 0;
}
