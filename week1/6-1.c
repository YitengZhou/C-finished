#include <stdio.h>
int main(void)
{
printf("\n");
printf("    char:%3d\n",sizeof(char));
printf("   short:%3d\n",sizeof(short));
printf("     int:%3d\n",sizeof(int));
printf("    long:%3d\n",sizeof(long));
printf("unsigned:%3d\n",sizeof(unsigned));
printf("   float:%3d\n",sizeof(float));
printf("     dbl:%3d\n",sizeof(double));
printf("long dbl:%3d\n",sizeof(long double));
printf("\n");
return 0;
}
