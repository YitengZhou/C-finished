#include <stdio.h>
int main(void)
{int n,i;
float num,max;
printf("How many numbers do you wish to enter?");
scanf("%d",&n);
printf("Enter %d real numbers:",n);
for (i=0;i<n;i++)
{
scanf("%f",&num);
if (i==0) max=num;
if (num>max) max=num;
}
printf("Maximum value:%5.2f\n",max);
return 0;
}
