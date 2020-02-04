/* 用fscanf读取文件内容，所以必须用空格，且空格和换行符处理一样、 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "12.h"
/* ****************************************** */
int main(int argc, char *argv[])
{
   FILE *fp;
   Prog *prog = NULL;

   if (argc < 2) {
     ON_ERROR("ERROR: Incorrect usage. Please try again.\n");
   }
   fp = tryOpenFile(argv[1]);
   prog = readIn(fp);
   program(prog);
   #ifndef INTERP             /* 如果没有 -DINTERP，则为parse */
   printf("Parsed OK \n");
   #endif
   #ifdef INTERP             /* 如果有 -DINTERP，则为interpret */
   printf("Interpreted OK \n");
   #endif
   freeMemory(&prog);
   return 0;
}
/* ****************************************** */
void program(Prog *p)
{
   p->cur = 0;
   expLeftBracket(p->codes[p->cur]);
   p->cur++;
   while (strsame(p->codes[p->cur], "\n")) {
      p->cur++;
   }
   instrs(p);
}
/* ****************************************** */
void instrs(Prog *p)
{
   while (strsame(p->codes[p->cur], "\n")) {
      p->cur++;
   }
   if (strsame(p->codes[p->cur], "}")) {
      return;
   }
   instruct(p);
   p->cur++;
   instrs(p);
}
/* ****************************************** */
void instruct(Prog *p)
{
   while (strsame(p->codes[p->cur], "\n")) {
      p->cur++;
   }
   if (strsame(p->codes[p->cur], "FILE")) {
      file(p);
   } else if (strsame(p->codes[p->cur], "ABORT")) {
      abortHere(p);
   } else if (strsame(p->codes[p->cur], "IN2STR") || \
              strsame(p->codes[p->cur], "INNUM")) {
      input(p);
   } else if (strsame(p->codes[p->cur], "JUMP")) {
      jump(p);
   } else if (strsame(p->codes[p->cur], "PRINT") || \
              strsame(p->codes[p->cur], "PRINTN")) {
      print(p);
   } else if (strsame(p->codes[p->cur], "RND")) {
      rnd(p);
   } else if (strsame(p->codes[p->cur], "IFEQUAL") || \
              strsame(p->codes[p->cur], "IFGREATER")) {
      ifCond(p);
   } else if (strsame(p->codes[p->cur], "INC")) {
      inc(p);
   } else if (p->codes[p->cur][0] == '$' || \
              p->codes[p->cur][0] == '%'){
      set(p);
   } else {
      ERROR("Invalid instruction ! ");
   }
}
/* 解析关键字 FILE */
void file(Prog *p)
{
   int flag = NOPRINT;

   p->cur++;
   strcon(p, flag);
}
/* 解析关键字 ABORT */
void abortHere(Prog *p)
{
   if (!strsame(p->codes[p->cur], "ABORT")) {
      ERROR("Abort failed ! ");
   }
}
/* 解析关键字 IN2STR */
void input(Prog *p)
{
   int flag;

   if (strsame(p->codes[p->cur], "IN2STR")) {
      flag = 0; /* 当为关键字 IN2STR 时，flag为0 */
   } else {
      flag = 1; /* 当为关键字 INNUM 时，flag为1 */
   }
   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   if (flag == 0) {
      strvar(p);
      p->cur++;
      expComma(p->codes[p->cur]);
      p->cur++;
      strvar(p);
   }
   if (flag == 1) {
      numvar(p);
   }
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
/* 解析关键字 JUMP */
void jump(Prog *p)
{
   p->cur++;
   numcon(p);
}
/* 解析关键字 PRINT 或者  PRINTN*/
void print(Prog *p)
{
   int flag = NOPRINT;

   if (strsame(p->codes[p->cur], "PRINT")) {
      #ifdef INTERP
         flag = PRINT;
      #endif
      p->cur++;
      varcon(p, flag);
      #ifdef INTERP
         printf("\n");
      #endif
   } else if (strsame(p->codes[p->cur], "PRINTN")) {
      #ifdef INTERP
         flag = PRINT;
      #endif
      p->cur++;
      varcon(p,flag);
   }
}
/* 解析关键字 RND */
void rnd(Prog *p)
{
   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   numvar(p);
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
/* 解析关键字 IFEQUAL 或者 IFGREATER */
void ifCond(Prog *p)
{
   int flag = NOPRINT;

   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   varcon(p, flag);
   p->cur++;
   expComma(p->codes[p->cur]);
   p->cur++;
   varcon(p, flag);
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
   p->cur++;
   expLeftBracket(p->codes[p->cur]);
   p->cur++;
   instrs(p);
}
/* 解析关键字 INC */
void inc(Prog *p)
{
   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   numvar(p);
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
/* 解析关键字 SET */
void set(Prog *p)
{
   int flag = NOPRINT;

   if (p->codes[p->cur][0] == '$' ) {
      strvar(p);
   } else if (p->codes[p->cur][0] == '%' ) {
      numvar(p);
   }
   p->cur++;
   expEqualSign(p->codes[p->cur]);
   p->cur++;
   varcon(p, flag);
}
/************************************************************/
/* 解析 VARCON, 变量或常量 */
void varcon(Prog *p, int flag)
{
   if (p->codes[p->cur][0] == '$') {         /* 字符变量 */
      strvar(p);
   }
   else if (p->codes[p->cur][0] == '%') {    /* 数据变量 */
      numvar(p);
   }
   else if (p->codes[p->cur][0] == '"' || p->codes[p->cur][0] == '#') {
      strcon(p, flag);
   }
   else {
      numcon(p);
   }
}
/* 解析 STRVAR, 字符变量 */
void strvar(Prog *p)
{
   int len;

   if (p->codes[p->cur][0] != '$') {
      ERROR("Expected a $ , ");
   }
   len = (int)strlen(p->codes[p->cur]);
   if (len < 2) {
      ERROR("Illegal string variable expression, ");
   } else {
      isCapital(p->codes[p->cur], len);
   }
}
/* 解析 NUMVAR, 数据变量 */
void numvar(Prog *p)
{
   int len;

   if (p->codes[p->cur][0] != '%') {
      ERROR("Expected a % , ");
   }
   len = (int)strlen(p->codes[p->cur]);
   if (len < 2) {
      ERROR("Illegal number variable expression, ");
   } else {
      isCapital(p->codes[p->cur], len);
   }
}
/* 解析 STRCON, 字符常量 */
void strcon(Prog *p, int flag)
{
   if (p->codes[p->cur][0] != '"' && p->codes[p->cur][0] != '#') {
      ERROR("Expected a \" or # , ");
   }
   else if (p->codes[p->cur][0] == '"' ) {
      strcon1(p, flag);
   }
   else if (p->codes[p->cur][0] == '#') {
      strcon2(p, flag);
   }
   else {
      ERROR("Incorrect string constant representation ? ");
   }
}
/* 解析 STRCON, 字符常量, " " , flag 用来判断是否需要打印 */
void strcon1(Prog *p, int flag)
{
   int len = (int)strlen(p->codes[p->cur]);

   if (flag == PRINT) {
      printWord(p->codes[p->cur]);
   }
   if (p->codes[p->cur][len - 1] != '"') { /* 如果字符串长度超过一个单词才进行此判断 */
      do {
         p->cur++;
         /* 如果判断单词为换行符，则表示没有结束符 */
         if (strsame(p->codes[p->cur], "\n")) {
                ERROR("Expected a \" ? ");
         }
         len = (int)strlen(p->codes[p->cur]);
         if (flag == PRINT) {
            printWord(p->codes[p->cur]);
         }
      } while(p->codes[p->cur][len-1] != '"');
   }
}
/* 解析 STRCON, 字符常量, # # , flag 用来判断是否需要打印 */
void strcon2(Prog *p, int flag)
{
   int len = (int)strlen(p->codes[p->cur]);

   if (flag == PRINT) {
      rot2Plain(p->codes[p->cur]);
      printWord(p->codes[p->cur]);
   }
   if (p->codes[p->cur][len - 1] != '#') { /* 如果字符串长度超过一个单词才判断 */
      do {
         p->cur++;
         /* 如果判断单词为换行符，则表示没有结束符 */
         if (strsame(p->codes[p->cur], "\n")) {
                ERROR("Expected a # ? ");
         }
         len = (int)strlen(p->codes[p->cur]);
         if (flag == PRINT) {
            rot2Plain(p->codes[p->cur]);
            printWord(p->codes[p->cur]);
         }
      } while(p->codes[p->cur][len-1] != '#');
   }
}
/* 解析 NUMCON, 数据常量 */
void numcon(Prog *p)
{
   int i, len;
   char ch;

   len = (int)strlen(p->codes[p->cur]);
   for (i = 0; i < len; i++) {
      ch = p->codes[p->cur][i];
      if ((ch < '0' || ch > '9') && ch != '.') {
         ERROR("Incorrect number constant representation ? ");
      }
   }
}
/* 将rot13编码的字符串转换为原字符串 */
void rot2Plain(char *str)
{
   int i, len = (int)strlen(str);

   for (i = 0; i < len; i++) {
      if ((str[i] >= 'a' && str[i] <= 'm') || (str[i] >= 'A' && str[i] <= 'M')) {
         str[i] =  str[i] + 13;
      } else if ((str[i] >= 'n' && str[i] <= 'z') || (str[i] >= 'N' && str[i] <= 'Z')) {
         str[i] = str[i] - 13;
      } else if (str[i] >= '0' && str[i] <= '4') {
         str[i] = str[i] + 5;
      } else if (str[i] >= '5' && str[i] <= '9') {
         str[i] = str[i] - 5;
      }
   }

}
/* 打印单词 */
void printWord(char *str)
{
   int i, len = (int)strlen(str);

   for (i = 0; i < len; i++) {
      if (str[i] != '\"' && str[i] != '#') {
         fprintf(stdout, "%c", str[i]);
      }
   }
   fprintf(stdout," ");
}
/* 判断某字符串是否为全为大写字母 */
void isCapital(char *str, int len)
{
   int i;

   for (i = 1; i < len; i++) {
      if ( str[i] < 'A' || str[i] > 'Z') {
         ERROR("Illegal expression, make sure all the letters are capital! ");
      }
   }
}
/* 判断字符串字符为 （ */
void expLeftParenthesis(char *str)
{
   if (!strsame(str, "(") ){
      ERROR("Expected a \" ( \" , ");
   }
}
/* 判断字符串字符为  ） */
void expRightParenthesis(char *str)
{
   if (!strsame(str, ")") ){
      ERROR("Expected a \" ) \" , ");
   }
}
/* 判断字符串字符为 { */
void expLeftBracket(char *str)
{
   if (!strsame(str, "{") ){
      ERROR("Expected a \" { \" , ");
   }
}
/* 判断字符串字符为 } */
void expRightBracket(char *str)
{
   if (!strsame(str, "}") ){
      ERROR("Expected a \" } \" , ");
   }
}
/* 判断字符串字符为 , */
void expComma(char *str)
{
   if (!strsame(str, ",") ){
      ERROR("Expected a \" , \" , ");
   }
}
/* 判断字符串字符为 = */
void expEqualSign(char *str)
{
   if (!strsame(str, "=") ){
      ERROR("Expected a \" = \" , ");
   }
}
/************************************************************/

/* 将文件内容读入结构体指向的内存空间，若空间存满，则将空间扩大为原来的2倍 */
Prog *readIn(FILE *fp)
{
   Prog *p = progInit();
   int i = 0, j = 0;
   char c = ' ';

   while(c != EOF) {
      j = 0;
      do {
         ifResize(p, i);               /* 如果空间存满，则重新分配空间 */
         c = getc(fp);                 /* 存入可见字符以及空格和换行符*/
         if ((c >= '!' && c <= '~') || c == ' ' || c == '\n') {
            p->codes[i][j++] = c;
         }
      } while(c != ' ' && c != '\n' && c != EOF);
      if (j > 1) {                     /* 排除无用的空格和换行符 */
         p->codes[i][j-1] = '\0';      /* j在已经赋值的位置上+1 */
         i++;
         ifResize(p, i);               /* 如果空间存满，则重新分配空间 */
         if (c == '\n') {              /* 保存每条有效指令末尾的换行符 */
            strcpy(p->codes[i++], "\n");
         }
      }
   }
   return p;
}

/* 判断是否需要resize */
void ifResize(Prog *p, int num)
{
   p->cur = num;
   if (p->cur >= p->total) {
      resizeProg(p);
   }
}
/* 将存储空间大小扩大为原来的2倍 */
void resizeProg(Prog *p)
{
   int i, new_total;

   if (p == NULL) {
      return;
   }
   new_total = p->total * 2;
   p->codes = (char **)realloc(p->codes, new_total * sizeof(char *));
   check(p->codes);
   for (i = p->cur; i < new_total; i++) {
      p->codes[i] = rowInit();
   }
   p->total = new_total;
}

/* 初始化内存空间，初始空间可存代码数量为50 */
Prog *progInit(void)
{
   int i;
   Prog *p = (Prog *) calloc(1,sizeof(Prog));

   check(p);
   p->total = NUMTOKENS;
   p->codes = (char **)calloc(NUMTOKENS, sizeof(char *));
   for (i = 0; i < NUMTOKENS; i++) {
      p->codes[i] = rowInit();
   }
   return p;
}

/* 初始化存放每一个代码的空间 */
char *rowInit(void)
{
   char *row = (char *)calloc(MAXTOKENSIZE, sizeof(char));
   check(row);

   return row;
}

/* Check if memory allocation was successful */
void check(void *p)
{
   if(p == NULL){
      ON_ERROR("Creation Failed\n");
   }
}

/* open the file and determine if it's successful */
FILE *tryOpenFile(char *filename)
{
   FILE *fp;

   fp = fopen(filename, "r");
   if(fp == NULL) {
     ON_ERROR("Can not open the file!\n");
   }
   return fp;
}

/* 释放内存空间 */
void freeMemory(Prog **p)
{
   int i;

   for (i = 0; i < (*p)->total; i++) {
      free((*p)->codes[i]);
   }
   free((*p)->codes);
   free(*p);
   p = NULL;
}
/*test*/
/*
1、测试能否打开文件；
2、测试读入矩阵是否成功；
*/
