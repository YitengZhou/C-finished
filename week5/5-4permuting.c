#include <stdio.h>
#include <string.h>

void swap(char *A,char *B)
{
  char temp;
  temp=*A;
  *A=*B;
  *B=temp;
}

void permute (char *a,int l,int r);

int main(void)
{
  char str[]="ABC";
  int n=strlen(str);
  permute(str,0,n-1);
  return 0;
}

void permute (char *a,int l,int r)
{
  int i;
  if (l==r){
    printf("%s\n",a);
    return ;
  }
  else{
    for (i=l;i<=r;i++)
    {
      if (a[l]==a[i]&& l!=i)
      continue;
      swap((a+l),(a+i));
      permute (a,l+1,r);
      swap((a+l),(a+i));
    }
  }
}
