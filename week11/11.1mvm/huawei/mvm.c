#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mvm.h"
#define DATALEN 50
#define STRLEN 1000
#define RESLEN 10

#define CHECK_MEM(VAR) if (VAR == NULL){ ON_ERROR("Error: memory allocate failed.\n"); }

mvm* mvm_init(void)
{
  mvm* init = NULL;

  init = (mvm*) calloc(1, sizeof(mvm));
  CHECK_MEM(init);
  init->head = NULL;
  init->numkeys = 0;
  return init;
}

int mvm_size(mvm* m)
{
  if (m == NULL)
  {
    return 0;
  }

  return m->numkeys;
}

void mvm_insert(mvm* m, char* key, char* data)
{
  /* int lenKey, lenData; */
  mvmcell* newCell = NULL;

  if (m == NULL || key == NULL || data == NULL)
  {
    return;
  }
  /*
  lenKey = strlen(key) + 1;
  lenData = strlen(data) + 1;
  newCell->key = (char*) calloc(lenKey, sizeof(char));
  newCell->data = (char*) calloc(lenData, sizeof(char));
  */
  newCell = (mvmcell*) calloc(1, sizeof(mvmcell));
  CHECK_MEM(newCell);
  newCell->key = (char*) calloc(DATALEN, sizeof(char));
  CHECK_MEM(newCell->key);
  newCell->data = (char*) calloc(DATALEN, sizeof(char));
  CHECK_MEM(newCell->data);
  strcpy(newCell->key, key);
  strcpy(newCell->data, data);
  newCell->next = m->head;
  m->head = newCell;
  m->numkeys++;
}

char* mvm_print(mvm* m)
{
  mvmcell* cur;
  char* sout;
  int length = 0;

  if (m == NULL)
  {
    ON_ERROR("Error: call mvm_print on null pointer.\n");
  }
  cur = m->head;
  sout = (char*) calloc(STRLEN, sizeof(char));
  CHECK_MEM(sout);
  while (cur != NULL)
  {
    length += sprintf(sout + length, "[%s](%s) ", cur->key, cur->data);
    cur = cur->next;
  }
  return sout;
}

void mvm_delete(mvm* m, char* key)
{
  mvmcell* cur, *del;

  if (m == NULL || key == NULL)
  {
    return;
  }
  if (m->head == NULL)
  {
    return;
  }
  if (strcmp(m->head->key, key) == 0)
  {
    del = m->head;
    m->head = m->head->next;
    m->numkeys--;
    free(del->key);
    free(del->data);
    free(del);
    return;
  }
  cur = m->head;
  while (cur->next != NULL)
  {
    if (strcmp(cur->next->key, key) == 0)
    {
      del = cur->next;
      cur->next = cur->next->next;
      m->numkeys--;
      free(del->key);
      free(del->data);
      free(del);
      return;
    }
    cur = cur->next;
  }
}

char* mvm_search(mvm* m, char* key)
{
  mvmcell* cur = NULL;

  if (m == NULL || key == NULL)
  {
    ON_ERROR("Error: call mvm_search on null pointer.\n");
  }
  cur = m->head;
  while (cur != NULL)
  {
    if (strcmp(cur->key, key) == 0)
    {
      return cur->data;
    }
    cur = cur->next;
  }
  return NULL;
}

char** mvm_multisearch(mvm* m, char* key, int* n)
{
  mvmcell* cur = NULL;
  char** result = NULL;

  if (m == NULL || key == NULL)
  {
    ON_ERROR("Error: call mvm_multisearch on null pointer.\n");
  }
  result = (char**) calloc(RESLEN, sizeof(char*));
  CHECK_MEM(result);
  cur = m->head;
  *n = 0;
  while (cur != NULL)
  {
    if (strcmp(cur->key, key) == 0)
    {
      if (*n >= RESLEN)
      {
        result = (char**) realloc(result, *n * sizeof(char*));
        CHECK_MEM(result);
      }
      result[*n] = cur->data;
      *n += 1;
    }
    cur = cur->next;
  }
  return result;
}

void mvm_free(mvm** p)
{
  mvm* tp;
  mvmcell* mc, *del;
  if (p == NULL)
  {
    return;
  }
  tp = *p;
  mc = tp->head;
  while (mc != NULL)
  {
    del = mc;
    mc = mc->next;
    free(del->key);
    free(del->data);
    free(del);
  }
  free(tp);
  *p = NULL;
}
