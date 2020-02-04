/*测试 *（p++）和 *（++p） 以及string头文件   */
#include <stdio.h>

int main (void)
{
   int a[10]={1,2,3,4,5,6,7,8,9,10};
   int *p=a;

   printf("%d",*p);
   printf("%d",*p++);
   printf("%d",*p);
   printf("%d",*(p++));
   printf("%d",*p);
   printf("%d",*(++p));
   printf("%d",*p);
   printf("%d",(*p)++);
   printf("%d",*p);
  return 0;
}
