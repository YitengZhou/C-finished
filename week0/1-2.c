#include <stdio.h>
int main(void)
{
int a,b,c,d=0;
printf("Input three intergers:");
scanf("%d %d %d",&a,&b,&c);
d= 2*(a+b+c)+c;
printf("Twice the sum of integers plus %d is %d !", c,d);
return 0;
}
