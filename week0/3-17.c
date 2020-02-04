#include <stdio.h>
int main(void)
{int n,i,con=0;
int num,max;
printf("How many numbers do you wish to enter?");
scanf("%d",&n);
printf("Enter %d real numbers:",n);
for (i=0;i<n;i++)
{
scanf("%d",&num);
if (i==0) max=num;
if (num%2==0)
printf("this even will delete, because I hate it!");
else
{con++;
if (num>max) max=num;}
}
printf("Maximum value in %d numbers:%5d\n",con,max);
return 0;
}
