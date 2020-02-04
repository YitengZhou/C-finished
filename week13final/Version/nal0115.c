#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "mvm.h"
#include "nal.h"
#include "read.h"

mvm *varible;

int main(int argc,char **argv)
{
    #ifdef INTERP
        varible=mvm_init();
        srand(time(0));
    #endif
    if (argc==2){
        Performance(argv[1]);
        #ifdef INTERP
            mvm_free(&varible);
            printf("Interpreter OK\n");
        #else
            printf("Parsed OK\n");
        #endif
    }
    else {
        fprintf(stderr,"ERROR : Incorrect input. try "\
        "%s test1.nal in %s, line %d\n",argv[0],__FILE__,__LINE__);
        exit(EXIT_FAILURE);
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
        #ifdef INTERP
            Set(p);
        #endif
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
        return;
    }
    if (IsCon(p)){
        return;
    }
    ERROR("Expecting a correct variable/constant rules ?\n");
}

void Set(Program *p)
{   
    mvm_insert(varible,p->text[p->cword-2],p->text[p->cword]);
}

int Ifequal(Program *p)
{   
    #ifdef INTERP
        char *value1,*value2;
    #endif
    p->cword++;
    VarCon(p);
    #ifdef INTERP
        if (p->text[p->cword][0]=='$'||p->text[p->cword][0]=='%'){ /*如果是float*/
            value1=mvm_search(varible,p->text[p->cword]);
            /*printf("search:%s->%s\n",p->text[p->cword],value1);*/
        }
        else{
            value1=p->text[p->cword];
        }
    #endif
    p->cword++;
    p->cword++;
    VarCon(p);
    #ifdef INTERP
        if (p->text[p->cword][0]=='$'||p->text[p->cword][0]=='%'){
            value2=mvm_search(varible,p->text[p->cword]);
        }
        else{
            value2=p->text[p->cword];
            /*printf("again search: %s->%s\n",value1,value2);*/
        }   
        if (strsame(value1,value2)){
            p->cword=p->cword+1;
            return 1;
        }
        else {
            NextBracket(p);
            /*p->cword=p->cword+1;
            printf("%d/%d\n",p->cword,p->progsize);*/
            return 0;
        }
    #else
        p->cword=p->cword+1;
        return 1;
    #endif
}

void NextBracket(Program *p)
{
    int Left=0,Right=0;
    while (Left==0 || Left !=Right){
        p->cword++;
        if (strsame(p->text[p->cword],"{")){
            Left++;
        }
        if (strsame(p->text[p->cword],"}")){
            Right++;
        }
    }
}

int Ifgreater(Program *p)
{   
    #ifdef INTERP
        char *value1,*value2;
    #endif
    p->cword++;
    VarCon(p);
    #ifdef INTERP
        if (p->text[p->cword][0]=='%'){
            value1=mvm_search(varible,p->text[p->cword]);
        }
        else{
            value1=p->text[p->cword];
        }
    #endif
    p->cword++;
    p->cword++;
    VarCon(p);
    #ifdef INTERP
        if (p->text[p->cword][0]=='%'){
            value2=mvm_search(varible,p->text[p->cword]);
        }
        else{
            value2=p->text[p->cword];
        }
        if (atoi(value1)>atoi(value2)){
            p->cword++;
            return 1;
        }
        else {
            NextBracket(p);
            p->cword++;
            return 0;
        }
    #else
        p->cword=p->cword+1;
        return 1;
    #endif
}

void In2str(Program *p)
{   
    #ifdef INTERP
    char str1[MAXTOKENSIZE],str2[MAXTOKENSIZE];
    #endif
    p->cword=p->cword+2;
    if (IsStrvar(p)){
        p->cword=p->cword+2;
        if (IsStrvar(p)){
            #ifdef INTERP
                scanf("%s %s",str1,str2);
                AddQuote(str1);
                AddQuote(str2);
                mvm_insert(varible,p->text[p->cword-2],str1);
                mvm_insert(varible,p->text[p->cword],str2);
            #endif
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

void AddQuote(char *input)
{
    int i,length=strlen(input);
    for (i=length;i>0;i--){
        input[i]=input[i-1];
    }
    input[0]=input[length+1]='"';
    input[length+2]='\0';
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
    if (p->text[p->cword][0]=='%' || p->text[p->cword][0]=='$'){
        printf("%s",mvm_search(varible,p->text[p->cword]));
    }
    else{
        for (i=1;i<length-1;i++){
            printf("%c",p->text[p->cword][i]);
        }
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
        if (Ifequal(p)){
            p->cword++;
            p->cword++;
            Instrs(p);
        }
        else {
            Instrs(p);
        }
        return 1;
    }
    else if (IsGreater(p)){
        p->cword++;
        if (Ifgreater(p)){
            p->cword++;
            p->cword++;
            Instrs(p);
        }
        else {
            Instrs(p);
        }
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
                #ifdef INTERP
                    RND(p);
                #endif
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

void RND(Program *p)
{
    int random;
    char str[MAXTOKENSIZE];
    random=(int)rand()%RANDOM;
    sprintf(str,"%d",random);
    mvm_insert(varible,p->text[p->cword],str);
}

int IsINC(Program *p)
{
    if (strsame(p->text[p->cword],"INC")){
        if (strsame(p->text[p->cword+LEFT],"(") && \
            strsame(p->text[p->cword+MIDDLE],")")){
            p->cword=p->cword+2;
            if (IsNumvar(p)){
                #ifdef INTERP
                    INC(p);
                #endif
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
                #ifdef INTERP
                    Innum(p);
                #endif
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

void Innum(Program *p)
{
    char str[MAXTOKENSIZE];
    scanf("%s",str);
    mvm_insert(varible,p->text[p->cword],str);
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
