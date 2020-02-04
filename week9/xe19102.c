#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "neillsdl2.h"

#define N 5 /*The integers stored in the tree are 5 characters (or less) in length.*/
#define P 2 /*Printing out position use*/
#define B 1 /*String buffer for "\0"*/

#define LINK_LEFT "    |" /*left node => point down*/
#define LINK_RIGHT "-----"/*right node => point right*/
#define LINK_RIGHT_S "-" /*right node => point right but use short "-"*/
#define NO_LINK "     " /*space*N*/
#define NO_LINK_S " " /*space*/
#define ORIGINAL_ARRAY "00000" /*ZERO*N*/

#define  R_SIZE_W 120
#define  R_SIZE_H 120
#define  GAP 20
#define  RADIUS 55
#define  MILLISECONDDELAY 500
#define  START 100
#define  POSITION_X(A) START+(A/2)*(R_SIZE_W+GAP)+(A%2)*R_SIZE_W
#define  POSITION_Y(A) START+(A/2)*(R_SIZE_H+GAP)+(A%2)*R_SIZE_H
#define  ARRAY_SIZE(B) B*P+1

#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

struct node {
    char *c;
    struct node *parent;
    struct node *left;
    struct node *right;
    int x;
    int y;
};
typedef struct node Node;

Node* generate_node(char* str, int length) ;
char* read_element(char* str, int *cursor);
Node* new_node (Node* new, char* c, Node* l, Node* r, Node* p) ;
Node* fill_node_xy (Node* new, int x, int y);
void  numbering (Node* n, int* x, int y, int* max_y);
char*** create_array (char*** f, int x, int y);
void  fill_array (Node *n, char*** f);
void  print_array (char*** f, int x, int y);
void  free_node(Node *n);
void  free_array(char*** f, int x, int y);
int   zero_count (char* s);
char* delete_zero(char* s, int count);
void  delete_array_zero (char*** f, int x, int y);
void  test(void);

void print_sdl_left (int x, int y, SDL_Simplewin sw, SDL_Rect rectangle);
void print_sdl_right (int x, int y, SDL_Simplewin sw, SDL_Rect rectangle);
void print_sdl_right_s (int x, int y, SDL_Simplewin sw, SDL_Rect rectangle);
void print_sdl_circle (int x, int y, SDL_Simplewin sw, int r) ;
void print_sdl_string(char* str, SDL_Simplewin sw, fntrow font[FNTCHARS][FNTHEIGHT], int x, int y);

void print_sdl_node (char*** f, SDL_Simplewin sw, SDL_Rect rectangle, fntrow font[FNTCHARS][FNTHEIGHT], int i, int j);
void print_sdl_answer (char*** f, SDL_Simplewin sw, SDL_Rect rectangle, fntrow font[FNTCHARS][FNTHEIGHT], int w, int h);
int main(int argc, char* argv[]) {

    Node *h;
    int x=0, y=0;
    char*** final_array = NULL;

    SDL_Simplewin sw;
    SDL_Rect rectangle;
    fntrow font[FNTCHARS][FNTHEIGHT];

    test();

    if (argc == 2){

        /*transfer string to linked list*/
        h = generate_node(argv[1], (unsigned)strlen(argv[1]));
        if (h != NULL){
            /*calculate left and right depth of the link*/
            /*x equal right depth => accumulation*/
            /*y equal lieft depth*/
            numbering(h, &x, y, &y);

            /*create a 3D array to print the result*/
            final_array = create_array(final_array, ARRAY_SIZE(x), ARRAY_SIZE(y));
            fill_array(h, final_array);

            delete_array_zero(final_array, ARRAY_SIZE(x), ARRAY_SIZE(y));
            /*Code next line is for plain txt printing if you need.*/
            /*print_array(final_array, ARRAY_SIZE(x), ARRAY_SIZE(y));*/

            /*SDL printing*/
            Neill_SDL_Init(&sw);
            Neill_SDL_ReadFont(font, "mode7.fnt");
            print_sdl_answer(final_array, sw, rectangle, font, ARRAY_SIZE(x), ARRAY_SIZE(y));

            SDL_Quit();
            atexit(SDL_Quit);

            /*free all memory*/
            free_array(final_array, ARRAY_SIZE(x), ARRAY_SIZE(y));
            free_node(h);
        }
        else {
            /*simple usage error check*/
            ON_ERROR("Usage Error.\n");
        }
    }
    else {
        ON_ERROR("Usage Error.\n");
    }
    return 0;
}


