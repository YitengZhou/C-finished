/*
    ZQ Fan
    cc19055@bristol.ac.uk
    Prime Factors : use SDL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "neillsdl2.h"
#include <assert.h>

#define FIRSTPRIME  2
#define SECONDPRIME 3

#define MAXLEN 128

#define MILLISECONDDELAY 1000
#define MAX8BITVALUR 255
#define MAXHUE 360
#define HUEDIVIDE 60
#define HUEBLUE 240
#define SIZESCALE 0.9

#define MIN(A, B) (A < B) ? A : B

typedef enum bool{false, true} bool;

typedef struct primeBody
{
    long int prime;
    int times;
    struct primeBody *next;
}Prime;

typedef struct
{
    Prime *first;
    Prime *last;
}List;

typedef struct pattern
{
    int size, cx, cy, color, colrange;
    double theta;
    long int n;
} pattern;

typedef struct rgb
{
    int r, g, b;
} rgb;

List *find_prime_factor(long int num);
bool in_primes(long int num, List *primes);
void convert_to_string(char result[MAXLEN], long int num, List *primes);
long int *convert_list_to_array(List *primes, int *size);

void sdl_primes(List *primes);
void draw_circle_pattern_primes(SDL_Simplewin *sw , long int *factors,
                                int index, pattern stage);
void hsv2rgb(rgb *color, int h, float s, float v);

List *init_primes(void);
void push_list(List *primes, long int num, int occur);
void delete_list_time(List *head, long int num);
void print_list(List *head);
void print_pattern(pattern stage, int index);

void test(void);
void test_find_prime_factor(void);
void test_list_func(void);
void test_hsv2rgb(void);

int main(int argc, char **argv)
{
    List *primes;
    long int num;
    char result[MAXLEN];

    if (argc != 2)
    {
        fprintf(stderr, "Usage(e.g.): %s 123\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    num = atol(argv[1]);
    if (num <= 0)
    {
        fprintf(stderr, "%s\n", "Input argument is not a positve integer");
        return 1;
    }
    /* primes: Lsit of all prime factorials */
    primes = find_prime_factor(num);
    delete_list_time(primes, 0);

    /* print in plain text */
    convert_to_string(result, num, primes);
    printf("%s\n", result);

    /* draw by sdl2 */
    sdl_primes(primes);

    free(primes);
    return 0;
}


List *find_prime_factor(long int num)
{
    List *primes;
    long int i;
    int cnt;

    primes = init_primes();

    if (num == 1)
    {
        push_list(primes, num, 1);
        return primes;
    }

    while (num % FIRSTPRIME == 0)
    {
        (primes->first->times)++;
        num /= FIRSTPRIME;
    }

    for (i = SECONDPRIME; i <= (long int)sqrt((double)num); i += 2)
    {
        if (in_primes(i, primes))
        {   cnt = 0;
            while(num % i == 0)
            {
                cnt++;
                num /= i;
            }
            push_list(primes, i, cnt);
        }
    }

    if (num != 1)
    {
        push_list(primes, num, 1);
    }
    return primes;
}

bool in_primes(long int num, List *primes)
{
    Prime *temp;
    if (!primes)
    {
        fprintf(stderr, "%s\n", "in_primes : List *primes == NULL");
        return false;
    }

    temp = primes->first;
    while (temp)
    {
        if (num % temp->prime == 0)
        {
            return false;
        }
        temp = temp->next;
    }
    return true;
}

