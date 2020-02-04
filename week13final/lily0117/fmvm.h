/*what & how -- double hash
1、Establish a hash table to speed up the search, and ideally, the time
  complexity of searching by using a hash table is O(1);
2、Use double hash in open addressing to avoid confliction；
3、After apply this method, the speed is about 200 times than before;
steps as blew：
1、Choose an initial (prime) array size（111）, created hash table via malloc().
2、If this hash table gets more than 60% full, creates a new array, roughly twice
the size (but still prime).
3、Rehash all the words into this new array from the old one.
notice：Both key & data are strings (char*)
*/

/* Error that can't be ignored */
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

struct mvmcell {
   char* key;
   char* data;
};
typedef struct mvmcell mvmcell;

struct mvm {
   mvmcell** table;
   int curnumber;
   int totalnumber;
};
typedef struct mvm mvm;

typedef enum bool{false, true}bool;

#define MAXKEY 100     /* largest size of key and value */
#define MAXDATA 300
#define ENDSYMBAL 1    /* store the end symbal of string */
#define TABLESIZE 111  /* The original length which expand the capacity based on this */

/* mvm initialization, creating a hash table of initial size */
mvm* mvm_init(void);
/* Initialize each mvmcell node*/
mvmcell* mvmcell_init(void);
/* Check whether memory allocation was successful */
void check(void *m);
/* Number of key/value pairs stored */
int mvm_size(mvm* m);
/* Insert one key/value pair */
bool mvm_trytoinsert(mvmcell** table, char* key, char* data, int tablesize);
/* Store the data in Hash table. During the insertion process,
   check whether the dense of the hash table is greater than 0.6.
   If it is greater, re-establish the hash table. */
void mvm_insert(mvm* m, char* key, char* data);
/* Once the density is greater than 0.6,
   resize the hash map and re-insert the data
   from the original hash table into the new hash table*/
void resizeHash(mvm** m, int tablesize);
/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key);
/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n);
/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m);
/* Remove one key/value */
void mvm_delete(mvm* m, char* key);
/* hash1("ABC") = (37(37(37 + 'A') + 'B') + 'C') % size */
unsigned hash1(int tablesize, char* key);
/* hash("ABC") = ((5381 * 33 + 'A') * 33 + 'B') * 33 + 'C' */
unsigned hash2(int tablesize, char* key);
/*  Find the prime number near 2 * tablesize */
int findPrime(int tablesize);
/* Determine if a number is prime */
int isPrime(int number);
/* Free & set p to NULL */
void mvm_free(mvm** p);
/* free memcell node */
void mvmcell_free(mvmcell **mcell);
/* end */
