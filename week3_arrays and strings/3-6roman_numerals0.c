#include <stdio.h>


int main(void)
{
  int i=0,j,num,n;
  char *roman_number[][10]={{"","I","II","III","IV","V","VI","VII","VIII","IX"},\
  {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"},\
  {"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"},\
  {"","M","MM","MMM"}};
  for(num=1;num<4000;num++)
  {
  printf("\n%d:",num);
  for (j=0,i=10000;j<4;j++,i/=10)
  {
    n=(num%i)/(i/10);
    printf("%s",roman_number[3-j][n]); /*输出%s,给的是地址即指针*/
  }
  }
  printf("\n");
  return 0;
}
