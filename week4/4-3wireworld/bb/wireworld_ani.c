#include "neillncurses.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define GRID_ROW 40
#define GRID_COL 40
#define GENS 1000

#define NDEBUG

char access(char a[GRID_ROW][GRID_COL], int m, int n);
int  h_cnt(char a[GRID_ROW][GRID_COL], int m, int n);
char shift(char a[GRID_ROW][GRID_COL], int m, int n);
void next_generation(char a[GRID_ROW][GRID_COL]);
void test();

int main(int argc, char *argv[])
{
  char a[GRID_ROW][GRID_COL];
  int i, j;

  NCURS_Simplewin sw;

  FILE *ifp;

  test();

  if (argc == 2)
  {
    ifp = fopen(argv[1], "r");
    if (ifp == NULL)
    {
      printf("Cannot open the file.\n");
      return (1);
    }

    for (i = 0; i <= GRID_ROW; i++)
    {
      for (j = 0; j <= GRID_COL; j++)
      {
        fscanf(ifp, "%c", &a[i][j]);
      }
    }
    fclose(ifp);
  } else{
    printf("ERROR: Incorrect usage");
    exit(1);
   }

  Neill_NCURS_Init(&sw);
  /* For the character 't' use red foreground colour, black background,
     and so on */
  Neill_NCURS_CharStyle(&sw, "t", COLOR_RED, COLOR_BLACK, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, "H", COLOR_BLUE, COLOR_BLACK, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, "c", COLOR_YELLOW, COLOR_BLACK, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, " ", COLOR_BLACK, COLOR_BLACK, A_NORMAL);

  do{
     Neill_NCURS_PrintArray(&a[0][0], GRID_ROW, GRID_COL, &sw);
     /*  Delay in 1/1000s of a second */
     Neill_NCURS_Delay(1000.0);
     /* Test for mouse click, or ESC key */
     Neill_NCURS_Events(&sw);

     next_generation(a);

  }while(!sw.finished);

  /* Call this function if we exit() anywhere in the code */
  atexit(Neill_NCURS_Done);

  exit(EXIT_SUCCESS);
  return 0;
  }


  /* m and n locate the position of a certain element */
  char access(char a[GRID_ROW][GRID_COL], int m, int n)
  {
    /* confine the element inside the border */
    if (m < 0) return '\0';
    if (m >= GRID_ROW) return '\0';
    if (n < 0) return '\0';
    if (n >= GRID_COL) return '\0';
    return a[m][n];
  }


  /* h_cnt is a function to count heads around a copper */
  /* m and n locate the position of a certain element */
  int h_cnt(char a[GRID_ROW][GRID_COL], int m, int n)
  {
    int cnt = 0;
    int i, j;
    /* set the element in a 3 by 3 grid to find its neighbors */
    for (i = m - 1; i <= m + 1; i++)
    {
      for (j = n - 1; j <= n + 1; j++)
      {
        access(a, m, n);
        /* neighbors should not include a[m][n] itself */
        if (i != m || j != n)
        {
          if (a[i][j] == 'H')
          cnt++;
        }
      }
    }
   return cnt;
  }


  /* function called to do the transformation of the cells */
  char shift(char a[GRID_ROW][GRID_COL], int i, int j)
  {
    char next;
    switch (a[i][j])
    {
      case ' ' : next = ' '; break;
      case 'H' : next = 't'; break;
      case 't' : next = 'c'; break;
      case 'c' :
      {
        int h_num;
        h_num = h_cnt(a, i, j);
        if (h_num == 1 || h_num == 2)
        {
          next = 'H';
        } else
        {
          next = 'c';
        }
      }; break;
      default : next = ' ';
    }
    return next;
  }


  /* function called to generate the shift between generations */
  void next_generation(char a[GRID_ROW][GRID_COL])
  {
    int  i, j;
    char next[GRID_ROW][GRID_COL];
    char temp;

    for (i = 0; i < GRID_ROW; i++)
    {
      for (j = 0; j < GRID_COL; j++)
      {
        next[i][j] = shift(a, i, j);
      }
    }

    for (i = 0; i < GRID_ROW; i++)
    {
      for (j = 0; j < GRID_COL; j++)
      {
        temp = next[i][j];
        a[i][j] = temp;
      }
    }
  }


  void test()
  {
    char tst[GRID_ROW][GRID_COL] = {{'H', 'H', 'c', 'c'},
                                    {'H', 'c', 'c', 't'},
                                    {'c', 'c', 't', ' '},
                                    {'H', ' ', 'c', 'H'}};

    assert(access(tst, -1, 1) == '\0');
    assert(access(tst, 41, 1) == '\0');
    assert(access(tst, -2, 50) == '\0');

    assert(h_cnt(tst, 1, 1) == 3);
    assert(h_cnt(tst, 1, 2) == 1);
    assert(h_cnt(tst, 2, 1) == 2);

    assert(shift(tst, 0, 0) == 't');
    assert(shift(tst, 0, 3) == 'c');
    assert(shift(tst, 1, 2) == 'H');
    assert(shift(tst, 2, 2) == 'c');
    assert(shift(tst, 2, 3) == ' ');
  }
