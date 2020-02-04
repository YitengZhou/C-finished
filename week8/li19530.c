/*#define NDEBUG*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "neillsdl2.h"

#define FONT_FILE "mode7.fnt"
#define MILLISECONDDELAY 1000
#define CIRCLE_SIZE 125
#define CENTRE_X 400
#define CENTRE_Y 300

typedef struct list {
   int target;
   struct node* head;
} List;

typedef struct node {
   int number;
   struct node* next;
} Node;

Node* allocateNode(void);
void  insertNode(List* list, int number);
void  freeList(List* list);
bool  isPrime(int n);
void  test(void);
void  fillList(List* list, int n);
void  initList(List* list);
void  displayFactor(SDL_Simplewin* sw, Node* node, int x, int y, int r);
void  clearScreen(SDL_Simplewin *sw);
void  displayText(SDL_Simplewin* sw, List* list);
void  prettyPrimes(void);

int main(void)
{
   test();
   prettyPrimes();
   return 0;
}

/*
 * Starts at 2 and increments, calculating the prime factors of each number, and
 * displays in a pretty format using SDL. Should be able to continue almost
 * indefinitely but circles seem to get too small to display after about 125.
 */
void prettyPrimes(void)
{
   List list;
   SDL_Simplewin sw;
   int i = 2;

   Neill_SDL_Init(&sw);

   do {
      SDL_Delay(MILLISECONDDELAY);
      initList(&list);
      fillList(&list, i);
      clearScreen(&sw);
      displayText(&sw, &list);
      displayFactor(&sw, list.head, CENTRE_X, CENTRE_Y, CIRCLE_SIZE);
      Neill_SDL_UpdateScreen(&sw);
      Neill_SDL_Events(&sw);
      freeList(&list);
      i++;
   } while (!sw.finished);

   SDL_Quit();
   atexit(SDL_Quit);
}

/*
 * Renders a large, black rectangle on each iteration to "clear" all the
 * previous white circles
 */
void clearScreen(SDL_Simplewin *sw)
{
   SDL_Rect rectangle;
   rectangle.x = 0;
   rectangle.y = 0;
   rectangle.w = WWIDTH;
   rectangle.h = WHEIGHT;
   Neill_SDL_SetDrawColour(sw, 0, 0, 0);
   SDL_RenderFillRect(sw->renderer, &rectangle);
}

/*
 * Displays text in the upper left corner with the current number and its
 * prime factors
 */
void displayText(SDL_Simplewin* sw, List* list)
{
   char target[100];  /* not dynamically allocated */
   char factors[100]; /* not dynamically allocated */
   char temp[100];    /* not dynamically allocated */
   Node* curr;
   fntrow fontdata[FNTCHARS][FNTHEIGHT];

   sprintf(target, "%d", list->target);

   curr = list->head;
   if (curr->next == NULL) {
      sprintf(factors, "%s", "prime");
   }
   else {
      sprintf(factors, "%s", "");
      while (curr->next != NULL) {
         sprintf(temp, "%d", curr->number);
         strcat(factors, temp);
         strcat(factors, " x ");
         curr = curr->next;
      }
      sprintf(temp, "%d", curr->number);
      strcat(factors, temp);
   }

   Neill_SDL_ReadFont(fontdata, FONT_FILE);
   Neill_SDL_DrawString(sw, fontdata, target, 10, 10);
   Neill_SDL_DrawString(sw, fontdata, factors, 10, 30);
}

/*
 * Displays the prime factors of a number using circles. This function is
 * called recursively until the end of the linked list is reached.
 */
void displayFactor(SDL_Simplewin* sw, Node* node, int x, int y, int r)
{
   int i;
   double angle;
   double n;
   int newx, newy,newr;

   if (node == NULL) {
      Neill_SDL_SetDrawColour(sw, 255, 255, 255);
      Neill_SDL_RenderFillCircle(sw->renderer, x, y, r);
      return;
   }

   n = (double) node->number;
   for (i = 0; i < node->number; i++) {
      newx = newy = newr = 0;
      angle = i * ((2 * M_PI) / n);
      newx = x + r * sin(angle);
      newy = y + r * cos(angle);
      newr = (r / n);
      displayFactor(sw, node->next, newx, newy, newr);
   }
}

