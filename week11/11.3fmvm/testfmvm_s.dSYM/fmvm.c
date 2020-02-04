#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fmvm.h"

#define HASHKEY1 300007
#define HASHKEY2 150011
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
  init->block=(mvmcell*)calloc(HASHKEY1,sizeof(mvmcell));
  init->hashsize=HASHKEY1;
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
  int i=1,flag=0,key1,key2,value;
  /* When mvm, key or data is NULL */
  if (m==NULL || key==NULL || data==NULL){
    return;
  }
  key1=GetKey1(key);
  if (m->block[key1].index==0){
    m->block[key1].key=(char *)calloc(strlen(key)+1,sizeof(char));
    m->block[key1].data=(char *)calloc(strlen(data)+1,sizeof(char));
    strcpy(m->block[key1].key,key);
    strcpy(m->block[key1].data,data);
    m->block[key1].index=GetKey(key);
    m->numkeys++;
    return;
  }
  else{
    key2=GetKey2(key);
    while (flag==0){
      if (strcmp(m->block[key1].key,key)==0){
        InsertSame(&m->block[key1],key,data);
        m->numkeys++;
        flag=1;
        return;
      }
      else{
        i++;
        value=(key1+i*key2)%m->hashsize;
        if (m->block[value].index==0){
          m->block[value].key=(char *)calloc(strlen(key)+1,sizeof(char));
          m->block[value].data=(char *)calloc(strlen(data)+1,sizeof(char));
          strcpy(m->block[value].key,key);
          strcpy(m->block[value].data,data);
          m->block[value].index=GetKey(key);
          m->numkeys++;
          flag=1;
          return;
        }
      }
    }
  }
}

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m)
{
  mvmcell *current=NULL;
  char *str,*temp;
  int i=0;
  str=calloc(STRSIZE,sizeof(char));
  temp=calloc(PRINTSIZE,sizeof(char));
  if (str==NULL||temp==NULL){
    ON_ERROR("Printing of MultiValue Maps Failed\n");
  }
  for (i=0;i<m->hashsize;i++){
    if (m->block[i].index!=0){
      sprintf(temp,"[%s](%s) ",m->block[i].key,m->block[i].data);
      strcat(str,temp);
      current=m->block[i].same;
      while (current!=NULL){
        sprintf(temp,"[%s](%s) ",current->key,current->data);
        strcat(str,temp);
        current=current->same;
      }
    }
  }
  free(temp);
  return str;
}


/* Remove one key/value */
void mvm_delete(mvm* m, char* key)
{
  mvmcell *cell,*temp;
  int key1,flag=0,i=0;
  /* When mvm, key or data is NULL */
  if (m==NULL || key==NULL || key[0]=='\0'){
    return ;
  }
  /* Otherwise, delete this key/value cell */
  key1=GetKey1(key);
  while (flag==0){
    if (m->block[key1].index==0){
      return;
    }
    else{
      if (strcmp(key,m->block[key1].key)==0){
        /*printf("%s->%d\n",key,key1);
        printf("%s(%d)->key:%s data:%s index:%d\n",key,key1,\
        m->block[key1].key,m->block[key1].data,m->block[key1].index);
        printf("!!!!!\n");*/
        if (m->block[key1].same==NULL){
          mvmcell_free(&m->block[key1]);
        }
        else{
          cell=&m->block[key1];
          temp=cell->same;
          while (temp->same!=NULL){
            temp=temp->same;
            cell=cell->same;
          }
          mvmcell_free(temp);
          free(cell->same);
          cell->same=NULL;
        }
        m->numkeys--;
        flag=1;
        return ;
      }
      else{
        i++;
        key1=(GetKey1(key)+i*GetKey2(key))%m->hashsize;
      }
    }
  }
  return;
}

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key)
{
  int key1,flag=0,i=0;
  /* When mvm, key or data is NULL */
  if (m==NULL || key==NULL){
    return NULL;
  }
  /* Otherwise, search value for key */
  key1=GetKey1(key);
  while (flag==0){
    if (m->block[key1].index==0){
      return NULL;
    }
    else{
      if (strcmp(key,m->block[key1].key)==0){
        flag=1;
        return m->block[key1].data;
      }
      else{
        i++;
        key1=(GetKey1(key)+i*GetKey2(key))%m->hashsize;
      }
    }
  }
  return NULL;
}

