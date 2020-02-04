#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*若读取1.txt文件需要改成10000和30,34words为4000和10*/
#define MAXNUMBER 10000
#define MAXLENTH 30
#define CSEC (double)(CLOCKS_PER_SEC)

void performance(char *file_name,char a[MAXNUMBER][MAXLENTH]);
void backforward(char a[MAXNUMBER][MAXLENTH],int n,int m);
int in_file(char a[MAXNUMBER][MAXLENTH],char *str,int n);
int binary(char a[MAXNUMBER][MAXLENTH],char *str,int n);
int interpolation(char a[MAXNUMBER][MAXLENTH],char *str,int n);
void test(void);

int main(int argc,char **argv)
{
  static char list[MAXNUMBER][MAXLENTH]={{'\0'}};
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
  /*for (i=0;i<cnt;i++){
    printf("%d:%s\n",i+1,a[i]);
  }*/
  printf("There are %d words\n",cnt);
  printf("Please enter which word you want search\n");
  scanf("%s",temp);

/*normal searching*/
  c1= clock();
  if (in_file(a,temp,cnt)){
    printf("%s in the list, order is %d\n",temp,in_file(a,temp,cnt));
  }
  else{
    fprintf(stderr,"%s NOT in the list\n",temp);
    exit(EXIT_FAILURE);
  }
  c2= clock();
  printf("Time of normal searching:%f\n",(double)(c2-c1)/CSEC);

/*binary searching*/
  c1= clock();
  if (binary(a,temp,cnt)){
    printf("%s in the list, order is %d\n",temp,binary(a,temp,cnt));
  }
  else{
    fprintf(stderr,"%s NOT in the list\n",temp);
    exit(EXIT_FAILURE);
  }
  c2= clock();
  printf("Time of binary searching:%f\n",(double)(c2-c1)/CSEC);

/*interpolation searching*/
  c1= clock();
  if (interpolation(a,temp,cnt)){
    printf("%s in the list, order is %d\n",temp,interpolation(a,temp,cnt));
  }
  else{
    fprintf(stderr,"%s NOT in the list\n",temp);
    exit(EXIT_FAILURE);
  }
  c2= clock();
  printf("Time of interpolation searching:%f\n",(double)(c2-c1)/CSEC);
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

int binary(char a[MAXNUMBER][MAXLENTH],char *str,int n)
{
  int l,r,m;
  l=0;
  r=n-1;
  while (l<=r) {
    m=(l+r)/2;
    if (strcmp(str,a[m])==0) {
      return m+1;
    }
    else{
      if (strcmp(str,a[m])<0) {
        r=m-1;
      }
      else {
        l=m+1;
      }
    }
  }
  return 0;
}

int interpolation(char a[MAXNUMBER][MAXLENTH],char *str,int n)
{
  int l,r,m;
  l=0;
  r=n-1;
  m=(str[0]-'a')/('z'-'a')*(n-1);
  while (l<=r) {
    if (strcmp(str,a[m])==0) {
      return m+1;
    }
    else{
      if (strcmp(str,a[m])<0) {
        r=m-1;
      }
      else {
        l=m+1;
      }
    }
    m=(l+r)/2;
  }
  return 0;
  }
