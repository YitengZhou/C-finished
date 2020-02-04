
#define NUMTOKENS 100         /* 初始可存储的代码数量为100 */
#define MAXTOKENSIZE 100          /* 每个代码的最长长度 */
#define NOPRINT 0
#define PRINT 1
#define strsame(A, B) (strcmp(A, B) == 0)
#define ERROR(PHRASE) { fprintf(stderr, \
        "Fatal Error: %s occured in %s, line %d\n", \
        PHRASE, __FILE__, __LINE__); exit(EXIT_FAILURE); }
/* Error */
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)
#define MAX_QUEUE 1000

typedef struct prog{
   char **codes;
   int cur;   /* cur code */
   int total;     /* 总共的code数量，空格或者换行符分隔每个code */
}Prog;

typedef struct queue {
   char n[MAX_QUEUE];
   int front;
   int back;
}Queue;

/* 队列 */
void initialiseQueue(Queue *q);
char removeQueue(Queue *q);
void insertQueue(char c, Queue *q);
int emptyQueue(Queue q);
/* 将文件内容读入动态内存空间 */
FILE *tryOpenFile(char *filename);
Prog *progInit(void);
char *rowInit(void);
void ifResize(Prog *p, int num);
Prog *readIn(FILE *fp);
void freeMemory(Prog **p);
/* 解析文件中的代码 */
void program(Prog *prog);
void instrs(Prog *p);
void instruct(Prog *p);
void file(Prog *p);
void abortHere(void);
void input(Prog *p);
void inputAndInsert(char *name, int flag);
void jump(Prog *p);
void print(Prog *p);
void rnd(Prog *p);
void ifCond(Prog *p);
void inc(Prog *p);
void set(Prog *p);
void setInsert(Prog *p);
/* 变量常数规则 */
void varcon(Prog *p);
void strvar(Prog *p);
void numvar(Prog *p);
void strcon(Prog *p);
void strcon1(Prog *p);
void strcon2(Prog *p);
void numcon(Prog *p);
/* 其他*/
int needCount(char *str);
void compare(Prog *p);
char *parseString(char *str);
char *strFromStrcon(Prog *p);
void executeFile(char *file);
int jumpPosition(Prog *p, int number);
void addOne(char *num_name);
char *num2String(double number);
double string2Num(char *str);
void rot2Plain(char *str);
/* 去掉字符串的 “ ”, # # */
void deleteSign(char *str);
/* 判断字符串是否为相应的内容 */
void isCapital(char *str, int len);
void expLeftParenthesis(char *str);
void expRightParenthesis(char *str);
void expLeftBracket(char *str);
void expRightBracket(char *str);
void expComma(char *str);
void expEqualSign(char *str);
