/* Multi-Value Map ADT : via Linked List
   Both key & data are strings (char*)
   Multiple Keys may be stored
   New data is inserted at the front of the list:
   O(1) insertion
   O(n) search
   O(n) deletion
*/

/* Error that can't be ignored */
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

struct mvmcell {
   int index;
   char* key;
   char* data;
   struct mvmcell *same;
};
typedef struct mvmcell mvmcell;

struct mvm {
   mvmcell* block;
   int numkeys;
   int hashsize;
};
typedef struct mvm mvm;

/* Creates the empty hashtable */
mvm* mvm_init(void);
/* Number of key/value pairs stored */
int mvm_size(mvm* m);
/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data);
/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m);
/* Remove one key/value */
void mvm_delete(mvm* m, char* key);
/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key);
/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n);
/* Free & set p to NULL */
void mvm_free(mvm** p);

/* Insert key/value pair to double hashtable */
void InsertBlock(mvmcell* cell, char* key, char* data);
/* Insert key/value pair to separate chaining */
void InsertSame(mvmcell* cell, char* key, char* data);
/* Getkey(key) = X4*26^4 + X3*26^3 + X2*26^2 + X1*26^1 + X0*26^0 */
int GetKey(char *str);
/* Getkey1(key) = Getkey(key) % HASHKEY1 */
int GetKey1(char *str);
/* Getkey2(key) = HASHKEY2 - Getkey(key) % HASHKEY2 */
int GetKey2(char *str);
/* Free mvmcell */
void mvmcell_free(mvmcell *cell);
/* Print all key/value in hashtable */
void PrintHashtable(mvm* m);