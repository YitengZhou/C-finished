#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUMBER 3000
#define MAXLENTH 10

void performance(char *file_name,char a[MAXNUMBER][MAXLENTH]);
void backforward(char a[MAXNUMBER][MAXLENTH],int n,int m);
void test(void);

int main(int argc,char **argv)
{
  char list[MAXNUMBER][MAXLENTH]={{'\0'}};
  if (argc==2)
  {
    performance(argv[1],list);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s sternaig\n",argv[0]);
  }
  return 0;
}

void performance(char *file_name,char a[MAXNUMBER][MAXLENTH])
{
  FILE *fp;
  char temp[MAXLENTH];
  int cnt=0,i;

  if ((fp=fopen(file_name,"r"))==NULL){
    printf("Fail to open the file\n");
    exit(EXIT_FAILURE);
  }

  while (fscanf(fp,"%s",temp)==1){
    i=0;
    while (a[i][0]!='\0' && strcmp(temp,a[i])>0) {
      i++;
    }
    backforward(a,i,cnt);
    strcpy(a[i],temp);
    cnt++;
  }
  /*含break的写法
  while (fscanf(fp,"%s",temp)==1){
      for (i=0;i<cnt;i++){
        if (strcmp(temp,a[i]) <0){
          backforward(a,i,cnt);
          strcpy(a[i],temp);
          break;
        }
      }
      strcpy(a[i],temp);
      cnt++;
  }*/

  for (i=0;i<cnt;i++){
    printf("%s\n",a[i]);
  }
  test();
  printf("There are %d words\n",cnt);
  fclose(fp);
}

void backforward(char a[MAXNUMBER][MAXLENTH],int n,int m)
{
  int i,max=m;
  for (i=n;i<max;i++,m--) {
    strcpy(a[m],a[m-1]);
  }
}

void test()
{
  char x[MAXNUMBER][MAXLENTH]={"aaaa",
                               "bbbb",
                               "cccc",
                               "dddd",
                               "eeee",
                               "ffff",
                               "dsss",
                               "uuuu",
                               "dddd",
                               "llll",
                             };
  backforward(x,0,10);
}
