#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "mvm.h"
#include "nal.h"

mvm *varible;

int main(int argc,char **argv)
{
    if (argc==2){
        Interpter();
        Performance(argv[1]);
        EndProgram();
    }
    else {
        fprintf(stderr,"ERROR : Incorrect input. try "\
        "%s test1.nal in %s, line %d\n",argv[0],__FILE__,__LINE__);
        exit(EXIT_FAILURE);
    }
    return 0;
}

/* If INTERP, calloc program structure and set random seed */
void Interpter(void)
{
    #ifdef INTERP
        varible=mvm_init();
        srand(time(0));
    #endif
    return;
}

/* Execute program and free program */
void Performance(char *progname)
{   
    Program *p;
    p=Initialtext(progname);
    Prog(p);
    Free(p);
}

/* Free global mvm and print 'parse' or 'interp' */
void EndProgram(void)
{
    #ifdef INTERP
        mvm_free(&varible);
        printf("Interpreter OK\n");
    #else
        printf("Parsed OK\n");
    #endif
    #ifdef TESTNAL
        varible = mvm_init();
        Test();
        mvm_free(&varible);
    #endif
    return;
}

void Prog(Program *p)
{
    if (!strsame(p->text[p->cword],"{")){
        ERROR("Program should start with { ?");
    }
    NextWord(p);
    Instrs(p);
}

void Instrs(Program *p)
{
    if (strsame(p->text[p->cword],"}")){
        return;
    }
    else{
        Instruct(p);
        NextWord(p);
        Instrs(p);
    }
}

void Instruct(Program *p)
{   
    if (IsFILE(p)){
        DoFile(p);
        return;
    }
    else if (IsAbort(p)){
        return;
    }
    else if (IsInput(p)){
        return;
    }
    else if (IsJump(p)){
        return;
    }
    else if (IsPrint(p)){
        return;
    }
    else if (IsRND(p)){
        return;
    }
    else if (IsIfcond(p)){
        return;
    }
    else if (IsINC(p)){
        return;
    }
    else if (IsSet(p)){
#ifdef INTERP
            Set(p);
#endif
        return;
    }
    else {
        printf("%d:%s\n",p->cword,p->text[p->cword]);
        ERROR("Program no this language");
    }
}

/* Initialize the progname structure */
Program* Initialtext(char *progname)
{
    FILE *fp;
    Program *p;
    int i=0,length,flag=0;
    char str[MAXTOKENSIZE];

    p=InitialStruct(progname);
    fp=fopen(progname,"r");

    while (fscanf(fp,"%s",str)==1){
        length=strlen(str);
        if (IsQuote(str[0])){
            flag=1;
        }
        if (IsQuote(str[length-1])){
            flag=0;
        }
        strcat(p->text[i++],str);
        if (flag==1){
            i--;
            length=strlen(p->text[i]);
            p->text[i][length]=' ';
        }
    }
    fclose(fp);
    return p;
}

Program* InitialStruct(char *progname)
{   
    Program *p;
    int i;
    
    p=(Program *)calloc(1,sizeof(Program));
    if (p==NULL){
        ERROR("Creat program structure failed");
    }
    p->progsize=ProgSize(progname);
    p->text=(char **)calloc(p->progsize,sizeof(char *));
    for (i=0;i<p->progsize;i++){
        p->text[i]=(char *)calloc(MAXTOKENSIZE,sizeof(char ));
        if (p->text[i]==NULL){
            ERROR("Creat word text failed");
        }
    }
    return p;
}

int ProgSize(char *progname)
{
    int size=0,flag=0,length;
    char str[MAXTOKENSIZE];
    FILE *fp;
    if (!(fp=fopen(progname,"r"))){
        fprintf(stderr,"ERROR : Cannot open %s in %s, line %d\n"\
        ,progname,__FILE__,__LINE__);
        exit(EXIT_FAILURE);
    }
    while (fscanf(fp,"%s",str)==1){
        length=strlen(str);
        if (IsQuote(str[0])){
            flag=1;
        }
        if (IsQuote(str[length-1])){
            flag=0;
        }
        size++;
        if (flag==1){
            size--;
        }
    }
    fclose(fp);
    return size;
}

int IsQuote(char c)
{
    if (c=='"'||c=='#'){
        return 1;
    }
    else {
        return 0;
    }
}

/* The grammar in instruct */
int IsFILE(Program *p)
{   
    if (strsame(p->text[p->cword],"FILE")){
        NextWord(p);
        if (IsStrcon(p)){
            return 1;
        }
        else{
            ERROR("Expecting a STRCON in FILE grammar ?");
        }
    }
    return 0;
}

