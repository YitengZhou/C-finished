/* Max length of word */
#define MAXTOKENSIZE 1000

/* Brackets distance */
#define LEFT 2
#define MIDDLE 4
#define RIGHT 5

/* ROT18 rules */
#define ALPHA 26
#define ROTAL 13
#define NUMBER 10
#define ROTNUM 5
#define RANDOM 100

/* Briefly function */
#define strsame(A,B) (strcmp(A,B)==0)
#define ERROR(PHRASE) {fprintf(stderr,\
"Fatal Error %s occured in %s, line %d\n",\
PHRASE,__FILE__,__LINE__);exit(EXIT_FAILURE);}

struct prog{
    char **text;
    int cword;
    int progsize;
};
typedef struct prog Program;

/* Execute the program */
void Interpter(void);
void Performance(char *progname);
void EndProgram(void);

/* The grammar in program */
void Prog(Program *p);
void Instrs(Program *p);
void Instruct(Program *p);

/* Initialize the progname structure */
Program* Initialtext(char *progname);
Program* InitialStruct(char *progname);
int ProgSize(char *progname);
int IsQuote(char c);

/* The grammar in instruct */
int IsFILE(Program *p);
int IsAbort(Program *p);
int IsInput(Program *p);
int IsJump(Program *p);
int IsPrint(Program *p);
int IsRND(Program *p);
int IsIfcond(Program *p);
int IsINC(Program *p);
int IsSet(Program *p);

/* The grammar in VARCON */
void VarCon(Program *p);
int IsVar(Program *p);
int IsCon(Program *p);
int IsStrvar(Program *p);
int IsNumvar(Program *p);
int IsStrcon(Program *p);
int IsNumcon(Program *p);

/* ROT18 rules */
void ROT18(Program *p);
char ROT13(char c);
char ROT5(char c);

/* The grammar in IFCOND */
int IsIfequal(Program *p);
int IsGreater(Program *p);

/* The grammar in INPUT */
int IsIn2str(Program *p);
int IsInnum(Program *p);

/* The interpreter in instruct */
void DoFile(Program *p);
void In2str(Program *p);
void Innum(Program *p);
void Print(Program *p,int newline);
void RND(Program *p);
void Equal(Program *p);
void Greater(Program *p);
void Set(Program *p);
void INC(Program *p);

/* Some assistant function */
void AddQuote(char *input);
char* FindValue(Program *p);
void NextBracket(Program *p);
void NextWord(Program *p);
void NextTwoWord(Program *p);
int IsNumber(char *string);
void Free(Program *p);
void Test(void);

/* Extension part */
int IsFOR(Program *p);
int FOR(Program *p);
int IsADD(Program *p);
void ADD(Program *p);



