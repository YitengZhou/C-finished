/* The aim of this testfmvm.c file is to test fmvm.c
   In addition to the similar parts from testmvm.c,
   I also add some assert to test GetKey() function
   insert key/value pair to double hashtable with separate chaining */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fmvm.h"

int main(void)
{

   int i, j;
   mvm *m;
   char* str;
   char** av;
   char animals[5][10] = {"cat",  "dog",  "bird",  "horse", "frog"};
   char  noises[5][10] = {"meow", "bark", "tweet", "neigh", "croak"};

   printf("Basic MVM Tests ... Start\n");
   /* Set up empty array */

   m = mvm_init();

   assert(m != NULL);
   assert(mvm_size(m)==0);

   /* test GetKey */
   assert(GetKey("a")==0);
   assert(GetKey("aaaa")==0);
   assert(GetKey("aaaaaaa")==0);
   assert(GetKey("z")==25);
   assert(GetKey("zz")==26*25+25);
   assert(GetKey("A")==abs('A'-'a'));
   assert(GetKey("Z")==abs('Z'-'a'));
   assert(GetKey(" ")==abs(' '-'a'));
   assert(GetKey("1")==abs('1'-'a'));

   /* Building and Searching */
   for(j=0; j<5; j++){

      mvm_insert(m, animals[j], noises[j]);
      assert(mvm_size(m)==j+1);
      i = strcmp(mvm_search(m, animals[j]), noises[j]);
      assert(i==0);
   }
   /* Test building & printing */
   str = mvm_print(m);

   /* Print by hash table order, so change the str
      block[1371] : [cat](meow){1371}
      block[2398] : [dog](bark){2398}
      block[23429] : [bird](tweet){23429}
      block[99742] : [frog](croak){99742}
      block[156783] : [horse](neigh){3456860} */
   i = strcmp(str, "[cat](meow) [dog](bark) [bird](tweet) [frog](croak) [horse](neigh) ");
   assert(i==0);
   free(str);

   /* Search for non-existent key */
   /* No-one knows what the fox says ? */
   assert(mvm_search(m, "fox") == NULL);

   /* Deletions - middle, then front */
   mvm_delete(m, "dog");
   assert(mvm_size(m)==4);
   str = mvm_print(m);

   /* Print by hash table order, so change the str
      block[1371] : [cat](meow){1371}
      block[23429] : [bird](tweet){23429}
      block[99742] : [frog](croak){99742}
      block[156783] : [horse](neigh){3456860} */
   i = strcmp(str, "[cat](meow) [bird](tweet) [frog](croak) [horse](neigh) ");
   assert(i==0);
   free(str);
   mvm_delete(m, "frog");
   assert(mvm_size(m)==3);
   str = mvm_print(m);

   /* Print by hash table order, so change the str
      block[1371] : [cat](meow){1371}
      block[23429] : [bird](tweet){23429}
      block[156783] : [horse](neigh){3456860} */
   i = strcmp(str, "[cat](meow) [bird](tweet) [horse](neigh) ");
   assert(i==0);
   free(str);
   /* Insert Multiple Keys */
   mvm_insert(m, "frog", "croak");
   mvm_insert(m, "frog", "ribbit");
   assert(mvm_size(m)==5);

   str = mvm_print(m);

   /* Print by hash table order, whole table seems like:
      block[1371] : [cat](meow)
      block[23429] : [bird](tweet)
      block[99742] : [frog](croak) -> [frog](ribbit)
      block[156783] : [horse](neigh) */
   i = strcmp(str, "[cat](meow) [bird](tweet) [frog](croak) [frog](ribbit) [horse](neigh) ");
   free(str);
   assert(i==0);

   mvm_insert(m, "frog", "1");
   mvm_insert(m, "frog", "2");
   mvm_insert(m, "frog", "3");
   mvm_insert(m, "frog", "4");
   mvm_insert(m, "frog", "5");
   assert(mvm_size(m)==10);

   str = mvm_print(m);
   /* Print by hash table order, whole table seems like:
      block[1371] : [cat](meow)
      block[23429] : [bird](tweet)
      block[99742] : [frog](croak) -> [frog](ribbit) -> [frog](1) -> [frog](2)
                     -> [frog](3) -> [frog](4) -> [frog](5) -> [frog](6)
                     -> [frog](7) -> [frog](8)
      block[156783] : [horse](neigh) */
   i = strcmp(str, "[cat](meow) [bird](tweet) [frog](croak) [frog](ribbit) \
[frog](1) [frog](2) [frog](3) [frog](4) [frog](5) [horse](neigh) ");
   free(str);
   assert(i==0);

   /* Search Multiple Keys */
   str = mvm_search(m, "frog");
   i = strcmp(str, "croak");
   assert(i==0);

   /* Multisearching */
   av = mvm_multisearch(m, "cat", &i);
   assert(i==1);
   i = strcmp(av[0], "meow");
   assert(i==0);
   free(av);
   av = mvm_multisearch(m, "horse", &i);
   assert(i==1);
   i = strcmp(av[0], "neigh");
   assert(i==0);
   free(av);
   av = mvm_multisearch(m, "frog", &i);
   assert(i==7);
   i = strcmp(av[0], "croak");
   j = strcmp(av[1], "ribbit");
   assert((i==0)&&(j==0));
   i = strcmp(av[2], "1");
   assert(i==0);
   i = strcmp(av[3], "2");
   assert(i==0);
   i = strcmp(av[4], "3");
   assert(i==0);
   i = strcmp(av[5], "4");
   assert(i==0);
   i = strcmp(av[6], "5");
   assert(i==0);
   free(av);

   /* Delete Multiple Keys */
   mvm_delete(m, "frog");
   mvm_delete(m, "frog");
   mvm_delete(m, "frog");
   mvm_delete(m, "frog");
   mvm_delete(m, "frog");

   mvm_delete(m, "frog");
   assert(mvm_size(m)==4);
   mvm_delete(m, "frog");
   assert(mvm_size(m)==3);
   str = mvm_print(m);

   /* Print by hash table order, whole table seems like:
      block[1371] : [cat](meow){1371}
      block[23429] : [bird](tweet){23429}
      block[156783] : [horse](neigh){3456860} */
   i = strcmp(str, "[cat](meow) [bird](tweet) [horse](neigh) ");
   assert(i==0);
   free(str);

   /* Weird NULL insert() edge cases */
   mvm_insert(m, NULL, "quack");
   assert(mvm_size(m)==3);
   mvm_insert(NULL, "duck", "quack");
   assert(mvm_size(m)==3);
   mvm_insert(m, "duck", NULL);
   assert(mvm_size(m)==3);

   /* Weird NULL delete() edge cases */
   mvm_delete(m, "");
   assert(mvm_size(m)==3);
   mvm_delete(m, NULL);
   assert(mvm_size(m)==3);
   mvm_delete(NULL, "frog");
   assert(mvm_size(m)==3);
   mvm_delete(m, "bird");
   assert(mvm_size(m)==2);
   str = mvm_print(m);

   /* Print by hash table order, whole table seems like:
      block[1371] : [cat](meow){1371}
      block[156783] : [horse](neigh){3456860} */
   i = strcmp(str, "[cat](meow) [horse](neigh) ");
   assert(i==0);
   free(str);

   /* Freeing */
   mvm_free(&m);
   assert(m==NULL);
   assert(mvm_size(m)==0);

   printf("Basic MVM Tests ... Stop\n");
   return 0;
}
