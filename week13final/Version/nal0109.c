#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "mvm.h"

#define MAXTOKENSIZE 100

#define LEFT 1
#define MIDDLE 3
#define RIGHT 5
#define ALPHA 26
#define ROTAL 13
#define NUMBER 10
#define ROTNUM 5

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

mvm *varible;

void Performance(char *progname);
Program* Initialtext(char *progname);
Program* InitialStruct(char *progname);
int ProgSize(char *progname);
void Prog(Program *p);
void Instrs(Program *p);
void Instruct(Program *p);

int IsPrint(Program *p);
int IsIfcond(Program *p);
int IsIfequal(Program *p);
int IsGreater(Program *p);
int IsSet(Program *p);
int IsJump(Program *p);
int IsFILE(Program *p);
int IsRND(Program *p);
int IsINC(Program *p);
int IsAbort(Program *p);
int IsInput(Program *p);
int IsIn2str(Program *p);
int IsInnum(Program *p);

int IsVar(Program *p);
int IsCon(Program *p);
int IsStrvar(Program *p);
int IsNumvar(Program *p);
int IsStrcon(Program *p);
int IsNumcon(Program *p);

void VarCon(Program *p);
void Var(Program *p);
void Con(Program *p);
void Strvar(Program *p);
void Numvar(Program *p);
void Strcon(Program *p);
void Numcon(Program *p);
/*void DeleteQuote(Program *p);*/
void ROT18(Program *p);
char ROT13(char c);
char ROT5(char c);

void Set(Program *p);
void Ifequal(Program *p);
void Ifgreater(Program *p);
void In2str(Program *p);
void Innum(Program *p);
void Print(Program *p,int newline);
void DoFile(Program *p);

void Free(Program *p);

int main(int argc,char **argv)
{
    if (argc==2){
        varible=mvm_init();
        Performance(argv[1]);
        #ifdef INTERP
            printf("Interpreter OK\n");
        #else
            printf("Parsed OK\n");
        #endif
        mvm_free(&varible);
    }
    else {
        ERROR("ERROR : Incorrect input \n");
    }
    return 0;
}

void Performance(char *progname)
{   
    Program *p;
    p=Initialtext(progname);
    Prog(p);
    Free(p);
}

