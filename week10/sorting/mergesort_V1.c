/* 本版本按照handout传递了一个空数组 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 5000

void mergeSort(int arr[],int result[], int l, int r);
void merge(int arr[],int result[], int l, int m, int r);
void printArray(int A[], int size);
void swap(int *a,int *b);

int main(void){

  int i, arr_size;
  int a[NUM];
  int result[NUM];

  for(i = 0; i < NUM; i++){
    a[i] = rand() % 100;
  }

  arr_size = sizeof(a)/sizeof(a[0]);

  printf("Given array is\n");
  printArray(a, arr_size);

  mergeSort(a,result, 0, arr_size - 1);
  printf("\nSorted array is\n");
  printArray(result, arr_size);

  return 0;
}

/* 如需创建新的array需传递 */
void mergeSort(int arr[],int result[], int l, int r){
  int m;
  if (l!=r){
    m = (l+r)/2;
    /*Sort first andsecond halves*/
    mergeSort(arr,result, l, m);
    mergeSort(arr,result, m+1, r);
    merge(arr,result, l, m, r);
 }
}


void merge(int arr[],int result[], int l, int m, int r){
  int s1 = l;
  int s2 = m+1;
  int k=l,i;

  while (s1 <= m && s2 <= r){
    if (arr[s1] <= arr[s2]){
      result[k++] = arr[s1++];
    }
    else {
     result[k++] = arr[s2++];
    }
  }
  /*将剩余未复制的数组拷贝进新array*/
  while (s1<=m){
    result[k++]=arr[s1++];
  }
  while (s2<=r){
    result[k++]=arr[s2++];
  }
  /*拷贝到原数组*/
  for (i=l;i<=r;i++){
    arr[i]=result[i];
  }
}

void printArray(int A[], int size){
  int i;
  for (i=0; i < size; i++)
    printf("%d ", A[i]);
  printf("\n");
}
