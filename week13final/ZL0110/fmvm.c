#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fmvm.h"
#include "nal.h"

#define MAXKEY 100     /* largest size of key and value */
#define MAXDATA 300
#define ENDSYMBAL 1    /* store the end symbal of string */
#define TABLESIZE 111  /* The original length which expand the capacity based on this */

/* mvm initialization, creating a hash table of initial size */
mvm* mvm_init(void)
{
   int i;
   mvm* m = (mvm*) calloc(1,sizeof(mvm));

   check(m);
   m->totalnumber = TABLESIZE;
   m->table = (mvmcell**)calloc(TABLESIZE, sizeof(mvmcell*));
   for (i = 0; i < TABLESIZE; i++) {
      *(m->table + i) = mvmcell_init();
   }
   return m;
}

/* Initialize each mvmcell node*/
mvmcell* mvmcell_init(void)
{
   mvmcell* mcell = (mvmcell*)calloc(1, sizeof(mvmcell));

   mcell->key = (char*)calloc(MAXKEY, sizeof(char));
   check(mcell->key);
   mcell->data = (char*)calloc(MAXDATA, sizeof(char));
   check(mcell->data);
   return mcell;
}

/* Check if memory allocation was successful */
void check(void *m)
{
   if(m == NULL){
      ON_ERROR("Creation Failed\n");
   }
}

/* Number of key/value pairs stored */
int mvm_size(mvm* m)
{
   if (m == NULL) {
      return 0;
   }
   return m->curnumber;
}

/* Insert one key/value pair */
bool mvm_trytoinsert(mvmcell** table, char* key, char* data, int tablesize)
{
   int i;
   unsigned addr = hash1(tablesize, key), offset = hash2(tablesize, key);
   unsigned newaddr;

   /* If the position is empty, put the data in the position and return success */
   if ( table[addr]->key[0] == '\0') {
      strcpy(table[addr]->key, key);
      strcpy(table[addr]->data, data);
      return true;
   } else {
      /* Double hash, add offset, repeat the above steps */
      for (i = 0; i < tablesize; i++) {
         newaddr = (addr + i * offset) % tablesize;
         if ( table[newaddr]->key[0] == '\0') {
            strcpy(table[newaddr]->key, key);
            strcpy(table[newaddr]->data, data);
            return true;
         }
      }
   }
   return false;
}

/* Store the data in Hash table. During the insertion process,
   check if the dense of the hash table is greater than 0.6.
   If it is greater, re-create the hash table. */
void mvm_insert(mvm* m, char* key, char* data)
{
   int tablesize;
   float dense = 0;

   if (m == NULL || key == NULL || data == NULL) {
      return;
   }
   tablesize = m->totalnumber;
   if (mvm_trytoinsert(m->table, key, data, tablesize)) {
      m->curnumber++;
   }
   dense = (float)(m->curnumber) / (float)(m->totalnumber);
   /* Once the density is greater than 0.6,
   re-create a space of about twice the size for storage */
   if (dense > 0.6) {
      /* Find the size of the new space, which is twice the original prime number */
      tablesize = findPrime(tablesize);
      /* Re-create the hash table */
      resizeHash(&m, tablesize);
   }
}

/* Once the density is greater than 0.6,
   resize the hash map and re-insert the data
   from the original hash table into the new hash table*/
void resizeHash(mvm** m, int tablesize)
{
   int i, count = 0;
   mvmcell** newtable = NULL, **temp = (*m)->table;

   newtable = (mvmcell**)calloc(tablesize, sizeof(mvmcell*));
   for (i = 0; i < tablesize; i++) {
      *(newtable + i) = mvmcell_init();
   }
   for (i = 0; i < (*m)->totalnumber; i++) {
      if ((*m)->table[i]->key[0] != '\0') {
         if (mvm_trytoinsert(newtable, (*m)->table[i]->key,
            (*m)->table[i]->data, tablesize)) {
            count++;
         }
      }
   }
   for (i = 0; i < (*m)->totalnumber; i++) {
     mvmcell_free(&temp[i]);
   }
   free(temp);
   (*m)->table = newtable;
   (*m)->curnumber = count;
   (*m)->totalnumber = tablesize;
}

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key)
{
   unsigned addr = hash1(m->totalnumber, key);
   unsigned offset = hash2(m->totalnumber, key);
   unsigned newaddr;
   int i;

   for (i = 0; i < m->totalnumber ; i++) {
      newaddr = (addr + i * offset) % m->totalnumber;
      if ( strcmp( m->table[newaddr]->key, key ) == 0) {
         return m->table[newaddr]->data;
      }
   }
   return NULL;
}