Program* Initialtext(char *progname)
{
    FILE *fp;
    Program *p;
    int i=0,length,flag=0;
    char str[MAXTOKENSIZE];

    p=InitialStruct(progname);
    if (!(fp=fopen(progname,"r"))){
        fprintf(stderr,"Cannot open %s\n",progname);
        exit(EXIT_FAILURE);
    }
    while (fscanf(fp,"%s",str)==1 && i<p->progsize){
        length=strlen(str);
        if (str[0]=='"'||str[0]=='#'){
            flag=1;
        }
        if (str[length-1]=='"'||str[length-1]=='#'){
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
    int i;
    Program *p;
    p=(Program *)calloc(1,sizeof(Program));
    if (p==NULL){
        ERROR("Creat program structure failed\n");
    }
    p->progsize=ProgSize(progname);
    p->text=(char **)calloc(p->progsize,sizeof(char *));
    for (i=0;i<p->progsize;i++){
        p->text[i]=(char *)calloc(MAXTOKENSIZE,sizeof(char ));
        if (p->text[i]==NULL){
            ERROR("Creat word text failed\n");
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
        fprintf(stderr,"Cannot open %s\n",progname);
        exit(EXIT_FAILURE);
    }
    while (fscanf(fp,"%s",str)==1){
        length=strlen(str);
        if (str[0]=='"'||str[0]=='#'){
            flag=1;
        }
        if (str[length-1]=='"'||str[length-1]=='#'){
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

void Prog(Program *p)
{
    if (!strsame(p->text[p->cword],"{")){
        ERROR("Program should start with { ?\n");
    }
    p->cword++;
    Instrs(p);
}

void Instrs(Program *p)
{
    /*printf("%d:%s\n",p->cword,p->text[p->cword]);*/   
    if (strsame(p->text[p->cword],"}")){
        return;
    }
    else{
        Instruct(p);
        p->cword++;
        Instrs(p);
    }
}

void Instruct(Program *p)
{   
    if (IsPrint(p)){
        return;
    }
    else if (IsIfcond(p)){
        return;
    }
    else if (IsSet(p)){
        Set(p);
        return;
    }
    else if (IsFILE(p)){
        DoFile(p);
        return;
    }
    else if (IsRND(p)){
        return;
    }
    else if (IsINC(p)){
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
    else {
        printf("%d:%s\n",p->cword,p->text[p->cword]);
        ERROR("Program no this language: \n");
    }
}

void VarCon(Program *p)
{   
    if (IsVar(p)){
        Var(p);
        return;
    }
    if (IsCon(p)){
        Con(p);
        return;
    }
    ERROR("Expecting a correct variable/constant rules ?\n");
}

void Var(Program *p)
{   
    int a;
    if (p->cword<0){
        a=100;
    }
    return;
}

void Con(Program *p)
{
    int a;
    if (p->cword<0){
        a=100;
    }
    return;
}

void Strvar(Program *p)
{
    int a;
    if (p->cword<0){
        a=100;
    }
    return;
}

void Numvar(Program *p)
{
    int a;
    if (p->cword<0){
        a=100;
    }
    return;
}

void Strcon(Program *p)
{
    int a;
    if (p->cword<0){
        a=100;
    }
    return;
}

void Numcon(Program *p)
{
    int a;
    if (p->cword<0){
        a=100;
    }
    return;
}

void Set(Program *p)
{   
    mvm_insert(varible,p->text[p->cword-2],p->text[p->cword]);
    printf("%s->%s\n",p->text[p->cword-2],p->text[p->cword]);
    printf("%s\n",mvm_print(varible));
}

void Ifequal(Program *p)
{   
    char *str;
    /*本版本要改的地方*/
    str=mvm_search(varible,p->text[p->cword+1]);
    printf("str:%s,varible:%s\n",str,p->text[p->cword+1]);
    if (strsame(p->text[p->cword],"(")){
        p->cword++;
        VarCon(p);
        p->cword++;
        Ifequal(p);
    }
    else if (strsame(p->text[p->cword],",")){
        p->cword++;
        VarCon(p);
        p->cword++;
    }
}

void Ifgreater(Program *p)
{   
    if (strsame(p->text[p->cword],"(")){
        p->cword++;
        VarCon(p);
        p->cword++;
        Ifgreater(p);
    }
    else if (strsame(p->text[p->cword],",")){
        p->cword++;
        VarCon(p);
        p->cword++;
    }
}

void In2str(Program *p)
{
    p->cword=p->cword+2;
    if (IsStrvar(p)){
        p->cword=p->cword+2;
        if (IsStrvar(p)){
            p->cword++;
        }
        else{
            ERROR("Expected a STRVAR ?\n");
        }
    }
    else{
        ERROR("Expected a STRVAR ?\n");
    }
}

/*
void DeleteQuote(Program *p)
{
    int i,length=strlen(p->text[p->cword]);
    p->text[p->cword][length-1]='\0';
    for (i=0;i<length-1;i++){
        p->text[p->cword][i]=p->text[p->cword][i+1];
    }
}*/

void ROT18(Program *p)
{
    int i,length=strlen(p->text[p->cword]);
    char c;
    for (i=0;i<length;i++){
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
        change= ((c-'a')+ ROTAL)%ALPHA +'a';
    }
    else {
        change= ((c-'A')+ ROTAL)%ALPHA +'A';
    }
    return change;
}

char ROT5(char c)
{
    char change;
    change= ((c-'0')+ ROTNUM)%NUMBER +'0';
    return change;
}

int IsPrint(Program *p)
{
    if (strsame(p->text[p->cword],"PRINT")){
        p->cword++;
        VarCon(p);
        #ifdef INTERP
            Print(p,1);
        #endif
        return 1;
    }
    else if (strsame(p->text[p->cword],"PRINTN")){
        p->cword++;
        VarCon(p);
        #ifdef INTERP
            Print(p,0);
        #endif
        return 1;
    }
    else{
        return 0;
    }
}

void Print(Program *p,int newline)
{
    int i,length=strlen(p->text[p->cword]);
        for (i=1;i<length-1;i++){
            printf("%c",p->text[p->cword][i]);
        }
        if (newline==1){
            printf("\n");
        }
}

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

int IsSet(Program *p)
{
    if (IsVar(p)){
        p->cword++;
        if (strsame(p->text[p->cword],"=")){
            p->cword++;
            VarCon(p);
            return 1;
        }
        else{
            ERROR("Expected a = ?\n");
        }
    }
    return 0;
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
    int length=strlen(p->text[p->cword]);
    int i;
    char c;
    for (i=0;i<length;i++){
        c=p->text[p->cword][i];
        if ((c<'0'||c>'9') && c!='.'){
            return 0;
        }
    }
    return 1;
}

int IsIfcond(Program *p)
{   
    if (IsIfequal(p)){
        p->cword++;
        Ifequal(p);
        p->cword++;
        p->cword++;
        Instrs(p);
        return 1;
    }
    else if (IsGreater(p)){
        p->cword++;
        Ifgreater(p);
        p->cword++;
        p->cword++;
        Instrs(p);
        return 1;
    }
    else {
        return 0;
    }
}

int IsIfequal(Program *p)
{   
    if (strsame(p->text[p->cword],"IFEQUAL")){
        if (strsame(p->text[p->cword+LEFT],"(") && \
            strsame(p->text[p->cword+MIDDLE],",") && \
            strsame(p->text[p->cword+RIGHT],")") && \
            strsame(p->text[p->cword+RIGHT+1],"{")){
            return 1;
        }
        else{
            ERROR("Expecting a ( , ) {?\n");
        }
    }
    else{
        return 0;
    }
}

int IsGreater(Program *p)
{   
    if (strsame(p->text[p->cword],"IFGREATER")){
        if (strsame(p->text[p->cword+LEFT],"(") && \
            strsame(p->text[p->cword+MIDDLE],",") && \
            strsame(p->text[p->cword+RIGHT],")") && \
            strsame(p->text[p->cword+RIGHT+1],"{")){
            return 1;
        }
        else{
            ERROR("Expecting a ( , ) ?\n");
        }
    }
    else{
        return 0;
    }
}

int IsJump(Program *p)
{
    if (strsame(p->text[p->cword],"JUMP")){
        p->cword++;
        if (IsNumcon(p)){
            #ifdef INTERP
                p->cword=atoi(p->text[p->cword])-1;
            #endif
            return 1;
        }
        else{
            ERROR("Expecting a NUMCON ?\n");
        }
    }
    else{
        return 0;
    }
}

int IsFILE(Program *p)
{   
    if (strsame(p->text[p->cword],"FILE")){
        p->cword++;
        if (IsStrcon(p)){
            return 1;
        }
        else{
            ERROR("Expecting a STRCON ?\n");
        }
    }
    else{
        return 0;
    }
}

int IsRND(Program *p)
{
    if (strsame(p->text[p->cword],"RND")){
        if (strsame(p->text[p->cword+LEFT],"(") && \
            strsame(p->text[p->cword+MIDDLE],")")){
            p->cword=p->cword+2;
            if (IsNumvar(p)){
                p->cword++;
                return 1;
            }
        }
        else{
            ERROR("Expecting a braces (NUMVAR) ?\n");
        }
    }
    return 0;
}

int IsINC(Program *p)
{
    if (strsame(p->text[p->cword],"INC")){
        if (strsame(p->text[p->cword+LEFT],"(") && \
            strsame(p->text[p->cword+MIDDLE],")")){
            p->cword=p->cword+2;
            if (IsNumvar(p)){
                p->cword++;
                return 1;
            }
        }
        else{
            ERROR("Expecting a braces (NUMVAR) ?\n");
        }
    }
    return 0;
}

int IsAbort(Program *p)
{
    if (strsame(p->text[p->cword],"ABORT")){
        return 1;
    }
    else{
        return 0;
    }
}

int IsInput(Program *p)
{
    if (IsIn2str(p)){
        In2str(p);
        return 1;
    }
    else if (IsInnum(p)){
        return 1;
    }
    else {
        return 0;
    }
}

int IsIn2str(Program *p)
{
    if (strsame(p->text[p->cword],"IN2STR")){
        if (strsame(p->text[p->cword+LEFT],"(") && \
            strsame(p->text[p->cword+MIDDLE],",") && \
            strsame(p->text[p->cword+RIGHT],")")){
            return 1;
        }
        else{
            ERROR("Expecting a ( , ) ?\n");
        }
    }
    else{
        return 0;
    }
}

int IsInnum(Program *p)
{
    if (strsame(p->text[p->cword],"INNUM")){
        if (strsame(p->text[p->cword+LEFT],"(") && \
            strsame(p->text[p->cword+MIDDLE],")")){
            p->cword=p->cword+2;
            if (IsNumvar(p)){
                p->cword++;
                return 1;
            }
        }
        else{
            ERROR("Expecting a braces (NUMVAR) ?\n");
        }
    }
    return 0;
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