/*
 * Initialises the linked list for prime factors by allocating a node at the
 * head, setting the number of this node to -1 and the next pointer to NULL.
 */
void initList(List* list)
{
   list->head = allocateNode();
   list->head->number = -1;
   list->head->next = NULL;
}

/*
 * Finds all the prime factors for a given number, populates them into a
 * linked list.
 */
void fillList(List* list, int n)
{
   int i;

   list->target = n;

   while (n > 1) {
      for (i = n; i > 1; i--) {
         if (isPrime(i) && n % i == 0) {
            insertNode(list, i);
            n /= i;
         }
      }
   }
}

/*
 * Returns true if argument is prime, otherwise false.
 */
bool isPrime(int n)
{
   int i;

   if (n <= 1) {
      return false;
   }

   for (i = 2; i <= sqrt(n); i++) {
      if (n % i == 0) {
         return false;
      }
   }
   return true;
}

/*
 * Dynamically allocates space for a node. Exits program if fails to do so.
 */
Node* allocateNode(void)
{
   Node* p = (Node*) malloc(sizeof(Node));
   if (p == NULL) {
      fprintf(stderr, "Error. Failed to allocate Node.\n");
      exit(EXIT_FAILURE);
   }
   return p;
}

/*
 * Frees memory of linked list.
 */
void freeList(List* list)
{
   Node* curr;
   Node* next;

   curr = list->head;
   while (curr != NULL) {
      next = curr->next;
      free(curr);
      curr = next;
   }
}

/*
 * Inserts a number into a linked list in the correct order (from greatest to
 * smallest).
 */
void insertNode(List* list, int number)
{
   Node* head = list->head;
   Node* curr;
   Node* temp;

   /* If list is empty */
   if (head->number == -1) {
      head->number = number;
      return;
   }

   /* If new node goes at start */
   if (number > head->number) {
      temp = allocateNode();
      temp->number = head->number;
      temp->next = head->next;
      head->next = temp;
      head->number = number;
      return;
   }

   /* If new node is in middle */
   curr = head;
   while (curr->next != NULL) {
      if (number > curr->next->number) {
         temp = allocateNode();
         temp->number = number;
         temp->next = curr->next;
         curr->next = temp;
         return;
      }
      curr = curr->next;
   }

   /* If new node goes at end */
   curr->next = allocateNode();
   curr->next->number = number;
   curr->next->next = NULL;
}

/*
 * Asserts to test functions.
 */
void test(void)
{
   List list;

   /* insertNode() */
   initList(&list);
   insertNode(&list, 2);
   assert(list.head->number == 2);
   insertNode(&list, 5);
   assert(list.head->number == 5);
   assert(list.head->next->number == 2);
   insertNode(&list, 3);
   assert(list.head->number == 5);
   assert(list.head->next->number == 3);
   assert(list.head->next->next->number == 2);
   insertNode(&list, 1);
   assert(list.head->number == 5);
   assert(list.head->next->number == 3);
   assert(list.head->next->next->number == 2);
   assert(list.head->next->next->next->number == 1);
   assert(list.head->next->next->next->next == NULL);
   freeList(&list);

   /* fillList() */
   initList(&list);
   fillList(&list, 9);
   assert(list.head->number == 3);
   assert(list.head->next->number == 3);
   assert(list.head->next->next == NULL);
   freeList(&list);
   initList(&list);
   fillList(&list, 8);
   assert(list.head->number == 2);
   assert(list.head->next->number == 2);
   assert(list.head->next->next->number == 2);
   assert(list.head->next->next->next == NULL);
   freeList(&list);

   /* isPrime() */
   assert(isPrime(0) == false);
   assert(isPrime(1) == false);
   assert(isPrime(2) == true);
   assert(isPrime(3) == true);
   assert(isPrime(4) == false);
   assert(isPrime(5) == true);
   assert(isPrime(8) == false);
}
