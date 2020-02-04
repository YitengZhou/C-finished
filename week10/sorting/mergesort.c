#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 5000

void mergeSort(int arr[], int l, int r);
void merge(int arr[], int l, int m, int r);
void printArray(int A[], int size);

int main(void){

  int i, arr_size;
  int a[NUM];

  for(i = 0; i < NUM; i++){
    a[i] = rand() % 100;
  }

  arr_size = sizeof(a)/sizeof(a[0]);

  printf("Given array is\n");
  printArray(a, arr_size);

  mergeSort(a, 0, arr_size - 1);
  printf("\nSorted array is\n");
  printArray(a, arr_size);
  return 0;
}

/* 如需创建新的array需传递 */
void mergeSort(int arr[], int l, int r){
  int m;
  if (l!=r){
    m = (l+r)/2;
    /*Sort first and second halves*/
    mergeSort(arr, l, m);
    mergeSort(arr, m+1, r);
    merge(arr, l, m, r);
 }
}


void merge(int arr[], int l, int m, int r){
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;
  int L[NUM/2+1], R[NUM/2+1];
/* create temp arrays */

/* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1+ j];

/* Merge the temp arrays back into arr[l..r]*/
  i = 0; /* Initial index of first subarray */
  j = 0; /* Initial index of second subarray */
  k = l; /* Initial index of merged subarray */
  while (i < n1 && j < n2){
    if (L[i] <= R[j]){
      arr[k] = L[i];
      i++;
    }
    else
    {
     arr[k] = R[j];
     j++;
    }
    k++;
  }

 /* Copy the remaining elements of L[], if there
 are any */
  while (i < n1){
    arr[k] = L[i];
    i++;
    k++;
  }

 /* Copy the remaining elements of R[], if there
 are any */
  while (j < n2){
    arr[k] = R[j];
    j++;
    k++;
  }
}

void printArray(int A[], int size){
  int i;
  for (i=0; i < size; i++)
    printf("%d ", A[i]);
  printf("\n");
}
