#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int pwdok(char *s);

int main(void)
{
  assert(pwdok("Ab1")==1);
  assert(pwdok("Ab#")==1);
  assert(pwdok("#abndsjksd2434")==0);
  assert(pwdok("Ab")==0);
  assert(pwdok("Ab?")==1);
  assert(pwdok("#abndsjksd2FFF44")==1);
  assert(pwdok("aa")==0);
  return 0;
}

int pwdok(char *s)
{
  int check[3]={0};
  int l,i;
  l=strlen(s);
  if (l<3) {
    return 0;
  }
  else {
    for (i=0;i<l;i++) {
      if (isupper(s[i])) {
        check[0]++;
      }
      if (islower(s[i])) {
        check[1]++;
      }
      if (isalpha(s[i])!=1) {
        check[2]++;
      }
    }
  }
  for (i=0;i<3;i++){
    if (check[i]==0) {
      return 0;
    }
  }
  return 1;
}
