#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void performance(char *b);
void quick_sort(char arr[], int low, int high);
char sort_loop(char arr[], int low, int high);

int main(int argc,char **argv)
{
  if (argc==2)
  {
    performance(argv[1]);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s sternaig\n",argv[0]);
  }
  return 0;
}

void performance(char *b)
{
  FILE *fp;
  char ch;
  char a[50]={'\0'};
  char storage[50]={'\0'};
  int i=0;
  fp=fopen("eng_370k_shuffle.txt","r");
  if (fp==NULL){
    printf("Fail to open the file\n");
    exit(0);
  }
  quick_sort(b, 0, (int)strlen(b) - 1);

  while ((ch=getc(fp))!=EOF){
    if (ch!='\n'){
      a[i]=ch;
      i++;
    }
    else{
      a[i]='\0';
      strcpy(storage,a);
      quick_sort(a, 0, (int)strlen(storage) - 1);
      i=0;
      if (strcmp(b, a) == 0) {
        printf("%s\n",storage);
      }
    }
  }
  fclose(fp);
}

void quick_sort(char arr[], int low, int high)
{
  int position;

  if (low < high) {
    position = sort_loop(arr, low, high);
    quick_sort(arr, low, position - 1);
    quick_sort(arr, position + 1, high);
  }
}

char sort_loop(char arr[], int low, int high)
{
  int start = low, end = high;
  int key = arr[start];

  while(start < end) {
    while(start < end && arr[end] >= key){
      end--;
    }
    if(start < end) {
      arr[start++] = arr[end];
    }
    while(start < end && arr[start] < key) {
      start++;
    }
    if(start < end) {
      arr[end--] = arr[start];
    }
  }
  arr[start] = key;
  return start;
}
