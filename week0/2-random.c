#include <stdio.h>
#include <stdlib.h>

int main (void)
{
int i,n;
printf("\n%s\n%s","Randomly distributed integers""are printed.","How many do you want to see?\n");
scanf("%d",&n);
for (i=0;i<n;++i)
{if (i%6==0)
printf("\n");
printf("%20d",rand());
}
printf("\n");
printf("%d\n",RAND_MAX);
return 0;
}