long int *convert_list_to_array(List *primes, int *size)
{
    int len, occur;
    Prime *node;
    long int *array = NULL;
    if (!primes)
    {
        fprintf(stderr, "%s\n", "convert_list_to_array : List *primes = NULL");
    }

    len = 0;
    node = primes->first;
    while (node)
    {
        if (node->prime == FIRSTPRIME)
        {
            len += node->times / 2 + node->times % 2;
        }
        else
        {
            len += node->times;
        }
        node = node->next;
    }

    if (len == 0)
    {
        fprintf(stderr, "%s\n",
                "convert_list_to_array : List *primes is empty");
        return array;
    }
    *size = len;

    if (!(array = (long int *)calloc(len, sizeof(long int))))
    {
        fprintf(stderr, "%s\n", "convert_list_to_array : fail to allocate");
        return array;
    }

    len = 0;
    node = primes->first;
    while (node)
    {
        occur = node->times;
        if (node->prime == FIRSTPRIME)
        {
            while(occur > 1)
            {
                array[len++] = 2 * FIRSTPRIME;
                occur -= 2;
            }
            if (occur == 1)
            {
                array[len++] = FIRSTPRIME;
                occur--;
            }
        }
        else
        {
            while(occur > 0)
            {
                array[len] = node->prime;
                len++;
                occur--;
            }
        }
        node = node->next;
    }

    return array;
}

void convert_to_string(char result[MAXLEN], long int num, List *primes)
{
    char s[MAXLEN];
    int sum, len;
    /* Since primes is a printer, must use a new var to copy */
    Prime *temp;
    if (!primes)
    {
        fprintf(stderr, "%s\n", "print_list : List *primes = NULL");
    }
    else if (!primes->first || !primes->last)
    {
        fprintf(stderr, "%s\n", "print_list : list is empty");
    }
    else
    {
        temp = primes->first;
        sprintf(result, "%ld = 1", num);
        sum = strlen(result);
        while (temp)
        {
            sprintf(s, " x %ld^%d", temp->prime, temp->times);
            len = strlen(s);
            if (sum + len< MAXLEN-1)
            {
                strcat(result, s);
                sum += len;
                temp = temp->next;
            }
            else
            {
                fprintf(stderr, "%s %d\n",
                    "The result is longer than ", MAXLEN);
                temp = NULL;
            }
        }
    }
}

List *init_primes(void)
{
    List *primes;

    if (!(primes = (List *)malloc(sizeof(List))))
    {
        fprintf(stderr, "%s\n",
         "fail to allocate memory for list of primes");
        exit(EXIT_FAILURE);
    }
    push_list(primes, FIRSTPRIME, 0);
    return primes;
}


void push_list(List *list, long int num, int occur)
{
    Prime *node;
    if (!list)
    {
        fprintf(stderr, "%s\n", "push_list : List is NULL");
    }
    else
    {
        if (!(node = (Prime *)malloc(sizeof(Prime))))
        {
            fprintf(stderr, "%s\n", "fail to allocate memory for node");
            free(list);
            exit(EXIT_FAILURE);
        }
        node->prime = num;
        node->times = occur;
        node->next = NULL;
        if (!list->first || !list->last)
        {
            list->first = list->last = node;
        }
        else
        {
            list->last->next = node;
            list->last = node;
        }
    }
}

void delete_list_time(List *head, long int num)
{
    Prime *node, *pre;
    if (!head)
    {
        fprintf(stderr, "%s\n", "delete_list_time : List *head = NULL");
    }
    else
    {
        while (head->first->times == num)
        {
            node = head->first;
            head->first = head->first->next;
            free(node);
        }
        pre = head->first;
        node = pre->next;
        while (node)
        {
            if (node->times == num)
            {
                pre->next = node->next;
                free(node);
            }
            else
            {
                pre = node;
            }
            node = pre->next;
        }
    }
}

void reverse_list(List *head)
{
    Prime *node , *temp;
    if (!head)
    {
        fprintf(stderr, "%s\n", "reverse_list : List *head = NULL");
    }
    if (head->first)
    {
        head->last = head->first;
        node = head->first->next;
        head->first->next = NULL;
        while (node)
        {
            temp = node->next;
            node->next = head->first;
            head->first = node;
            node = temp;
        }
    }
}

