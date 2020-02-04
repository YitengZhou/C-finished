#include <stdio.h>
#include <stdlib.h>

#include "set.h"

/* Create empty set */
set* set_init(void)
{
  set *s;
  s = (set*) calloc(1,sizeof(set));
  if(s == NULL){
     ON_ERROR("Creation of Set Failed\n");
  }
  return s;
}

/* Create new set, copied from another */
set* set_copy(set* s)
{
  set *new;
  int i;
  new=set_init();
  if (s==NULL){
    return new;
  }
  for (i=0;i<s->sz;i++){
    set_insert(new,s->ua->data[i]);
  }
  new->sz=s->sz;
  return new;
}

/* Create new set, copied from an array of length n*/
set* set_fromarray(arrtype* a, int n)
{
  set *new;
  int i;
  new=set_init();
  for (i=0;i<n;i++){
    set_insert(new,a[i]);
  }
  return new;
}

/* Basic Operations */
/* Add one element into the set */
void set_insert(set* s, arrtype l)
{
  int i,flag=0;
  if (s==NULL){
    return ;
  }
  if (s->sz==0){
    s->ua = (arr*) calloc(1,sizeof(arr));
  }
  for (i=0;i<s->sz;i++){
    if (arr_get(s->ua,i)==l){
      flag=1;
    }
  }
  if (flag!=1){
    arr_set(s->ua, s->ua->pz, l);
    s->sz++;
  }
  return ;
}

/* Return size of the set */
int set_size(set* s)
{
  if (s==NULL){
    return 0;
  }
  else{
    return s->sz;
  }
}

/* Returns true if l is in the array, false elsewise */
int set_contains(set* s, arrtype l)
{
  int i;
  if (s==NULL||s->sz==0){
    return 0;
  }
  else{
    for (i=0;i<s->ua->pz;i++){
      if (arr_get(s->ua,i)==l){
        return 1;
      }
    }
    return 0;
  }
}

/* Remove l from the set (if it's in) */
void set_remove(set* s, arrtype l)
{
  int i=0,flag=0;
  if (s==NULL||s->sz==0){
    return;
  }
  while (i<s->ua->pz){
    if (s->ua->data[i]==l){
      s->ua->data[i]=-1;/*需要free*/
      flag=1;
    }
  }
  if (flag==1){
    s->sz--;
  }
  return;
}

/* Remove one element from the set - there's no
   particular order for the elements, so any will do */
arrtype set_removeone(set* s)
{
  s->ua->pz--;
  s->sz--;
  return s->ua->data[s->sz];
}

/* Operations on 2 sets */
/* Create a new set, containing all elements from s1 & s2 */
set* set_union(set* s1, set* s2)
{
  set *new;
  int i;
  new=set_init();
  if (s1==NULL && s2==NULL){
    return new;
  }
  if (s1==NULL&& s2!=NULL){
    for (i=0;i<s2->sz;i++){
      set_insert(new,s2->ua->data[i]);
    }
  }
  if (s2==NULL&& s1!=NULL){
    for (i=0;i<s1->sz;i++){
      set_insert(new,s1->ua->data[i]);
    }
  }
  if (s1!=NULL&&s2!=NULL){
    for (i=0;i<s1->sz;i++){
      set_insert(new,s1->ua->data[i]);
    }
    for (i=0;i<s2->sz;i++){
      set_insert(new,s2->ua->data[i]);
    }
  }
  return new;
}
/* Create a new set, containing all elements in both s1 & s2 */
set* set_intersection(set* s1, set* s2)
{
  set *new;
  int i,j;
  new=set_init();
  if (s1==NULL || s2==NULL){
    return new;
  }
  else {
    for (i=0;i<s1->sz;i++){
      for (j=0;j<s2->sz;j++){
        if (s1->ua->data[i]==s2->ua->data[j]){
          set_insert(new,s1->ua->data[i]);
        }
      }
    }
  }
  return new;
}

/* Finish up */
/* Clears all space used, and sets pointer to NULL */
void set_free(set** s)
{
   set *a=NULL;
   if (s==NULL){
     return;
   }
   else {
     a = *s;
     if (a->sz!=0){
      free(a->ua->data);
     }
     free(a->ua);
     free(a);
     /* Helps to assert that the job has been done.*/
     *s = NULL;
     return;
   }
}
