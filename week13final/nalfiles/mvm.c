/* This mvm.c file create Multi-Value Map ADT : via Linked List */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mvm.h"

#define STRSIZE 1000
#define PRINTSIZE 100
#define MAXREPEAT 10

mvm* mvm_init(void)
{
  mvm *init;
  init=(mvm*)calloc(1,sizeof(mvm));
  if(init == NULL){
     ON_ERROR("Creation of MultiValue Maps Failed\n");
  }
  return init;
}

/* Number of key/value pairs stored */
int mvm_size(mvm* m)
{
  if (m==NULL){
    return 0;
  }
  return m->numkeys;
}

/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data)
{
  mvmcell *new;
  /* When mvm, key or data is NULL */
  if (m==NULL || key==NULL || data==NULL){
    return ;
  }
  /* Otherwise, insert new cell into head
     creat sizeof key/data is strlen(key/data) + 1, 1 for '\0' */
  new=(mvmcell*)calloc(1,sizeof(mvmcell));
  new->key=(char *)calloc(strlen(key)+1,sizeof(char));
  new->data=(char *)calloc(strlen(data)+1,sizeof(char));
  if(new == NULL||new->key==NULL||new->data==NULL){
     ON_ERROR("Insert Key and Data Failed\n");
  }
  strcpy(new->key,key);
  strcpy(new->data,data);
  m->numkeys++;
  new->next=m->head;
  m->head=new;
}

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
  mvmcell *print;
  char *str,*temp;
  str=calloc(STRSIZE,sizeof(char));
  temp=calloc(PRINTSIZE,sizeof(char));
  if (str==NULL||temp==NULL){
    ON_ERROR("Printing of MultiValue Maps Failed\n");
  }
  print=m->head;
  while (print!=NULL){
    sprintf(temp,"[%s](%s) ",print->key,print->data);
    strcat(str,temp);
    print=print->next;
  }
  free(temp);
  return str;
}


/* Remove one key/value */
void mvm_delete(mvm* m, char* key)
{
  mvmcell *current,*temp;
  /* When mvm, key or data is NULL */
  if (m==NULL || key==NULL || key[0]=='\0'){
    return ;
  }
  /* Otherwise, delete this key/value cell */
  current=m->head;
  if (strcmp(current->key,key)==0){
    m->head=current->next;
    free(current->key);
    free(current->data);
    free(current);
    m->numkeys--;
  }
  else{
    while (current->next!=NULL){
      if (strcmp(current->next->key,key)==0){
        temp=current->next->next;
        free(current->next->key);
        free(current->next->data);
        free(current->next);
        current->next=temp;
        m->numkeys--;
      }
      current=current->next;
    }
  }
}

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key)
{
  mvmcell *cell;
  /* When mvm, key or data is NULL */
  if (m==NULL || key==NULL){
    return NULL;
  }
  /* Otherwise, search value for key */
  cell=m->head;
  while (cell!=NULL){
    if (strcmp(cell->key,key)==0){
      return cell->data;
    }
    cell=cell->next;
  }
  return NULL;
}

/* Return *argv[] list of pointers to all values stored with key,
   n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n)
{
  char **list=NULL;
  mvmcell *cell;
  int number=0;
  /* When mvm, key or data is NULL */
  if (m==NULL || key==NULL){
    return NULL;
  }
  /* Otherwise, multisearch all value for key */
  list=(char**)calloc(MAXREPEAT,sizeof(char*));
  cell=m->head;
  while (cell!=NULL){
    if (strcmp(cell->key,key)==0){
      /* If the number of values beyond MAXREPEAT(10), realloc new space */
      if (number>=MAXREPEAT){
        list=(char**)realloc(list,(number+1) * sizeof(char*));
        if (list==NULL){
          ON_ERROR("Realloc New Space for list Failed\n");
        }
      }
      list[number]=cell->data;
      number++;
    }
    cell=cell->next;
  }
  *n=number;
  return list;
}

/* Free & set p to NULL */
void mvm_free(mvm** p)
{
  mvmcell *current,*temp;
  mvm *a=*p;
  current=a->head;
  while (current!=NULL){
    temp=current;
    current=current->next;
    free(temp->key);
    free(temp->data);
    free(temp);
  }
  free(a);
  *p=NULL;
}
