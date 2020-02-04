#include <stdio.h>
int main(void)
{
float a,b,c;
float temp;
printf("please, input the 3 sides of a triangle\n");
scanf("%f%f%f",&a,&b,&c);
if (a<b)
{temp=a;
a=b;
b=temp;
}
if (a<c)
{temp=a;
a=c;
c=temp;
}
if (b<c)
{temp=b;
b=c;
c=temp;
}
if (a<c+b)
{
if (c==a && b==a)
printf("this triangle is a equilateral triangle\n");
else if (c==a || c==b || a==b)
printf("this triangle is a isosceles triangle\n");
else
printf("this triangle is a scalene triangle\n");
if (a*a== c*c + b*b)
printf("this triangle is a right angled triangle\n");}

else
printf("it is not a triangle!!!\n");

return 0;
}
