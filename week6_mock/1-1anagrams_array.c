#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

int anagram(char s1[],char s2[]);

int main(void)
{
  assert(anagram("elvis","lives")==1);
  assert(anagram("dreads","sadder")==1);
  assert(anagram("replays","parsley")==1);
  assert(anagram("listen","silent")==1);
  assert(anagram("orchestra","carthorse")==1);
  assert(anagram("aee","eea")==1);

  assert(anagram("elvis","elvis")==0);
  assert(anagram("neill","neil")==0);
  assert(anagram("neil","neill")==0);
  assert(anagram("horse","short")==0);
  return 0;
}
/*字符个数相等->比较两个数组是否相等，相等0，不等1->重拍，有一个相等返回1，全都不等返回0
不相等返回0*/
int anagram (char s1[],char s2[])
{
  int cnt1,cnt2,i;
  int list1[26]={0},list2[26]={0};
  cnt1=strlen(s1);
  cnt2=strlen(s2);
  if (cnt1==cnt2) {
    if (strcmp(s1,s2)!=0) {
      for (i=0;i<cnt1;i++) {
        list1[s1[i]-'a']++;
        list2[s2[i]-'a']++;
      }
      for (i=0;i<26;i++) {
        if (list1[i]!=list2[i]){
          return 0;
        }
      }
      return 1;
    }
    else
      return 0;
  }
  else
    return 0;
}