/* Return *argv[] list of pointers to all values stored with key,
   n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n)
{
   char** result = (char**) calloc(1,sizeof(char**));
   unsigned addr = hash1(m->totalnumber, key);
   unsigned offset = hash2(m->totalnumber, key);
   unsigned newaddr;
   int i = 0;

   *n = 0;
   do {
      newaddr = (addr + i * offset) % m->totalnumber;
      if ( strcmp( m->table[newaddr]->key, key) == 0) {
         /* If more than one matching data is found,
            a larger space needs to be created to point to the new location */
         if (*n >= 1) {
            result = (char**)realloc(result, (*n+1) * sizeof(char**));
         }
         /* Point the newly created pointer to the searched location */
         result[*n] = m->table[newaddr]->data;
         (*n)++;
      }
      i++;
   }while(m->table[newaddr]->key[0] != '\0');
   return result;
}

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
   char* cell = (char*) calloc(MAXDATA,sizeof(char));
   char* str = (char*) calloc(1,sizeof(char));
   int i, len;

   for (i = 0; i < m->totalnumber; i++) {
      if (m->table[i]->key[0] != '\0') {
         sprintf(cell, "[%s](%s) ", m->table[i]->key, m->table[i]->data);
         len = (int)strlen(str) + (int)strlen(cell);
         str = (char*) realloc(str, len * sizeof(char) + ENDSYMBAL);
         strcat(str, cell);
      }
   }
   free(cell);
   return str;
}

/* Remove one key/value */
void mvm_delete(mvm* m, char* key)
{
   unsigned addr, offset, newaddr;
   int i = 0;

   /* If the parameter passed in is empty or the key value
   to be deleted is not in the linked list, exit directly*/
   if (m == NULL || key == NULL || key[0] == '\0' \
       || mvm_search(m, key) == NULL) {
      return;
   }
   addr = hash1(m->totalnumber, key);
   offset = hash2(m->totalnumber, key);
   newaddr = addr % m->totalnumber;
   while(m->table[newaddr]->key[0] != '\0') {
      if(!strcmp(m->table[newaddr]->key, key)) {
         strcpy(m->table[newaddr]->key, "");
         strcpy(m->table[newaddr]->data, "");
         m->curnumber--;
         return;
      }
      i++;
      newaddr = (addr + i * offset) % m->totalnumber;
   }
}

/* hash1("ABC") = (37(37(37 + 'A') + 'B') + 'C') % tablesize */
unsigned hash1(int tablesize, char* key)
{
   int hashval = 0, i = 0;

   while(key[i] != '\0') {
      hashval = 37 * hashval + key[i++];
   }
   hashval %= tablesize;
   if (hashval < 0) {
      hashval += tablesize;
   }
   return (unsigned)hashval;
}

/* hash("ABC") = ((5381 * 33 + 'A') * 33 + 'B') * 33 + 'C' */
unsigned hash2(int tablesize, char* key)
{
   unsigned hash = 5381;
   int c;

   while ((c = *key++) != '\0') {
      hash = ((hash << 5) + hash) + c;
   }
   return (unsigned)(hash % tablesize);
}

/* Find the prime number near 2 * tablesize */
int findPrime(int tablesize)
{
   int i, newsize = 2 * tablesize;

   for(i = newsize; ;i++) {
      if (isPrime(i)) {
         return i;
      }
   }
   return newsize;
}

/* Determine if a number is prime */
int isPrime(int number)
{
   int i;

   if (number == 2) {
      return 1;
   }
   for(i = 2; i < number; i++) {
      if (number % i == 0) {
         return 0;
      }
   }
   return 1;
}

/* Free & set p to NULL */
void mvm_free(mvm** p)
{
   mvm *a = *p;
   int i;
   if (p == NULL) {
      return;
   }
   for (i = 0; i < a->totalnumber; i++) {
      mvmcell_free(&a->table[i]);
   }
   free(a->table);
   free(a);
   *p=NULL;
}
/* free memcell node */
void mvmcell_free(mvmcell **mcell)
{
   mvmcell *a = *mcell;

   free(a->key);
   free(a->data);
   free(a);
   a = NULL;
}
/* end */
