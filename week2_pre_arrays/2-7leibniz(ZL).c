#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define pi 3.14159265359
#define FLOAT_ACCURACY 1.0e-9
/*3.141592654*/
/*important to == or != with float*/
int doublecompare(double num);

int main(void)
{
  int cnt=0,sign=1,i;
  double pai=0;
  for (i=1;doublecompare(pai)!=1;i+=2)
  {
    if (sign==1)
    {pai+=4.0/(double)i;
    sign=0;}
    else
    {pai-=4.0/(double)i;
    sign=1;}
    cnt++;
  }
  printf("pi is %.12f and count is %d\n",pai,cnt);
  return 0;
}

int doublecompare(double num)
{
  if (num>pi -FLOAT_ACCURACY && num< pi+FLOAT_ACCURACY)
  return 1;
  else
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
