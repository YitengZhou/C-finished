#include <stdio.h>
int returnSum(int iNum1,int iNum2);
int main(void)
{
int i,j,k;
j=2;k=3;
i=returnSum(j,k);
printf("The sum of j and k is %d\n",i);
return 0;
}

int returnSum(int iNum1,int iNum2)
{int i;
i = iNum1 +iNum2;
return i;
}
