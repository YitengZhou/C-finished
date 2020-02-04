#include <stdio.h>
#include <string.h>

int main (void)
{
  char a[200]="zhouyiteng";
  char b[20]="maxiaoyuan";
  char d[200];
  printf("%s\n",a);
  strcat(a,b);
  printf("%s\n",a);
  strcpy(d,a);
  printf("%s\n",d);
  printf("%lu and %lu\n",strlen(a),strlen(b));
  printf("%d\n",strcmp(a,b));
  printf("%d\n",strcmp(b,a));
  return 0;
}
