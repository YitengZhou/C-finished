#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#define MAXTOKENSIZE 1000

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

void Performance(char *progname);
Program* Initialtext(char *progname);
Program* InitialStruct(char *progname);
int ProgSize(char *progname);
void Print(Program *p);

void Free(Program *p);

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
    Program *p;
    p=Initialtext(progname);
    Print(p);
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

void Print(Program *p)
{
    int i,length=p->progsize;
    for (i=0;i<length;i++){
        printf("%d:%s\n",i,p->text[i]);
    }
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
