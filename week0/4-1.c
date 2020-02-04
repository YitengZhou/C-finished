#include <stdio.h>
#define NDEBUG
#include <assert.h>

int main(void)
{
int a=1;
printf("good");
assert(a>0);
assert(a<0);
printf("good2");
return 0;}
