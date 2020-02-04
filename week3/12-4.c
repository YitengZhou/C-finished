#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct llist {
  char word[50];
  struct llist *next;
};

int main (void)
{
  int j, cnt=0;
  struct llist head={"",NULL}, *cp;
  printf("Enter a list of words.\n");
  cp = &head;

  do {
    j = scanf ("%s",cp->word);
    cp->next= malloc(sizeof (struct llist));
    cp=cp->next;
    cp->next =NULL;
    strcpy(cp->word,"");
    cnt++;
  } while (j!=EOF);

  printf("%d word:\n",cnt-1);
  cp=&head;
  while (strcmp(cp->word,""))
  {
    printf("%s\n",cp->word);
    cp=cp->next;
  }
  return 0;
}