int IsAbort(Program *p)
{
    if (strsame(p->text[p->cword],"ABORT")){
#ifdef INTERP
            mvm_free(&varible);
            Free(p);
            printf("ABORT this program !\n");
            exit(EXIT_SUCCESS);
#endif
        return 1;
    }
    return 0;
}

int IsInput(Program *p)
{
    if (IsIn2str(p)){
#ifdef INTERP
            In2str(p);
#endif
        NextWord(p);
        return 1;
    }
    if (IsInnum(p)){
#ifdef INTERP
            Innum(p);
#endif
        NextWord(p);
        return 1;
    }
    return 0;
}

int IsJump(Program *p)
{
    if (strsame(p->text[p->cword],"JUMP")){
        NextWord(p);
        if (IsNumcon(p)){
#ifdef INTERP
                p->cword=atoi(p->text[p->cword])-1;
#endif
            return 1;
        }
        else{
            ERROR("Expecting a NUMCON in JUMP grammar?");
        }
    }
    return 0;
}

/* If INTERP, print(1) or printn(0) word */
int IsPrint(Program *p)
{
    if (strsame(p->text[p->cword],"PRINT")){
        NextWord(p);
        VarCon(p);
#ifdef INTERP
            Print(p,1);
#endif
        return 1;
    }
    if (strsame(p->text[p->cword],"PRINTN")){
        NextWord(p);
        VarCon(p);
#ifdef INTERP
            Print(p,0);
#endif
        return 1;
    }
    return 0;
}

int IsRND(Program *p)
{
    if (strsame(p->text[p->cword],"RND")){
        NextWord(p);
        if (strsame(p->text[p->cword],"(") && \
            strsame(p->text[p->cword+LEFT],")")){
            NextWord(p);
            if (IsNumvar(p)){
#ifdef INTERP
                    RND(p);
#endif
                NextWord(p);
                return 1;
            }
            else{
                ERROR("Expected a NUMVAR in RND grammar ?");
            }
        }
        else{
            ERROR("Expecting a pair of braces ( NUMVAR ) in RND grammar ?");
        }
    }
    return 0;
}

int IsIfcond(Program *p)
{
    if (IsIfequal(p)){
        Equal(p);
        Instrs(p);
        return 1;
    }
    if (IsGreater(p)){
        Greater(p);
        Instrs(p);
        return 1;
    }
    return 0;
}

int IsIfequal(Program *p)
{   
    if (strsame(p->text[p->cword],"IFEQUAL")){
        NextWord(p);
        if (strsame(p->text[p->cword],"(") && \
            strsame(p->text[p->cword+LEFT],",") && \
            strsame(p->text[p->cword+MIDDLE],")") && \
            strsame(p->text[p->cword+RIGHT],"{")){
            NextWord(p);
            return 1;
        }
        else{
            ERROR("Expecting a series of symbols ( , ) { in IFEQUAL grammar?");
        }
    }
    return 0;
}

int IsGreater(Program *p)
{   
    if (strsame(p->text[p->cword],"IFGREATER")){
        NextWord(p);
        if (strsame(p->text[p->cword],"(") && \
            strsame(p->text[p->cword+LEFT],",") && \
            strsame(p->text[p->cword+MIDDLE],")") && \
            strsame(p->text[p->cword+RIGHT],"{")){
            NextWord(p);
            return 1;
        }
        else{
            ERROR("Expecting a series of symbols ( , ) { in IFGREATER grammar?");
        }
    }
    return 0;
}

int IsINC(Program *p)
{
    if (strsame(p->text[p->cword],"INC")){
        NextWord(p);
        if (strsame(p->text[p->cword],"(") && \
            strsame(p->text[p->cword+LEFT],")")){
            NextWord(p);
            if (IsNumvar(p)){
#ifdef INTERP
                    INC(p);
#endif
                NextWord(p);
                return 1;
            }
            else{
                ERROR("Expected a NUMVAR in INC grammar ?");
            }
        }
        else{
            ERROR("Expecting a braces ( NUMVAR ) in INC grammar ?");
        }
    }
    return 0;
}

int IsSet(Program *p)
{
    if (IsVar(p)){
        NextWord(p);
        if (strsame(p->text[p->cword],"=")){
            NextWord(p);
            VarCon(p);
            return 1;
        }
        else{
            ERROR("Expected a = in SET grammar ?");
        }
    }
    if (IsCon(p)){
        ERROR("Expected a STRVAR/NUMVAR in SET grammar ?");
    }
    return 0;
}

/* The grammar in VARCON */
void VarCon(Program *p)
{   
    if (IsVar(p)){
        return;
    }
    if (IsCon(p)){
        return;
    }
    ERROR("Expecting a correct variable/constant rules in VARCON grammar ?");
}

int IsVar(Program *p)
{
    if (IsStrvar(p)){
        return 1;
    }
    if (IsNumvar(p)){
        return 1;
    }
    return 0;
}

