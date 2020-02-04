#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

int anagram(char s1[],char s2[]);
void permute (char *a,int l,int r);
void swap(char *A,char *B);

int main(void)
{
  char a1[]="elvis",a2[]="svlvi";
  char a3[]="elvis",a4[]="elvis";
  char a5[]="neil",a6[]="neill";
  printf("%s-%s\n",a1,a2);
  printf("%d\n",anagram(a1,a2));
  printf("%s-%s\n",a1,a2);
  assert(anagram(a1,a2)==1);
  assert(anagram(a3,a4)==0);
  assert(anagram(a5,a6)==0);
  return 0;
}
/*字符个数相等->比较两个数组是否相等，相等0，不等1->重拍，有一个相等返回1，全都不等返回0
不相等返回0*/
int anagram (char s1[],char s2[])
{
  int cnt1,cnt2;
  cnt1=strlen(s1);
  cnt2=strlen(s2);
  if (cnt1==cnt2) {
    if (strcmp(s1,s2)!=0) {
        permute(s1,0,cnt1-1);
        if (strcmp(s1,s2)==0)
        {return 1;}
      return 0;
    }
    else
      return 0;

  }
  else
    return 0;
}

void permute (char *a,int l,int r)
{
  int i;
  static int cnt=1;
  if (l==r){
    printf("%d-%s\n",cnt,a);
    cnt++;
    return ;
  }
  else{
  for (i=l;i<=r;i++)
  {
    if (a[l]==a[i]&& l!=i)
    continue;
    swap((a+l),(a+i));
    permute (a,l+1,r);
  }
}
}

void swap(char *A,char *B)
{
  char temp;
  temp=*A;
  *A=*B;
  *B=temp;
}
