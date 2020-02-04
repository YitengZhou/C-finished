#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void permute(char str[],int l,int r);
void swap(char *a,char *b);
void performance(char *file_name);
void permute(char str[],int l,int r);

FILE *ofp;
int cnt=1;

int main(int argc,char **argv)
{
  if (argc==2)
  {
    performance(argv[1]);
  }
  else {
    printf("ERROR: Incorrect usage. try e.g. "\
    "%s eng_370k_shuffle.txt\n",argv[0]);
  }
  return 0;
}

void performance(char *file_name) {
  FILE *ifp;
  char ch;
  char str[20];
  int i=0;
  ifp=fopen(file_name,"r");
  ofp=fopen("5-2output","w");
  if (ifp==NULL || ofp==NULL){
    printf("Failed to open the file\n");
    exit(0);
  }
  while ((ch=getc(ifp))!=EOF){
    if (ch!='\n'){
      str[i]=ch;
      i++;
    }
    else {
      str[i]='\0';
      permute(str,0,(int)strlen(str)-1);
      cnt=1;
      i=0;
    }
  }
  fclose(ifp);
  fclose(ofp);
}

void permute(char str[],int l,int r)
{
  int i;
  if (l==r) {
    fprintf(ofp,"%d->%s\n",cnt,str);
    cnt++;
    return;
  }
  else{
    for (i=l;i<=r;i++) {
      if (str[i]==str[l]&& i!=l)
        continue;
      swap(str+l,str+i);
      permute(str,l+1,r);
      swap(str+l,str+i);
    }
  }
}

void swap(char *a,char *b)
{
  char temp;
  temp=*a;
  *a=*b;
  *b=temp;
}