int IsCon(Program *p)
{   
    if (IsStrcon(p)){
        return 1;
    }
    if (IsNumcon(p)){
        return 1;
    }
    return 0;
}

int IsStrvar(Program *p)
{
    char d,c=p->text[p->cword][0];
    int i,length=strlen(p->text[p->cword]);
    if (c!='$'){
        return 0;
    }
    for (i=1;i<length;i++){
        d=p->text[p->cword][i];
        if (d<'A' && d>'Z'){
            return 0;
        }
    }
    return 1;
}

int IsNumvar(Program *p)
{
    char d,c=p->text[p->cword][0];
    int i,length=strlen(p->text[p->cword]);
    if (c!='%'){
        return 0;
    }
    for (i=1;i<length;i++){
        d=p->text[p->cword][i];
        if (d<'A' && d>'Z'){
            return 0;
        }
    }
    return 1;
}

int IsStrcon(Program *p)
{   
    char c=p->text[p->cword][0];
    int length=strlen(p->text[p->cword]);
    char d=p->text[p->cword][length-1];
    if (c=='"' && d=='"'){
        return 1;
    }
    if (c=='#' && d=='#'){
        ROT18(p);
        return 1;
    }
    return 0;
}

int IsNumcon(Program *p)
{   
    int i,length=strlen(p->text[p->cword]);
    char c;
    for (i=0;i<length;i++){
        c=p->text[p->cword][i];
        if ((c<'0'||c>'9') && c!='.'){
            return 0;
        }
    }
    return 1;
}

/* ROT18 rules */
void ROT18(Program *p)
{
    int i,length=strlen(p->text[p->cword]);
    char c;
    p->text[p->cword][0]=p->text[p->cword][length-1]='"';
    for (i=1;i<length-1;i++){
        c=p->text[p->cword][i];
        if (isalpha(c)){
            p->text[p->cword][i]=ROT13(c);
        }
        if (isdigit(c)){
            p->text[p->cword][i]=ROT5(c);
        }
    }
}

char ROT13(char c)
{
    char change;
    if (islower(c)){
        change= ((c-'a')+ ROTAL) % ALPHA +'a';
    }
    else {
        change= ((c-'A')+ ROTAL) % ALPHA +'A';
    }
    return change;
}

char ROT5(char c)
{
    char change;
    change= ((c-'0')+ ROTNUM) % NUMBER +'0';
    return change;
}

/* The grammar in IFCOND */
int IsIn2str(Program *p)
{
    if (strsame(p->text[p->cword],"IN2STR")){
        NextWord(p);
        if (strsame(p->text[p->cword],"(") && \
            strsame(p->text[p->cword+LEFT],",") && \
            strsame(p->text[p->cword+MIDDLE],")")){
            NextWord(p);
            if (IsStrvar(p)){
                NextTwoWord(p);
                if (IsStrvar(p)){
                    return 1;
                }
            }
            ERROR("Expected a STRVAR in IN2STR grammar ?");
        }
        else{
            ERROR("Expecting a ( , ) in IN2STR grammar ?");
        }
    }
    return 0;
}

int IsInnum(Program *p)
{
    if (strsame(p->text[p->cword],"INNUM")){
        NextWord(p);
        if (strsame(p->text[p->cword],"(") && \
            strsame(p->text[p->cword+LEFT],")")){
            NextWord(p);
            if (IsNumvar(p)){
                return 1;
            }
            else{
                ERROR("Expected a NUMVAR in INNUM grammar ?");
            }
        }
        else{
            ERROR("Expecting a braces ( NUMVAR ) in INNUM grammar?");
        }
    }
    return 0;
}

/* The grammar in INPUT */
void DoFile(Program *p)
{   
    char str[MAXTOKENSIZE];
    int i,length=strlen(p->text[p->cword]);
        strcpy(str,p->text[p->cword]);
        for (i=0;i<length-2;i++){
            str[i]=str[i+1];
        }
        str[i]='\0';
#ifdef INTERP
        Performance(str);
#endif
}

void In2str(Program *p)
{   
    char str1[MAXTOKENSIZE],str2[MAXTOKENSIZE];
    scanf("%s %s",str1,str2);
    AddQuote(str1);
    AddQuote(str2);
    mvm_insert(varible,p->text[p->cword-LEFT],str1);
    mvm_insert(varible,p->text[p->cword],str2);
}

void Innum(Program *p)
{
    char str[MAXTOKENSIZE];
    scanf("%s",str);
    mvm_insert(varible,p->text[p->cword],str);
}

