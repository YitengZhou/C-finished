#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int partition(int *a,int l,int r);
void quicksort(int *a,int l,int r);
void printArray(int a[], int size);

#define NUM 100

int main(void)
{
  int i,arr_size;
  int a[NUM];
  for (i=0;i<NUM;i++){
    a[i]=rand()%100;
  }
  arr_size = sizeof(a)/sizeof(a[0]);

  printf("Given array is\n");
  printArray(a, arr_size);
  quicksort(a,0,NUM-1);
  printf("\nSorted array is\n");
  printArray(a, arr_size);
  return 0;
}

void quicksort(int *a, int l, int r)
{
  int pivpoint;
  pivpoint = partition(a,l,r);
  if (l<pivpoint){
    quicksort(a,l,pivpoint-1);
  }
  if (r>pivpoint){
    quicksort(a,pivpoint+1,r);
  }
}

int partition(int *a,int l,int r)
{
  int piv;
  piv=a[l];
  while (l<r){
    while (piv < a[r] && l<r) {
      r--;
    }
    if (r!=l){
      a[l]=a[r];
      l++;
    }
    while (piv > a[l] && l<r) {
      l++;
    }
    if (r!=l){
      a[r]=a[l];
      r--;
    }
  }
  a[r]=piv;
  return r;
}

void printArray(int a[], int size){
  int i;
  for (i=0; i < size; i++)
    printf("%d ", a[i]);
  printf("\n");
}