void print_list(List *head)
{
    /* Since head is a printer, must use a new var to copy */
    Prime *temp;
    if (!head)
    {
        fprintf(stderr, "%s\n", "print_list : List *head = NULL");
    }
    else if (!head->first || !head->last)
    {
        fprintf(stderr, "%s\n", "print_list : list is empty");
    }
    else
    {
        temp = head->first;
        while (temp)
        {
            printf("Prime : %ld, Time : %d\n", temp->prime, temp->times);
            temp = temp->next;
        }
        printf("\n");
    }
}

void sdl_primes(List *primes)
{
    SDL_Simplewin sw;
    pattern stage;
    long int *factors;
    int len;

    factors = convert_list_to_array(primes, &len);

    Neill_SDL_Init(&sw);
    /* Clear screen, & set draw colour to white */
    SDL_SetRenderDrawColor(sw.renderer,
                           MAX8BITVALUR,
                           MAX8BITVALUR,
                           MAX8BITVALUR,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sw.renderer);

    stage.size     = MIN(WWIDTH, WHEIGHT);
    stage.cx       = WWIDTH/2;
    stage.cy       = WHEIGHT/2;
    stage.color    = HUEBLUE;
    stage.colrange = MAXHUE;
    stage.theta    = 0.;

    draw_circle_pattern_primes(&sw, factors, len-1, stage);

    Neill_SDL_UpdateScreen(&sw);

    do
    {
        /* Has anyone pressed ESC or killed the SDL window ?
           Must be called frequently - it's the only way of escaping  */
        Neill_SDL_Events(&sw);
    }while (!sw.finished);
    /*SDL_Delay(MILLISECONDDELAY);*/
    /* Clear up graphics subsystems */
    SDL_Quit();
    atexit(SDL_Quit);

    free(factors);
}

void draw_circle_pattern_primes(SDL_Simplewin *sw , long int *factors,
                                int index, pattern stage)
{
    long int i, n;
    pattern nextStage;
    double delta;
    rgb rgbColor;

    rgbColor.r = 0;
    rgbColor.g = 0;
    rgbColor.b = 0;
    
    if (index < 0)
    {
        if (stage.size < 1)
        {
            fprintf(stderr, "%s\n",
                    "draw_circle_pattern_primes : exceed minmum pixel");
            free(factors);
            exit(EXIT_FAILURE);
        }
        else
        {
            /* stage.color is hue of hsv
               set s, v = 1.0 */
            hsv2rgb(&rgbColor, stage.color, 1., 1.);
            Neill_SDL_SetDrawColour(sw, rgbColor.r, rgbColor.g, rgbColor.b);
            Neill_SDL_RenderFillCircle(sw->renderer,
                                       stage.cx, stage.cy, stage.size/4);
        }
    }
    else
    {
        n = factors[index];
        if (n == 1)
        {
            draw_circle_pattern_primes(sw, factors, index-1, stage);
        }
        else
        {
            /* calculate next image's info */
            /* n==2, n==4 is special, need to incline */
            if(n == 4)
            {
                stage.theta += M_PI/4;
            }
            else if (n != FIRSTPRIME)
            {
                stage.theta = 0.;
            }

            delta = 2 * M_PI / (double)n;
            nextStage.size =
            (int)(sin(delta/2.)/(1.+sin(delta/2.))*(double)stage.size*SIZESCALE);
            nextStage.colrange = stage.colrange/n;
            for (i = 0; i < n; i++)
            {
                nextStage.cx = stage.cx - (int)((double)stage.size/2./(1.+sin(delta/2.))
                    *sin((double)i*delta+ stage.theta));
                nextStage.cy = stage.cy - (int)((double)stage.size/2./(1.+sin(delta/2.))
                    *cos((double)i*delta+ stage.theta));

                nextStage.color = stage.color + i * nextStage.colrange;
                nextStage.theta = i * delta + stage.theta;
                draw_circle_pattern_primes(sw, factors, index-1, nextStage);
            }
        }
    }
}