/* Return *argv[] list of pointers to all values stored with key,
   n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n)
{
  char **list=NULL;
  mvmcell *cell;
  int number=0,key1,flag=0,i=0;
  /* When mvm, key or data is NULL */
  if (m==NULL || key==NULL){
    return NULL;
  }
  /* Otherwise, multisearch all value for key */
  list=(char**)calloc(MAXREPEAT,sizeof(char*));
  key1=GetKey1(key);
  while (flag==0){
    if (m->block[key1].index==0){
      return NULL;
    }
    else{
      cell=&m->block[key1];
      if (strcmp(key,m->block[key1].key)==0){
        while (cell!=NULL){
          if (strcmp(key,m->block[key1].key)==0){
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
          cell=cell->same;
        }
        *n=number;
        return list;
      }
      else{
        i++;
        key1=(GetKey1(key)+i*GetKey2(key))%m->hashsize;
      }
    }
  }
  return NULL;
}

/* Free & set p to NULL */
void mvm_free(mvm** p)
{
  mvm *a=*p;
  mvmcell *cur,*temp;
  int i;
  for (i=0;i<a->hashsize;i++){
    if (a->block[i].index!=0){
      free(a->block[i].key);
      free(a->block[i].data);
    }
    if (a->block[i].same!=0){
      cur=a->block[i].same;
      while (cur!=NULL){
        temp=cur;
        cur=cur->same;
        free(temp->key);
        free(temp->data);
        free(temp);
      }
    }
  }
  free(a->block);
  free(a);
  *p=NULL;
}

/* 'A'-'a'=-32, ' '-'a'=-65, '1'-'a'=-48 */
int GetKey(char *str)
{
  int i,length=strlen(str);
  double sum=0;
  if (length>5){
    length=5;
  }
  for (i=0;i<length;i++){
    sum+=(double)abs((str[i]-'a'))*pow(26.0,length-i-1);
  }
  return (int)sum;
}

int GetKey1(char *str)
{
  return GetKey(str)%HASHKEY1;
}

int GetKey2(char *str)
{
  return HASHKEY2-GetKey(str)%HASHKEY2;
}

void InsertSame(mvmcell* cell, char* key, char* data)
{
  mvmcell* temp;
  temp=cell;
  while (temp->same!=NULL){
    temp=temp->same;
  }
  temp->same=(mvmcell*)calloc(1,sizeof(mvmcell));
  temp->same->key=(char *)calloc(strlen(key)+1,sizeof(char));
  temp->same->data=(char *)calloc(strlen(data)+1,sizeof(char));
  strcpy(temp->same->key,key);
  strcpy(temp->same->data,data);
  temp->same->index=GetKey(key);
  return;
}

void mvmcell_free(mvmcell *cell)
{
  free(cell->key);
  cell->key=NULL;
  free(cell->data);
  cell->data=NULL;
  cell->index=0;
}

void PrintHashtable(mvm* m)
{
  mvmcell *current=NULL;
  int i=0;
  for (i=0;i<m->hashsize;i++){
    if (m->block[i].index!=0){
      printf("\nblock[%d] : ",i);
      printf("[%s](%s){%d} ",m->block[i].key,m->block[i].data,m->block[i].index);
      current=m->block[i].same;
      while (current!=NULL){
        printf("-> [%s](%s){%d} ",current->key,current->data,m->block[i].index);
        current=current->same;
      }
    }
  }
}
