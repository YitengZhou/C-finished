#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

int isogram(char *s);
void test(void);
void open_and_print(char *file);

int main(int argc,char **argv)
{
  if (argc == 2) {
    open_and_print(argv[1]);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s eng_370k_shuffle.txt\n",argv[0]);
  }
  return 0;
}

void open_and_print(char *file)
{
  FILE *fp;
  int n=0,m=0;
  char ch;
  char temp[100],max[100];
  fp=fopen(file,"r");
  if (fp==NULL){
    printf("Failed to open the file.\n");
    exit(1);
  }
  while ((ch=getc(fp))!=EOF)
  {
    if (ch!='\n' && ch!=' ') {
      temp[n]=ch;
      n++;
    }
    else {
      temp[n]='\0';
      if (isogram(temp)==1 && (int)strlen(temp)>=m) {
        m = (int)strlen(temp);
        strcpy(max,temp);
      }
      n=0;
    }
  }
  test();
  printf("%s (%d)\n",max,m);
  fclose(fp);
}

int isogram(char *s)
{
  int list[26]={0};
  int i;
  for (i=0;i<(int)strlen(s);i++){
    list[s[i]-'a']++;
  }
  for (i=0;i<26;i++) {
    if (list[i]>1)
    return 0;
  }
  return 1;
}

void test(void)
{
assert(isogram("programming")==0);
assert(isogram("housewarmings")==0);
assert(isogram("abductions")==1);
assert(isogram("housewarming")==1);
assert(isogram("hydromagnetics")==1);
assert(isogram("uncopyrightable")==1);
}
