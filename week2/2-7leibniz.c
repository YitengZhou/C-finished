#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define pi 3.14159265359
/*3.141592654*/
/*important to == or != with float*/

int main(void)
{
  int cnt=1;
  double sum=0;
  while (fabs(sum-pi)>1.0e-9)
  {
    if (cnt%2==1)
    sum=sum+4.0/(2*cnt-1);
    else
    sum=sum-4.0/(2*cnt-1);
    cnt++;
  }
  printf("pi is %.12f and count is %d\n",sum,cnt);
  return 0;
}

/*
#define FLOAT_ACCURACY 1.0e-9
int doublecomparr(double num)
{
if (num > pi- FLOAT_ACCURACY && num <  FLOAT_ACCURACY +pi)
return 1;
else
return 0;
}
*/
