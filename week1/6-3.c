#include <stdio.h>
#include <math.h>

int main(void)
{
  double x;
  double z;
  int y;
  for (x=0.0;x<10.0;x++)
  {y = (double)sin(x)*sin(x)+ (double)cos(x)*cos(x)-1;
    z= sin(x)*sin(x)+ cos(x)*cos(x);
  printf("%d %.20f\n",y,z);}
  return 0;
}
