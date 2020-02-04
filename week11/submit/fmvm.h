/* Multi-Value Map ADT : via hashing with double hashing and separate chaining. */
/*
  For faster search time, the hash table is a good choice.
  The reason why I select double hash and separate chain is that
  Map1 is more appropriate to the double hash because every key(word) is unique,
  but Map2 is reverse, where there are lots of collisions happened,
  especially when -n flag set very small, for example 1 or 2.
  According to this, I believe Map2 is more advantageous to separate chaining,
  which means a small rhyme (e.g  -n 2 PASSING-> IHO NG) would not create
  thousands of collisions, it just double hash and find totally same key (IHO NG),
  and then insert at the end of the separate chaining (because there is
  a little trouble when I want to insert at the front of array-based list).
  In this case, IHO NG maybe linked thousands of words,
  but it's also convenient for search and print because
  you can print all same rhyme words following this chain rather than doublehash
  and strcmp thousands of times. Therefore, I combined the advantages of
  double hashing and separate chaining to deal with this problem.

  map1 (double hash) :
  ......
            hash(GetKey1) -> block[31] : [B](B IY1)
            hash(GetKey1) -> block[32] : [A](AH0)
  hash(GetKey1+i*GetKey2) -> block[33] : [GENTILE'S](AY2 L Z)
            hash(GetKey1) -> block[34] : [HANISEE](IH0 S IY0)
                             block[35] :
  hash(GetKey1+i*GetKey2) -> block[36] : [OXIDIZED](AY2 Z D)
                             block[37] :
                             block[38] :
            hash(GetKey1) -> block[39] : [HANING](N IH0 NG)
  ......
  map2 (double hash with separate chaining):
  ......
            hash(GetKey1) -> block[125] : [OY1 Z IY0](NOISY)
                                            -> [OY1 Z IY0](BOISE)
                                              -> [OY1 Z IY0](FOISY)
                             block[126] :
                             block[127] :
                             block[128] :
                             block[129] :
  hash(GetKey1+i*GetKey2) -> block[126] : [OY1 Y AH0](JOLLA)
            hash(GetKey1) -> block[130] : [OY1 UW2 Z](SOYUZ(1))
  hash(GetKey1+i*GetKey2) -> block[131] : [OY1 T ER0](KREUTER)
                                           -> [OY1 T ER0](LOITER)
                                            -> [OY1 T ER0](RECONNOITER)
                                             -> [OY1 T ER0](REUTTER)
                                              -> [OY1 T ER0](RECONNOITRE)
                                               -> [OY1 T ER0](GOITER)
                                                -> [OY1 T ER0](REUTER)
                                                 -> [OY1 T ER0](BOYTER)
  ......
  After experiment, this method indeed has a faster search time than 11.2,
  although it has a slower create and free maps time. More details and data
  in the 'MVMs data analysis.xlsx' file, which I will submit it in this assessment.
  Compare mvm vs fmvm:
  1. As searching and print time showing, 11.3 faster MVMs has a faster
     search time than 11.2 MVMs in different 10 cases.
  2. Because 11.2 MVMs is a simple linked list, it inserts, creates
     and frees two maps is faster than faster MVMs, which need to
     double hash into the list and avoid collisions.
  3. In different 10 cases, 11.2 MVMs changes slightly compare with
     11.3 faster MVMs, which will be slower if -n set to 1 or 2.
     The reason seems to be that there are more same keys in map2
     when -n set 1 or 2, which leads to slow down as inserting the same key
     into a double hashtable separate chain.

  More details, I write a function PrintHashtable(mvm* m) to print hashtable
  map1 or map2, and include <time.h> to print how many time I spend on different
  parts. I didn't delete those just comment it in case you want to browse and test.
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
