#include <stdio.h>
int main(void)
{
  int hour1,min1,hour2,min2;
  int hourd,mind,a;
  printf("Enter two times:\n");
  scanf("%d:%d %d:%d",&hour1,&min1,&hour2,&min2);
  hourd= hour2-hour1;
  if (hourd<0) hourd +=24;
  mind= min2-min1;
  if (mind<0) hourd +=60;
  printf("Difference is : ");
  printf("%d:%d\n",hourd,mind);
  return 0;
}
