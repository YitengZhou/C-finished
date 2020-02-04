#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define CSEC (double)(CLOCKS_PER_SEC)

int main(void){
  clock_t c1,c2;
  int i,j;
  c1= clock();
  for (i=0;i<1000000;i++){
    j=i*2;
  }
  c2= clock();
  printf("%f\n",(double)(c2-c1)/CSEC);
  return 0;
}