Node* generate_node(char* str, int length) {

    int i=0;
    Node *n = NULL, *head;
    n = new_node(n, read_element(str, &i), NULL, NULL, NULL);
    head = n;

    for ( ; i<length; i++) {
        if (str[i] == '(') {
            if (n->left==NULL) {
                n->left = new_node(n->left, NULL, NULL, NULL, n);
                n = n->left;
            }
            else {
                n->right = new_node(n->right, NULL, NULL, NULL, n);
                n = n->right;
            }
        }
        else if (str[i] == '*') {
        }
        else if (str[i] == ')') {
            n = n->parent;
        }
        else if (str[i] >= '0' && str[i] <= '9') {
            n->c=read_element(str, &i);
            i--;
        }
        else {
            head = NULL;
            return head;
        }
    }

    return head;

}

Node* new_node (Node* new, char* c, Node* l, Node* r, Node* p) {
    new = (Node*)calloc(1, sizeof(Node));
    new->c=c;
    new->left = l;
    new->right = r;
    new->parent = p;
    return new;
}

/*Read element between ()*/
char* read_element(char* str, int *cursor) {

    int i, j;
    char* temp;
    temp = (char*)calloc(N, sizeof(char));
    strncpy(temp, ORIGINAL_ARRAY, N);

    for (i=0; str[*cursor+i] != '(' && str[*cursor+i] != ')' && str[*cursor+i] != '*'; i++) {}
    for (j=0; j<i; j++){
        temp[N-i+j] = str[*cursor];
        (*cursor)++;
    }

    return temp;
}

/*Calculate left/right depth of every nodes*/
void numbering (Node* n, int* x, int y, int* max_y) {

    n = fill_node_xy(n, *x, y);
    if (y>(*max_y)) {*max_y=y;}

    if (n->left != NULL && n->left->c != NULL) {
        numbering(n->left, x, ++y, max_y);
        --y;
    }
    if (n->right != NULL && n->right->c != NULL) {
        (*x)++;
        numbering(n->right, x, y, max_y);
    }

}

Node* fill_node_xy (Node* new, int x, int y) {
    new->x = x;
    new->y = y;
    return new;
}


/*create a 3-D array for printing*/
char*** create_array (char*** f, int x, int y) {
    int i, j;
    f = (char***)calloc(y, sizeof(char**));
    for (j=0; j<y; j++) {
        f[j] = (char**)calloc(x, sizeof(char*));
    }
    for (j=0; j<y; j++) {
        for (i=0; i<x; i++){
            if (i%2 == 0){
                f[j][i] = (char*)calloc(N+B, sizeof(char));
                strncpy(f[j][i], NO_LINK, N);
            }
            else {
                f[j][i] = (char*)calloc(1+B, sizeof(char));
                strncpy(f[j][i], NO_LINK_S, 1);
            }
        }
    }
    return f;
}

void fill_array (Node *n, char*** f) {

    int i;

    if (n->c!=NULL){
        strncpy(f[(n->y)*P][(n->x)*P], n->c, N);
    }

    if (n->left != NULL && n->left->c != NULL) {
        strncpy(f[(n->y)*P+1][(n->x)*P], LINK_LEFT, N);
        fill_array (n->left, f);
    }
    if (n->right != NULL &&  n->right->c != NULL) {
        for (i=0; i<((n->right->x)*P-(n->x)*P)-1; i++){
            if (((n->x)*P+i+1)%2 == 0){
                strncpy(f[(n->y)*P][(n->x)*P+i+1], LINK_RIGHT, N);
            }
            else {
                strncpy(f[(n->y)*P][(n->x)*P+i+1], LINK_RIGHT_S, 1);
            }
        }
        fill_array (n->right, f);
    }

}

 /*count every elements front zero count*/
int zero_count (char* s){
    int i, j=0;
    if (s[0]>'9' || s[0] <'0') {
        return N;
    }
    for (i=0; i<N-1; i++){ /*No need to check the final one*/
        if (s[i]=='0'){
            j++;
        }
        else {
            return j;
        }
    }
    return j;
}

char* delete_zero(char* s, int count){
    char* str = (char*)calloc(N-count+B, sizeof(char));
    memcpy(str, s+count, N-count+B);
    return str;
}

/*Delete the unnecessary zero*/
void delete_array_zero (char*** f, int x, int y){
    int i, j, min_zerocnt;
    char* tmp;
    int zero_cnt;
    for (i=0; i<x; i=i+P){
        min_zerocnt=N;
        for (j=0; j<y; j=j+P){
            zero_cnt = zero_count(f[j][i]);
            if(zero_cnt<min_zerocnt){
                min_zerocnt = zero_count(f[j][i]);
            }
        }
        if (min_zerocnt!=N){
            for (j=0; j<y; j++){
                tmp = f[j][i];
                f[j][i] = delete_zero(f[j][i], min_zerocnt);
                free(tmp);
            }
        }
    }
}

