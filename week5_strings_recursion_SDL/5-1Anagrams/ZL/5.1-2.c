#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM 46 /* the longest word has 45 letters */
/*
1. count the length of the string entered by the user;
2. sort the string entered by the user;
3. read lines from source file;
4. count the length of lines;
5. sort every line;
6. compare line and the string entered by the user, if they were the same, print the line;
*/
void execute(char* input_word);
int count(char* input_word);
void quick_sort(char arr[], int low, int high);
char sort_loop(char arr[], int low, int high);

int main(int argc, char *argv[])
{
  if(argc == 2){
    execute(argv[1]);
  } else {
    printf("ERROR: Incorrect usage. try e.g %s file.txt\n", argv[0]);
  }
  return 0;
}

void execute(char* input_word)
{
  char line[MAXIMUM], output[MAXIMUM];
  FILE* fp = fopen("eng_370k_shuffle.txt","r");
  int i, j, ch, count1, count2;

  if(fp == NULL) {
    printf("failed to open eng_370k_shuffle.txt.\n");
    exit(1);
  }

  count1 = count(input_word);
  quick_sort(input_word, 0, count1 - 1);

  do {
    i = 0;
    do {
      ch = (line[i++] = getc(fp));
    }while(ch != EOF && ch != '\n');
    /* '\n' has already stored in the line[i-1], we should change it to '\0' */
    line[i-1] = '\0';
    /* count the number of valid characters in the line before'\0' */
    count2 = count(line);
    /*store the origin line in the output array*/
    for (j = 0; j < count2; j++) {
      output[j] = line[j];
    }
    output[j] = '\0';
    quick_sort(line, 0, count2 - 1);
    if (strcmp(input_word, line) == 0) {
      printf("%s\n",output);
    }
  }while(ch != EOF);

  fclose(fp);
}
/*count the length*/
int count(char* input_word)
{
  int input_number = 0;
  char letter = *input_word;

  while(letter != '\0' && letter != '\n'){
    input_number++;
    letter = *(++input_word);
  }
  return input_number;
  printf("%d",input_number);
}
/*quick sort*/
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
