#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 100

#define LEFT 1
#define MIDDLE 3
#define RIGHT 5

#define strsame(A,B) (strcmp(A,B)==0)
#define ERROR(PHRASE) {fprintf(stderr,\
"Fatal Error %s occured in %s, line %d\n",\
PHRASE,__FILE__,__LINE__);exit(EXIT_FAILURE);}

struct prog{
    char text[MAXNUMTOKENS][MAXTOKENSIZE];
    int cword;
};
typedef struct prog Program;

void Performance(char *progname);
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
int RightSymbol(Program *p);

void VarCon(Program *p);
void Var(Program *p);
void Con(Program *p);
void Strvar(Program *p);
void Numvar(Program *p);
void Strcon(Program *p);
void Numcon(Program *p);

void Set(Program *p);
void Ifequal(Program *p);
void Ifgreater(Program *p);
void In2str(Program *p);
void Innum(Program *p);

int main(int argc,char **argv)
{
    if (argc==2){
        Performance(argv[1]);
    }
    else {
        ERROR("ERROR : Incorrect input \n");
    }
    return 0;
}

void Performance(char *progname)
{   
    Program p;
    FILE *fp;
    int i;
    p.cword=0;
    for (i=0;i<MAXNUMTOKENS;i++){
        p.text[i][0]='\0';
    }
    if (!(fp=fopen(progname,"r"))){
        fprintf(stderr,"Cannot open %s\n",progname);
        exit(EXIT_FAILURE);
    }
    i=0;
    while (fscanf(fp,"%s",p.text[i++])==1 && i<MAXNUMTOKENS);
    assert(i<MAXNUMTOKENS);
    Prog(&p);
    printf("Parsed OK\n");
    fclose(fp);
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
        VarCon(p);
    }
    else if (IsIfcond(p)){
        return;
    }
    else if (IsSet(p)){
        return;
    }
    else if (IsJump(p)){
        return;
    }
    else if (IsFILE(p)){
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
    int length=strlen(p->text[p->cword]);
    char c=p->text[p->cword][0];
    if (length!=1 && c!='='){
        ERROR("Expected a = ?\n");
    }
    p->cword++;
    VarCon(p);
}

void Ifequal(Program *p)
{   
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

int IsPrint(Program *p)
{
   if (strsame(p->text[p->cword],"PRINT") || \
        strsame(p->text[p->cword],"PRINTN")){
       p->cword++;
       return 1;
   }
   else{
       return 0;
   }
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
    else{
        return 0;
    }
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
    if (c=='"' || c=='#'){
        if (d=='"' || d=='#'){
            return 1;
        }
        if (RightSymbol(p)){
            return 1;
        }
        else{
            return 0;
        }
    }
    else {
        return 0;
    }
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

int RightSymbol(Program *p)
{   
    int length;
    char d;
    p->cword++;
    length=strlen(p->text[p->cword]);
    d=p->text[p->cword][length-1];
    if (length==0){
        ERROR("Expecting a correct STRCON ?\n");
    }
    if (d=='"' || d=='#'){
        return 1;
    }
    else {
        return RightSymbol(p);
    }
}

int IsIfcond(Program *p)
{   
    if (IsIfequal(p)){
        p->cword++;
        Ifequal(p);
        p->cword++;
        if (strsame(p->text[p->cword],"{")){
            p->cword++;
            Instrs(p);
            return 1;
        }
        else{
            ERROR("Expecting a { ?\n");
        }
    }
    else if (IsGreater(p)){
        p->cword++;
        Ifgreater(p);
        p->cword++;
        if (strsame(p->text[p->cword],"{")){
            p->cword++;
            Instrs(p);
            return 1;
        }
        else{
            ERROR("Expecting a { ?\n");
        }
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

int IsGreater(Program *p)
{   
    if (strsame(p->text[p->cword],"IFGREATER")){
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

int IsJump(Program *p)
{
    if (strsame(p->text[p->cword],"JUMP")){
        p->cword++;
        if (IsNumcon(p)){
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
