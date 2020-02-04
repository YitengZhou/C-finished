
#include <stdio.h>
#include <stdlib.h>

#define UP (-1) 
#define DOWN (1) 
#define LEFT (-1) 
#define RIGHT (1) 

typedef struct {
    int gridsize;
    int requiredLength; 
    int currX; 
    int currY;
    int currPatternLength;
    int printGrids;
} progParams;

void moveFinger(progParams p, int **keypad, int *solutionCount);
int validatePos(progParams p);
progParams validateParams(int argc, char ** argv);
void printgrid(int **keypad, progParams p);

int main(int argc, char** argv){
    progParams params;
    int i, **keypad, solutionCount;

    params = validateParams(argc, argv);

    /* setup mem */
    keypad = (int **) malloc(sizeof(int *) * params.gridsize);
    for(i=0; i< params.gridsize; i++){
        keypad[i] = (int *) calloc(params.gridsize, sizeof(int));
    }

    /* calc and print maze */
    solutionCount = 0;
    moveFinger(params, keypad, &solutionCount);
    printf("%d patterns of len %d \n", solutionCount, params.requiredLength);

    /* clean up */
    for(i=0; i< params.gridsize; i++){
        free(keypad[i]);
    }
    free(keypad);

    return 0;
}


void moveFinger(progParams p, int **keypad, int *solutionCount){
    int x, y;
    x = p.currX;
    y = p.currY;
    if(!validatePos(p)){
        return;
    }
    if(keypad[y][x] != 0){
        return;
    }
    p.currPatternLength +=1;
    /* every time we hit the length, increment count and stop searching */
    if(p.currPatternLength >= p.requiredLength ){
        *solutionCount += 1;
        if(p.printGrids){
            printgrid(keypad, p);        
        }
        return;
    } 

    /* still searching - mark that we've visited this square so we don't revisit */
    keypad[y][x]= p.currPatternLength;

    /* try each direction turn */
    p.currX = x + RIGHT;
    moveFinger(p, keypad, solutionCount);
    p.currX = x;    
    p.currY = y + DOWN;
    moveFinger(p, keypad, solutionCount);
    p.currY = y;    
    p.currX = x + LEFT;
    moveFinger(p, keypad, solutionCount);
    p.currX = x;    
    p.currY = y + UP;
    moveFinger(p, keypad, solutionCount);

    /* if there's nowhere left to go from this square, we're going to have 
        to go back one and try the next direction - remove the marker
        from this square 
    */
    keypad[y][x]= 0;    
}

void printgrid(int **keypad, progParams p){
    int i, j;
    for(i=0; i< p.gridsize; i++){
        for(j=0; j< p.gridsize; j++){
            printf("|%2d|", keypad[j][i]);
        }
        printf("\n");
    }
    printf("\n");    
}

int validatePos(progParams p){
    if(p.currX >= 0 && p.currX < p.gridsize && 
        p.currY >= 0 && p.currY < p.gridsize){
        return 1;
    }
    return 0;
}

progParams validateParams(int argc, char ** argv){
    progParams p;    
    if(argc < 5){
        printf("invalid args. \n Should be: 1. width, 2. requiredLength,"
                "3. startCoordX, 4. startCoordY, (optional 5 - print) \n");
        exit(1);
    } 
    p.gridsize = atoi(argv[1]);
    p.requiredLength = atoi(argv[2]);
    p.currX = atoi(argv[3]);
    p.currY = atoi(argv[4]);
    p.currPatternLength = 0;
    if(argc == 6){
        p.printGrids = atoi(argv[5]);
    } else {
        p.printGrids = 1; 
    }
    if(!(p.gridsize>1 && p.requiredLength <=(p.gridsize * p.gridsize)
        && p.currX >=0 && p.currY >=0 
        && p.currX < p.gridsize 
        && p.currY < p.gridsize)){
        printf("invalid inputs (should say why but I'm lazy)\n");
        printf("%d %d %d %d \n", p.gridsize, p.requiredLength, p.currX, p.currY);
        exit(1);
    }
    return p;
}