void print_array (char*** f, int x, int y){
    int i, j;
    for (j=0; j<y; j++) {
        for (i=0; i<x; i++) {
            fprintf(stdout, "%s", f[j][i]);
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

void free_array(char*** f, int x, int y) {
    int i, j;
    for (j=0; j<y; j++) {
        for (i=0; i<x; i++){
            free(f[j][i]);
        }
    }
    for (j=0; j<y; j++) {
        free(f[j]);
    }
    free(f);
}

void free_node(Node *n) {
    if (n->left != NULL) {free_node(n->left);}
    if (n->right != NULL) {free_node(n->right);}
    free(n->c);
    free(n);
    n = NULL;
}

void test(void) {
    assert(zero_count("00583")==2);
    assert(zero_count("00000")==4);
    assert(zero_count("00006")==4);
    assert(zero_count("     ")==N);
    assert(zero_count("-----")==N);
    assert(zero_count("12345")==0);
}


void print_sdl_answer (char*** f, SDL_Simplewin sw, SDL_Rect rectangle, fntrow font[FNTCHARS][FNTHEIGHT], int w, int h){

    int i, j;
    for (j=0; j<h; j=j+2){
        for (i=0; i<w && !sw.finished; i++){
            if (j-1>0) {print_sdl_node(f, sw, rectangle, font, i, j-1);}
            {print_sdl_node(f, sw, rectangle, font, i, j);}
            Neill_SDL_Events(&sw);
        }

    }
    Neill_SDL_UpdateScreen(&sw);
    SDL_Delay(MILLISECONDDELAY);

}

void print_sdl_node (char*** f, SDL_Simplewin sw, SDL_Rect rectangle, fntrow font[FNTCHARS][FNTHEIGHT], int i, int j) {
    if (f[j][i][0]<='9' && f[j][i][0]>='0'){
        print_sdl_string(f[j][i], sw, font, POSITION_X(i), POSITION_Y(j));
        Neill_SDL_UpdateScreen(&sw);
        SDL_Delay(MILLISECONDDELAY);
    }
    if (f[j][i][0] =='-') {
        if (i%2==0){
            print_sdl_right(POSITION_X(i), POSITION_Y(j), sw, rectangle);
        }
        else {
            print_sdl_right_s(POSITION_X(i), POSITION_Y(j), sw, rectangle);
            Neill_SDL_UpdateScreen(&sw);
            SDL_Delay(MILLISECONDDELAY/2);
        }
    }
    if (f[j][i][(strlen(f[j][i])-1)] == '|') {
        print_sdl_left(POSITION_X(i), POSITION_Y(j), sw, rectangle);
        Neill_SDL_UpdateScreen(&sw);
        SDL_Delay(MILLISECONDDELAY/2);
    }


}

void print_sdl_string(char* str, SDL_Simplewin sw, fntrow font[FNTCHARS][FNTHEIGHT], int x, int y){
    int i, zero_cnt = zero_count(str);

    print_sdl_circle (x+(R_SIZE_W/2), y+(R_SIZE_H/2), sw, RADIUS);
    i = (unsigned)strlen(str)-zero_cnt;

    switch (i) {
        case 5 : Neill_SDL_DrawString(&sw, font, str+zero_cnt, x+5, y+50); break;
        case 4 : Neill_SDL_DrawString(&sw, font, str+zero_cnt, x+13, y+50); break;
        case 3 : Neill_SDL_DrawString(&sw, font, str+zero_cnt, x+20, y+50); break;
        case 2 : Neill_SDL_DrawString(&sw, font, str+zero_cnt, x+27, y+50); break;
        case 1 : Neill_SDL_DrawString(&sw, font, str+zero_cnt, x+35, y+50); break;
    }

}

void print_sdl_right_s (int x, int y, SDL_Simplewin sw, SDL_Rect rectangle) {

    rectangle.w = GAP;
    rectangle.h = 2;

    Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
    rectangle.x = x;
    rectangle.y = y+(R_SIZE_H)/2;
    SDL_RenderFillRect(sw.renderer, &rectangle);

}

void print_sdl_right (int x, int y, SDL_Simplewin sw, SDL_Rect rectangle) {

    rectangle.w = R_SIZE_W;
    rectangle.h = 2;

    Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
    rectangle.x = x;
    rectangle.y = y+(R_SIZE_H)/2;
    SDL_RenderFillRect(sw.renderer, &rectangle);

}

void print_sdl_left (int x, int y, SDL_Simplewin sw, SDL_Rect rectangle) {

    rectangle.w = 2;
    rectangle.h = GAP;

    Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
    rectangle.x = x+R_SIZE_W/2;
    rectangle.y = y;
    SDL_RenderFillRect(sw.renderer, &rectangle);

}

void print_sdl_circle (int x, int y, SDL_Simplewin sw, int r) {

    Neill_SDL_SetDrawColour(&sw, 255, 204, 204);
    Neill_SDL_RenderFillCircle(sw.renderer, x, y, r);
    Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
    Neill_SDL_RenderFillCircle(sw.renderer, x, y, r-3);

}
