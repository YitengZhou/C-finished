#include <stdio.h>
int main(void)
{
int i,j;
for (i=0;i<4;i++)
{for (j=0;j<4-i;j++)
printf("  ");
printf("CCCC\n");}
for (i=0;i<4;i++)
printf("CCCC\n");
for (i=0;i<4;i++)
{for (j=0;j<i+1;j++)
printf("  ");
printf("CCCC\n");}
return 0;
}
