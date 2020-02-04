#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

int isogram(char *s);

int main(void)
{
  assert(isogram("programming")==0);
  assert(isogram("housewarmings")==0);
  assert(isogram("abductions")==1);
  assert(isogram("housewarming")==1);
  assert(isogram("hydromagnetics")==1);
  assert(isogram("uncopyrightable")==1);
  return 0;
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
