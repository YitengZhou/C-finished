#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

int anagram(char s1[],char s2[]);

int main(void)
{
  char a1[]="elvis",a2[]="lives";
  char a3[]="dreads",a4[]="sadder";
  char a5[]="replays",a6[]="parsley";
  char a7[]="listen",a8[]="silent";
  char a9[]="orchestra",a10[]="carthorse";
  char a11[]="aee",a12[]="eea";

  char b1[]="elvis",b2[]="elvis";
  char b3[]="neill",b4[]="neil";
  char b5[]="neil",b6[]="neill";
  char b7[]="horse",b8[]="short";

  assert(anagram(a1,a2)==1);
  assert(anagram(a3,a4)==1);
  assert(anagram(a5,a6)==1);
  assert(anagram(a7,a8)==1);
  assert(anagram(a9,a10)==1);
  assert(anagram(a11,a12)==1);

  assert(anagram(b1,b2)==0);
  assert(anagram(b3,b4)==0);
  assert(anagram(b5,b6)==0);
  assert(anagram(b7,b8)==0);
  return 0;
}
/*字符个数相等->比较两个数组是否相等，相等0，不等1->重拍，有一个相等返回1，全都不等返回0
不相等返回0*/
int anagram (char s1[],char s2[])
{
  int cnt1,cnt2,i,j;
  cnt1=strlen(s1);
  cnt2=strlen(s2);
  if (cnt1==cnt2) {
    if (strcmp(s1,s2)!=0) {
      for(i=0;i<cnt1;i++) {
        for (j=0;j<cnt2;j++) {
          if (s1[i]==s2[j])
            {s1[i]=s2[j]='*';}
        }
      }
      if (strcmp(s1,s2)==0){
        return 1;
      }
      else
        return 0;
    }
    else
      return 0;
  }
  else
    return 0;
}
