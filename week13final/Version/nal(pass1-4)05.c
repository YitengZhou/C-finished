#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXNUMTOKENS 100
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

int Isprint(Program *p);
int Isifequal(Program *p);
int Isset(Program *p);
int Isvar(Program *p);
int Iscon(Program *p);
int Isstrvar(Program *p);
int Isnumvar(Program *p);
int Isstrcon(Program *p);
int Isnumcon(Program *p);
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
    if (Isprint(p)){
        VarCon(p);
    }
    else if (Isifequal(p)){
        p->cword++;
        Ifequal(p);
    }
    else if (Isset(p)){
        p->cword=p->cword+3;
    }
}

void VarCon(Program *p)
{   
    if (Isvar(p)){
        Var(p);
        return;
    }
    if (Iscon(p)){
        Con(p);
        return;
    }
    ERROR("Expecting a correct variable/constant rules ?\n");
}

void Var(Program *p)
{
    return;
}

void Con(Program *p)
{
    return;
}

void Strvar(Program *p)
{
    return;
}

void Numvar(Program *p)
{
    return;
}

void Strcon(Program *p)
{
    return;
}

void Numcon(Program *p)
{
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

int Isprint(Program *p)
{
   if (strsame(p->text[p->cword],"PRINT")){
       p->cword++;
       return 1;
   }
   else{
       return 0;
   }
}

int Isset(Program *p)
{
    if (Isvar(p)){
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

int Isvar(Program *p)
{
    if (Isstrvar(p)){
        return 1;
    }
    if (Isnumvar(p)){
        return 1;
    }
    return 0;
}

int Iscon(Program *p)
{   
    if (Isstrcon(p)){
        return 1;
    }
    if (Isnumcon(p)){
        return 1;
    }
    return 0;
}

int Isstrvar(Program *p)
{
    char c=p->text[p->cword][0];
    char d=p->text[p->cword][1];
    int length=strlen(p->text[p->cword]);
    if (length==2){
        if (c=='$' && d>='A' && d<='Z'){
            return 1;
        }
    }
    return 0;
}

int Isnumvar(Program *p)
{
    char c=p->text[p->cword][0];
    char d=p->text[p->cword][1];
    int length=strlen(p->text[p->cword]);
    if (length==2){
        if (c=='%' && d>='A' && d<='Z'){
            return 1;
        }
    }
    return 0;
}

int Isstrcon(Program *p)
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

int Isnumcon(Program *p)
{   
    return 0;
}

int Isifequal(Program *p)
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
