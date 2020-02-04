#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "nal.h"

void test(void)
{
   Prog *p = NULL;
   char *row = NULL;
   FILE *fp = NULL;

   printf("Basic Tests ... Start\n");

   /********************** strsame ***************************/
   assert(strsame("ABC", "ABC") == 1);
   assert(strsame("ABC", "AB") == 0);
   assert(strsame("ABC", "123") == 0);
   /********************** rowInit ***************************/
   row = rowInit();
   assert(row != NULL);
   free(row);
   /********************** progInit ***************************/
   p = progInit();
   assert(p->total == NUMTOKENS);
   assert(p != NULL);
   /********************** ifResize ***************************/
   ifResize(p, NUMTOKENS);
   assert(p->total == NUMTOKENS*2);
   freeMemory(&p);
   /********************** readIn ***************************/
   fp = tryOpenFile("test1.nal");
   p = readIn(fp);
   assert(strsame(p->codes[0], "{") == 1);
   assert(strsame(p->codes[1], "\n") == 1);
   assert(strsame(p->codes[2], "PRINT") == 1);
   /********************** readIn ***************************/
  
   /********************** readIn ***************************/
   /********************** readIn ***************************/
   /********************** readIn ***************************/
   /********************** readIn *****
   printf("|%s|\n", p->codes[1]);
   **********************/


   printf("Basic Tests ... Stop\n");

}
