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
   printf("Parsed OK !\n");
   freeMemory(&prog);
   return 0;
}
/* ****************************************** */
void program(Prog *p)
{
   p->current = 0;
   expLeftBracket(p->codes[p->current]);
   p->current++;
   instrs(p);
}
/* ****************************************** */
void instrs(Prog *p)
{
   if (strsame(p->codes[p->current], "}")) {
      return;
   }
   instruct(p);
   p->current++;
   instrs(p);
}
/* ****************************************** */
void instruct(Prog *p)
{
   if (strsame(p->codes[p->current], "FILE")) {
      file(p);
   } else if (strsame(p->codes[p->current], "ABORT")) {
      abortHere(p);
   } else if (strsame(p->codes[p->current], "IN2STR") || \
       strsame(p->codes[p->current], "INNUM")) {
      input(p);
   } else if (strsame(p->codes[p->current], "JUMP")) {
      jump(p);
   } else if (strsame(p->codes[p->current], "PRINT") || \
       strsame(p->codes[p->current], "PRINTN")) {
      print(p);
   } else if (strsame(p->codes[p->current], "RND")) {
      rnd(p);
   } else if (strsame(p->codes[p->current], "IFEQUAL") || \
       strsame(p->codes[p->current], "IFGREATER")) {
      ifCond(p);
   } else if (strsame(p->codes[p->current], "INC")) {
      inc(p);
   } else if (p->codes[p->current][0] == '$' || \
              p->codes[p->current][0] == '%'){
      set(p);
   } else {
      ERROR("Invalid instruction ! ");
   }
}
/* 解析关键字 FILE */
void file(Prog *p)
{
   p->current++;
   strcon(p);
}
/* 解析关键字 ABORT */
void abortHere(Prog *p)
{
   if (!strsame(p->codes[p->current], "ABORT")) {
      ERROR("Abort failed ! ");
   }
}
/* 解析关键字 IN2STR */
void input(Prog *p)
{
   int flag;

   if (strsame(p->codes[p->current], "IN2STR")) {
      flag = 0; /* 当为关键字 IN2STR 时，flag为0 */
   } else {
      flag = 1; /* 当为关键字 INNUM 时，flag为1 */
   }
   p->current++;
   expLeftParenthesis(p->codes[p->current]);
   p->current++;
   if (flag == 0) {
      strvar(p);
      p->current++;
      expComma(p->codes[p->current]);
      p->current++;
      strvar(p);
   }
   if (flag == 1) {
      numvar(p);
   }
   p->current++;
   expRightParenthesis(p->codes[p->current]);
}
/* 解析关键字 JUMP */
void jump(Prog *p)
{
   p->current++;
   numcon(p);
}
/* 解析关键字 PRINT 或者  PRINTN*/
void print(Prog *p)
{
   if (strsame(p->codes[p->current], "PRINT")) {
      p->current++;
      varcon(p);
   }
   if (strsame(p->codes[p->current], "PRINTN")) {
      p->current++;
      varcon(p);
   }
}
/* 解析关键字 RND */
void rnd(Prog *p)
{
   p->current++;
   expLeftParenthesis(p->codes[p->current]);
   p->current++;
   numvar(p);
   p->current++;
   expRightParenthesis(p->codes[p->current]);
}
/* 解析关键字 IFEQUAL 或者 IFGREATER */
void ifCond(Prog *p)
{
   p->current++;
   expLeftParenthesis(p->codes[p->current]);
   p->current++;
   varcon(p);
   p->current++;
   expComma(p->codes[p->current]);
   p->current++;
   varcon(p);
   p->current++;
   expRightParenthesis(p->codes[p->current]);
   p->current++;
   expLeftBracket(p->codes[p->current]);
   p->current++;
   instrs(p);
}
/* 解析关键字 INC */
void inc(Prog *p)
{
   p->current++;
   expLeftParenthesis(p->codes[p->current]);
   p->current++;
   numvar(p);
   p->current++;
   expRightParenthesis(p->codes[p->current]);
}
/* 解析关键字 SET */
void set(Prog *p)
{
   if (p->codes[p->current][0] == '$' ) {
      strvar(p);
   }
   if (p->codes[p->current][0] == '%' ) {
      numvar(p);
   }
   p->current++;
   expEqualSign(p->codes[p->current]);
   p->current++;
   varcon(p);
}
/************************************************************/
/* 解析 VARCON, 变量或常量 */
void varcon(Prog *p)
{
   if (p->codes[p->current][0] == '$') {         /* 字符变量 */
      strvar(p);
   }
   else if (p->codes[p->current][0] == '%') {    /* 数据变量 */
      numvar(p);
   }
   else if (p->codes[p->current][0] == '"' || p->codes[p->current][0] == '#') {
      strcon(p);
   }
   else {
      numcon(p);
   }
}
/* 解析 STRVAR, 字符变量 */
void strvar(Prog *p)
{
   int len;

   if (p->codes[p->current][0] != '$') {
      ERROR("Expected a $ , ");
   }
   len = (int)strlen(p->codes[p->current]);
   if (len < 2) {
      ERROR("Illegal string variable expression, ");
   } else {
      isCapital(p->codes[p->current], len);
   }
}
/* 解析 NUMVAR, 数据变量 */
void numvar(Prog *p)
{
   int len;

   if (p->codes[p->current][0] != '%') {
      ERROR("Expected a % , ");
   }
   len = (int)strlen(p->codes[p->current]);
   if (len < 2) {
      ERROR("Illegal number variable expression, ");
   } else {
      isCapital(p->codes[p->current], len);
   }
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
/* 解析 STRCON, 字符常量 */
void strcon(Prog *p)
{
   int len;

   if (p->codes[p->current][0] != '"' && p->codes[p->current][0] != '#') {
      ERROR("Expected a \" or # , ");
   }
   if (p->codes[p->current][0] == '"' ) {
      len = (int)strlen(p->codes[p->current]);
      if (p->codes[p->current][len - 1] != '"') {/* 如果字符串长度超过一个单词才判断 */
         do {
            p->current++;
            len = (int)strlen(p->codes[p->current]);
            if (isKeyword(p->codes[p->current]) || isSymbol(p->codes[p->current])) {
                   ERROR("Expected a \" ? ");
            }
         } while(p->codes[p->current][len-1] != '"');
      }
   }
   else if (p->codes[p->current][0] == '#') {
      len = (int)strlen(p->codes[p->current]);
      if (p->codes[p->current][len - 1] != '#') { /* 如果字符串长度超过一个单词才判断 */
         do {
            p->current++;
            len = (int)strlen(p->codes[p->current]);
            if (isKeyword(p->codes[p->current]) || isSymbol(p->codes[p->current])) {
                   ERROR("Expected a # ? ");
            }
         } while(p->codes[p->current][len-1] != '#');
      }
   }
   else {
      ERROR("Incorrect string constant representation ? ");
   }
}
/* 解析 NUMCON, 数据常量 */
void numcon(Prog *p)
{
   int i, len;
   char ch;

   len = (int)strlen(p->codes[p->current]);
   for (i = 0; i < len; i++) {
      ch = p->codes[p->current][i];
      if ((ch < '0' || ch > '9') && ch != '.') {
         ERROR("Incorrect number constant representation ? ");
      }
   }
}
/* 判断是否为关键字 */
int isKeyword(char *str)
{
   if (strsame(str, "FILE")   || strsame(str, "ABORT") /
       strsame(str, "INPUT")  || strsame(str, "IFCOND") /
       strsame(str, "INC")    || strsame(str, "SET") /
       strsame(str, "JUMP")   || strsame(str, "PRINT") /
       strsame(str, "PRINTN") || strsame(str, "RND") ) {
       return 1;
    }
    return 0;
}
/* 判断是否为特殊符号 */
int isSymbol(char *str)
{
   if (strsame(str, "}")) {
      return 1;
   }
   if (str[0] == '$' || str[0] == '%') {
      return 1;
   }
   return 0;
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
   int i = 0;

   while (fscanf(fp, "%s", p->codes[i++]) == 1) {
      p->current = i;
      if (p->current >= p->total) {
         resizeProg(p);
      }
   }
   return p;
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
   for (i = p->current; i < new_total; i++) {
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