void hsv2rgb(rgb *color, int h, float s, float v)
{
	/* R,G,B from 0-255, h from 0-inf, s,v from 0-1.0 */
	int i, difs;
	int rgb_min, rgb_max, rgb_adj;

	rgb_max = (int) (v*MAX8BITVALUR);
	rgb_min = (int) ((float)rgb_max*((1.-s)/1.));

    h = h % MAXHUE;
	i = h / HUEDIVIDE;
    /* factorial part of h */
	difs = h % HUEDIVIDE;
	/* rgb adjustment amount by hue */
    rgb_adj = (rgb_max - rgb_min)*difs / HUEDIVIDE;

	switch (i)
    {
    	case 0:
    	    color->r = rgb_max;
            color->g = rgb_min + rgb_adj;
    	    color->b = rgb_min;
    		break;
    	case 1:
    		color->r = rgb_max - rgb_adj;
    		color->g = rgb_max;
    		color->b = rgb_min;
    		break;
    	case 2:
    		color->r = rgb_min;
    		color->g = rgb_max;
    		color->b = rgb_min + rgb_adj;
    		break;
    	case 3:
    		color->r = rgb_min;
    		color->g = rgb_max - rgb_adj;
    		color->b = rgb_max;
    		break;
    	case 4:
    		color->r = rgb_min + rgb_adj;
    		color->g = rgb_min;
    		color->b = rgb_max;
    		break;
    	case 5:
    		color->r = rgb_max;
    		color->g = rgb_min;
    		color->b = rgb_max - rgb_adj;
    		break;
        default:
            fprintf(stderr, "%s\n", "hsv2rgb : Error");
	}
}

void print_pattern(pattern stage, int index)
{
    printf("index : %d\nsize : %d\ncx : %d\ncy : %d\ncolor: %d\ncolrange : %d\n"
        ,index, stage.size, stage.cx, stage.cy, stage.color, stage.colrange);
}


void test(void)
{
    test_find_prime_factor();
    test_list_func();
    test_hsv2rgb();
}

void test_find_prime_factor(void)
{
    List *primes;
    char result[MAXLEN];
    char s1[] = "27000 = 1 x 2^3 x 3^3 x 5^3";
    char s2[] = "38654705664 = 1 x 2^32 x 3^2";
    char s3[] = "72 = 1 x 2^3 x 3^2";
    char s4[] = "31 = 1 x 31^1";
    char s5[] = "1 = 1 x 1^1";
    primes = find_prime_factor(27000);
    delete_list_time(primes, 0);
    convert_to_string(result, 27000, primes);
    assert(strcmp(result, s1) == 0);
    free(primes);

    primes = find_prime_factor(38654705664);
    delete_list_time(primes, 0);
    convert_to_string(result, 38654705664, primes);
    assert(strcmp(result, s2) == 0);
    free(primes);

    primes = find_prime_factor(72);
    delete_list_time(primes, 0);
    convert_to_string(result, 72, primes);
    assert(strcmp(result, s3) == 0);
    free(primes);

    primes = find_prime_factor(31);
    delete_list_time(primes, 0);
    convert_to_string(result, 31, primes);
    assert(strcmp(result, s4) == 0);
    free(primes);

    primes = find_prime_factor(1);
    delete_list_time(primes, 0);
    convert_to_string(result, 1, primes);
    assert(strcmp(result, s5) == 0);
    free(primes);
}

void test_list_func(void)
{
    List *primes;
    primes = init_primes();
    assert(primes->first->prime == 2);
    assert(primes->first->prime == 0);

    push_list(primes, 2, 1);
    push_list(primes, 3, 1);

    assert(primes->last->prime == 3);
    assert(primes->last->prime == 1);

    delete_list_time(primes, 1);
    assert(primes->last->prime == 2);
    assert(primes->last->prime == 0);

    free(primes);
}

void test_hsv2rgb(void)
{
    rgb color;

    hsv2rgb(&color, 0, 1., 1.);
    assert(color.r == 0);
    assert(color.g == 0);
    assert(color.b == 255);

    hsv2rgb(&color, 240, 1., 1.);
    assert(color.r == 0);
    assert(color.g == 255);
    assert(color.b == 0);
}
