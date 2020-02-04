#include <stdio.h>
#include <stdlib.h>

#define NUM 100

int intcompare(const void *a,const void *b);
int charcompare(const void *a,const void *b);
void printArray(int a[], int size);
void printChar(char b[][10], int size);

int main(void)
{
  int a[NUM];
  int i;
  char b[5][10]={
    {"banana"},
    {"zoos"},
    {"gift"},
    {"element"},
    {"manage"}
  };
  for (i=0;i<NUM;i++){
    a[i]=rand()%100;
  }
  printArray(a,NUM);
  qsort(a,NUM,sizeof(int),intcompare);
  printArray(a,NUM);
  printf("This is given\n");
  printChar(b,5);
  /* 本处注意sizeof(b[0]每一个字符串长度) */
  qsort(b,5,sizeof(b[0]),charcompare);
  printf("This is sorted\n");
  printChar(b,5);
  return 0;
}

int intcompare(const void *a,const void *b)
{
  const int *ia=(const int *)a;
  const int *ib=(const int *)b;
  return *ia-*ib;
}

int charcompare(const void *a,const void *b)
{
  const char *ia=(const char *)a;
  const char *ib=(const char *)b;
  return *ia-*ib;
}

void printArray(int a[], int size){
  int i;
  for (i=0; i < size; i++)
    printf("%d ", a[i]);
  printf("\n");
}

void printChar(char b[][10], int size)
{
  int i;
  for (i=0; i < size; i++){
    printf("%s\n", b[i]);
  }
  printf("\n");
}
