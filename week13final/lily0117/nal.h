
#define NUMTOKENS 100         /* 初始可存储的代码数量为100 */
#define MAXTOKENSIZE 50       /* 每个代码的最长长度 */
#define RAND_NUMBER 100
#define END 1                 /* store the end symbal of string */
#define MAX_QUEUE 1000        /* 队列的长度 */
#define INPUTSTRLEN 20        /* 输入字符串的最大长度 */
#define INPUTNUMLEN 11        /* 输入数字的最大长度 */
#define LETTERENCODE 13       /* 字符解码 */
#define NUMBERENCODE 5        /* 数字解码 */
#define COMPARE 7             /* IFEQUAL或IFGREATER所在位置距离当前位置的距离 */
#define FIRST_STR 5           /* 待比较的第一个变量所在位置距离当前位置的距离 */
#define SECOND_STR 3          /* 待比较的第二个变量所在位置距离当前位置的距离 */
#define strsame(A, B) (strcmp(A, B) == 0)
/* error */
#define ERROR(PHRASE) { fprintf(stderr, \
        "Fatal Error: %s occured in %s, line %d\n", \
        PHRASE, __FILE__, __LINE__); exit(EXIT_FAILURE); }
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)


typedef struct prog{
   char **codes;
   int cc;        /* current code */
   int total;     /* 总共的code数量，空格或者换行符分隔每个code */
   int actual;    /* 实际存储的code数量 */
}Prog;

typedef struct queue {
   char n[MAX_QUEUE];
   int front;
   int back;
}Queue;           /* 队列，用来存储 { } */

void test(void);
/************************ queue ***************************/
void initialiseQueue(Queue *q);
char removeQueue(Queue *q);
void insertQueue(char c, Queue *q);
/********************** 读入文件内容 ************************/
FILE *tryOpenFile(char *filename);
Prog *readIn(FILE *fp);
Prog *progInit(void);
char *rowInit(void);
void ifResize(Prog *p, int num);
/************************ 解析文件内容 ******************************/
void program(Prog *prog);
void instrs(Prog *p);
void instruct(Prog *p);
void file(Prog *p);
void executeFile(char *file);
void abortHere(Prog *p);
void input(Prog *p);
void inputAndInsert(char *name, int flag);
void jump(Prog *p);
int jumpPosition(Prog *p, int number);
void print(Prog *p);
void printVariable(Prog *p);
void rnd(Prog *p);
void ifCond(Prog *p);
void compare(Prog *p);
char *parseString(char *str);
void inc(Prog *p);
void addOne(char *num_name);
void set(Prog *p);
void setInsert(Prog *p);
/************************* 工具函数 ******************************/
void judgeNumber(char *str, int *p_repeat);
char *num2String(float number);
float string2Num(char *str);
char *strFromStrcon(Prog *p);
void encodeRot18(char *str);
void deleteSign(char *str);
/************************* 变量或常量 ****************************/
void varcon(Prog *p);
void strvar(Prog *p);
void numvar(Prog *p);
void strcon(Prog *p);
void strcon1(Prog *p);
void strcon2(Prog *p);
void numcon(Prog *p);
/********************* 判断字符是否符合标准 **********************/
void isCapital(char *str, int len);
void expLeftParenthesis(char *str);
void expRightParenthesis(char *str);
void expLeftBracket(char *str);
void expRightBracket(char *str);
void expComma(char *str);
void expEqualSign(char *str);
/************************ free ***************************/
void freeMemory(Prog **p);
