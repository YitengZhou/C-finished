#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXNUMBER 3000
#define MAXLENTH 10
#define CSEC (double)(CLOCKS_PER_SEC)

void performance(char *file_name,char a[MAXNUMBER][MAXLENTH]);
void backforward(char a[MAXNUMBER][MAXLENTH],int n,int m);
int in_file(char a[MAXNUMBER][MAXLENTH],char *str,int n);
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
  clock_t c1,c2;
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
  fclose(fp);
  test();
  for (i=0;i<cnt;i++){
    printf("%d:%s\n",i+1,a[i]);
  }
  printf("There are %d words\n",cnt);
  printf("Please enter which word you want search\n");
  scanf("%s",temp);
  c1= clock();
  if (in_file(a,temp,cnt)){
    printf("%s in the list, order is %d\n",temp,in_file(a,temp,cnt));
  }
  else{
    fprintf(stderr,"%s NOT in the list\n",temp);
    exit(EXIT_FAILURE);
  }
  c2= clock();
  printf("Time:%f\n",(double)(c2-c1)/CSEC);
}

void backforward(char a[MAXNUMBER][MAXLENTH],int n,int m)
{
  int i,max=m;
  for (i=n;i<max;i++,m--) {
    strcpy(a[m],a[m-1]);
  }
}
int in_file(char a[MAXNUMBER][MAXLENTH],char *str,int n)
{
  int i;
  for (i=0;i<n;i++){
    if (strcmp(a[i],str)==0){
      return i+1;
    }
  }
  return 0;
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