void Print(Program *p,int newline)
{
    int i;
    char *str;
    if (IsVar(p)){
        str = mvm_search(varible, p->text[p->cword]);
    }
    else {
        str = p->text[p->cword];
    }
    if (!IsQuote(str[0])){
        printf("%s", str);
    }
    else{
        for (i = 1; i < (int)strlen(str) - 1; i++){
            printf("%c", str[i]);
        }
    }
    if (newline == 1){
        printf("\n");
    }
}

void RND(Program *p)
{
    int random;
    char str[MAXTOKENSIZE];
    random=(int)rand()%RANDOM;
    sprintf(str,"%d",random);
    mvm_insert(varible,p->text[p->cword],str);
}

void Equal(Program *p)
{
#ifdef INTERP
        char *value1,*value2;
#endif
    VarCon(p);
#ifdef INTERP
        value1=FindValue(p);
#endif
    NextTwoWord(p);
    VarCon(p);
#ifdef INTERP
        value2=FindValue(p);
        NextWord(p);
        if (strsame(value1,value2)){
            NextTwoWord(p);
        }
        else {
            NextBracket(p);
        }
#else
        NextWord(p);
        NextTwoWord(p);
#endif
}

void Greater(Program *p)
{
#ifdef INTERP
        char *value1,*value2;
#endif
    VarCon(p);
#ifdef INTERP
        value1=FindValue(p);
#endif
    NextTwoWord(p);
    VarCon(p);
#ifdef INTERP
        value2=FindValue(p);
        NextWord(p);
        if (IsNumber(value1) && IsNumber(value2)){
            if (atoi(value1) > atoi(value2)){
                NextTwoWord(p);
                return;
            }
            else{
                NextBracket(p);
            }
        }
        else{
            if (strcmp(value1,value2)>0){
                NextTwoWord(p);
                return;
            }
            else{
                NextBracket(p);
            }
        }
#else
        NextWord(p);
        NextTwoWord(p);
#endif
}

void INC(Program *p)
{
    char *str;
    char add[MAXTOKENSIZE];
    int new;
    str=mvm_search(varible,p->text[p->cword]);
    new=atoi(str)+1;
    sprintf(add,"%d",new);
    mvm_insert(varible,p->text[p->cword],add);
}

void Set(Program *p)
{   
    char *value;
    if (IsVar(p)){
        value=mvm_search(varible,p->text[p->cword]);
        mvm_insert(varible, p->text[p->cword - LEFT], value);
    }
    if (IsCon(p)){
        mvm_insert(varible, p->text[p->cword - LEFT], p->text[p->cword]);
    }
}

/* Some assistant function */
void AddQuote(char *input)
{
    int i,length=strlen(input);
    for (i=length;i>0;i--){
        input[i]=input[i-1];
    }
    input[0]=input[length+1]='"';
    input[length+2]='\0';
}

char* FindValue(Program *p)
{
    char* value;
    if (IsVar(p)){
        value=mvm_search(varible,p->text[p->cword]);
    }
    else{
        value=p->text[p->cword];
    }
    return value;
}

void NextBracket(Program *p)
{
    int Left=0,Right=0;
    while (Left==0 || Left !=Right){
        NextWord(p);
        if (strsame(p->text[p->cword],"{")){
            Left++;
        }
        if (strsame(p->text[p->cword],"}")){
            Right++;
        }
    }
}

void NextWord(Program *p)
{
    p->cword++;
}

void NextTwoWord(Program *p)
{
    p->cword=p->cword+2;
}

int IsNumber(char *string)
{
    int i, length = strlen(string);
    char c;
    for (i = 0; i < length; i++) {
        c = string[i];
        if ((c < '0' || c > '9') && c != '.') {
            return 0;
        }
    }
    return 1;
}

void Free(Program *p)
{
    int i;
    if (p!=NULL){
        for (i=0;i<p->progsize;i++){
            free(p->text[i]);
        }
        free(p->text);
        free(p);
    }
}

void Test(void)
{
    printf("Basic Grammar Tests File ... Start\n");

    Performance("testFILE.nal");
    printf("Tests File <testFILE.nal> OK\n");

    Performance("testIN2STR.nal");
    printf("Tests File <testIN2STR.nal> OK\n");

    Performance("testINNUM.nal");
    printf("Tests File <testINNUM.nal> OK\n");

    Performance("testJUMP.nal");
    printf("Tests File <testJUMP.nal> OK\n");

    Performance("testPRINT.nal");
    printf("Tests File <testPRINT.nal> OK\n");

    Performance("testRND.nal");
    printf("Tests File <testRND.nal> OK\n");

    Performance("testIFCOND.nal");
    printf("Tests File <testIFCOND.nal> OK\n");

    Performance("testINC.nal");
    printf("Tests File <testINC.nal> OK\n");

    Performance("testSET.nal");
    printf("Tests File <testSET.nal> OK\n");

    printf("Basic Grammar Tests File ... Stop\n");
}
