/* 用fscanf读取文件内容，所以必须用空格，且空格和换行符处理一样 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "12.h"
#include "fmvm.h"

mvm *m;
Queue q;

char *search(mvm* m, char* key);
/* ****************************************** */
int main(int argc, char *argv[])
{
   FILE *fp;
   Prog *prog = NULL;

   if (argc < 2) {
     ON_ERROR("ERROR: Incorrect usage. Please try again.\n");
   }
   initialiseQueue(&q);
   srand((unsigned)time(NULL));  /* 随机数种子, 放在主函数中保证每次生成的随机数不一样 */
   m = mvm_init();               /* 初始化数组，用以存储全局变量 */
   fp = tryOpenFile(argv[1]);
   prog = readIn(fp);
   fclose(fp);
   program(prog);
   #ifndef INTERP                /* 如果没有 -DINTERP，则为parse */
   printf("Parsed OK \n");
   #endif
   #ifdef INTERP                 /* 如果有 -DINTERP，则为interpret */
   printf("Interpreted OK \n");
   #endif
   mvm_free(&m);
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
   while (strsame(p->codes[p->cur], "\n") || strsame(p->codes[p->cur], " ")) {
      p->cur++;
   }
   if (strsame(p->codes[p->cur], "FILE")) {
      file(p);
   } else if (strsame(p->codes[p->cur], "ABORT")) {
      abortHere();
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
/* √ 解析关键字 FILE */
void file(Prog *p)
{
   p->cur++;
   strcon(p);
   #ifdef INTERP  /* 如果有 -DINTERP，则为interpret */
   executeFile(p->codes[p->cur]);
   #endif
   p->cur++;
}
/* √ 解析关键字 ABORT */
void abortHere(void)
{
   #ifdef INTERP             /* 如果有 -DINTERP，则为interpret */
   exit(EXIT_FAILURE);
   #endif
}
/* √ 解析关键字 IN2STR */
void input(Prog *p)
{
   int flag = 0;

   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   if (strsame(p->codes[p->cur-2], "IN2STR")) {
      flag = 0;
      strvar(p);
      p->cur++;
      expComma(p->codes[p->cur]);
      p->cur++;
      strvar(p);
      #ifdef INTERP                     /* 如果有 -DINTERP，则为interpret */
      inputAndInsert(p->codes[p->cur-2], flag);
      inputAndInsert(p->codes[p->cur], flag);
      #endif
   } else {
      flag = 1;
      numvar(p);
      #ifdef INTERP                     /* 如果有 -DINTERP，则为interpret */
      inputAndInsert(p->codes[p->cur], flag);
      #endif
   }
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
void inputAndInsert(char *name, int flag)
{
   double input_number;
   char input_str[20];

   if (mvm_search(m, name) != NULL) {
      mvm_delete(m, name);
   }
   if (flag == 0) {
      scanf("%s", input_str);

      mvm_insert(m, name, input_str); /* -2找到对应的变量名 */
   } else {
      scanf("%lf", &input_number); /* 加判断，若输入的不是数字则重新输入*/

      mvm_insert(m, name, num2String(input_number));
   }
}
/* √ 解析关键字 JUMP */
void jump(Prog *p)
{
   #ifdef INTERP
   int i, num, number = 0;
   #endif
   p->cur++;
   numcon(p);

   #ifdef INTERP              /* 如果有 -DINTERP，则为interpret */
   for (i = 0; i < (int)strlen(p->codes[p->cur]); i++) {   /* 读取需要跳转的数字 */
      num = p->codes[p->cur][i] - '0';
      number = number * 10 + num;
   }
   p->cur = jumpPosition(p, number);
   instrs(p);

   #endif
}
/* √ 解析关键字 PRINT 或者 PRINTN*/
void print(Prog *p)
{
   int flag = 0;
   #ifdef INTERP
   char *str;
   #endif

   if (strsame(p->codes[p->cur], "PRINT")) {
      flag = 1;
   }
   p->cur++;
   #ifdef INTERP
   if (p->codes[p->cur][0] == '$') {
      printf("%s", search(m, p->codes[p->cur]));
   } else if (p->codes[p->cur][0] == '%') {
      printf("%d", (int)string2Num(search(m, p->codes[p->cur]))); /*！！！！！！！！转为了整型，修改*/
   } else if (p->codes[p->cur][0] == '"' || p->codes[p->cur][0] == '#') {
      str = strFromStrcon(p);
      deleteSign(str);
      printf("%s", str);
   } else { /* 否则为常量 */
      printf("%s", p->codes[p->cur]);
   }
   if (flag == 1) {
      printf("\n");
   }
   #endif
   varcon(p);
}
/* √ 解析关键字 RND */
void rnd(Prog *p)
{
   #ifdef INTERP             /* 如果有 -DINTERP，则为interpret */
   int num;
   num = rand() % 100;
   #endif

   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   numvar(p);
   #ifdef INTERP             /* 如果有 -DINTERP，则为interpret */
   if (mvm_search(m, p->codes[p->cur]) != NULL) {
      mvm_delete(m, p->codes[p->cur]); /* 一个变量只对应一个数据，故需删去之前的数据 */
   }
   mvm_insert(m, p->codes[p->cur], num2String(num));
   #endif
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
/* 解析关键字 IFEQUAL 或者 IFGREATER */
void ifCond(Prog *p)
{
   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   varcon(p);
   p->cur++;
   expComma(p->codes[p->cur]);
   p->cur++;
   varcon(p);
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
   p->cur++;
   expLeftBracket(p->codes[p->cur]);
   p->cur++;
   #ifdef INTERP
   compare(p);
   #endif
   #ifndef INTERP
   instrs(p);
   #endif
}
void compare(Prog *p)
{
   int cmp_result1, cmp_result2, number, flag = 0;
   char *cmp_str1, *cmp_str2;

   cmp_str1 = parseString(p->codes[p->cur-5]);
   cmp_str2 = parseString(p->codes[p->cur-3]);
   cmp_result1 = strcmp(cmp_str1, cmp_str2);
   cmp_result2 = (int)string2Num(cmp_str1) - (int)string2Num(cmp_str2);
   insertQueue('{', &q);
   if (strsame(p->codes[p->cur-7], "IFEQUAL") && cmp_result1 == 0) {
      instrs(p);
   } else if (strsame(p->codes[p->cur-7], "IFGREATER" ) && cmp_result2 > 0) {
      instrs(p);
   }
   number = q.back - q.front - 1;
      while (q.back - q.front != number) {
         while (!strsame(p->codes[p->cur], "}")) {
            if (strsame(p->codes[p->cur], "IFEQUAL") || strsame(p->codes[p->cur], "IFGREATER")) {
               insertQueue('{', &q);
               flag = 1;

            }
            p->cur++;
         }
         p->cur++;
         removeQueue(&q);
      }
}
/* 处理字符串，若为变量，返回变量代表的值，同时解码及去掉 “ ” 及 # # */
char *parseString(char *str)
{
   char *parse_str = (char *)calloc(strlen(str)+1, sizeof(char));

   strcpy(parse_str, str);
   if (str[0] == '$' || str[0] == '%') {
     strcpy(parse_str, mvm_search(m, str));
   }
   rot2Plain(parse_str);
   deleteSign(parse_str);
   return parse_str;
}
/* √ 解析关键字 INC */
void inc(Prog *p)
{
   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   numvar(p);
   #ifdef INTERP              /* 如果有 -DINTERP，则为interpret */
   addOne(p->codes[p->cur]);  /* 数据变量对应的值加1 */
   #endif
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
/* √ 解析关键字 SET */
void set(Prog *p)
{
   if (p->codes[p->cur][0] == '$' ) {
      strvar(p);
   } else if (p->codes[p->cur][0] == '%' ) {
      numvar(p);
   }
   p->cur++;
   expEqualSign(p->codes[p->cur]);
   p->cur++;
   #ifdef INTERP             /* 如果有 -DINTERP，则为interpret */
   setInsert(p);
   #endif
   varcon(p);
}
/* 将set的值插入全局数组中 */
void setInsert(Prog *p)
{
   char *name = NULL, *value = NULL;

   name = (char *)calloc(strlen(p->codes[p->cur-2])+1, sizeof(char));
   strcpy(name, p->codes[p->cur-2]);
   if (p->codes[p->cur-2][0] == '$') {
      value = (char *)calloc(strlen(strFromStrcon(p))+1, sizeof(char));
      rot2Plain(value);
      strcpy(value, strFromStrcon(p));
   } else {
      value = (char *)calloc(strlen(p->codes[p->cur])+1, sizeof(char));
      strcpy(value, p->codes[p->cur]);
   }
   if (mvm_search(m, name) != NULL) {
     mvm_delete(m, name);
   }
   mvm_insert(m, name, value);
}
/************************************************************/
/* 解析 VARCON, 变量或常量 */
void varcon(Prog *p)
{
   if (p->codes[p->cur][0] == '$') {         /* 字符变量 */
      strvar(p);
   }
   else if (p->codes[p->cur][0] == '%') {    /* 数据变量 */
      numvar(p);
   }
   else if (p->codes[p->cur][0] == '"' || p->codes[p->cur][0] == '#') {
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
void strcon(Prog *p)
{
   if (p->codes[p->cur][0] != '"' && p->codes[p->cur][0] != '#') {
      ERROR("Expected a \" or # , ");
   } else if (p->codes[p->cur][0] == '"') {
      strcon1(p);
   } else if (p->codes[p->cur][0] == '#') {
      strcon2(p);
   } else {
      ERROR("Incorrect string constant representation ? ");
   }
}
/* 解析 STRCON, 字符常量, " " , flag 用来判断是否需要打印 */
void strcon1(Prog *p)
{
   int len = (int)strlen(p->codes[p->cur]);

   if (p->codes[p->cur][len - 1] != '"') { /* 如果字符串长度超过一个单词才进行此判断 */
      do {
         p->cur++;
         /* 如果判断单词为换行符，则表示没有结束符 */
         if (strsame(p->codes[p->cur], "\n")) {
                ERROR("Expected a \" ? ");
         }
         len = (int)strlen(p->codes[p->cur]);
      } while(p->codes[p->cur][len-1] != '"');
      p->cur++;
   }

}
/* 解析 STRCON, 字符常量, # # , flag 用来判断是否需要打印 */
void strcon2(Prog *p)
{
   int len = (int)strlen(p->codes[p->cur]);

   if (p->codes[p->cur][len - 1] != '#') { /* 如果字符串长度超过一个单词才判断 */
      do {
         p->cur++;
         /* 如果判断单词为换行符，则表示没有结束符 */
         if (strsame(p->codes[p->cur], "\n")) {
                ERROR("Expected a # ? ");
         }
         len = (int)strlen(p->codes[p->cur]);
      } while(p->codes[p->cur][len-1] != '#');
      p->cur++;
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
/************************************************************/
/* execute the instructions in file */
void executeFile(char *file)
{
   int i, len = (int)strlen(file);
   FILE *f;
   char *file_name = (char *)calloc(len+1, sizeof(char));
   Prog *sub_prog = NULL;

   for (i = 1; i < len - 1; i++) {
      file_name[i-1] = file[i];
   }
   file_name[i-1] = '\0';
   f = tryOpenFile(file_name);
   sub_prog = readIn(f);
   fclose(f);
   program(sub_prog);
   free(sub_prog);
}
/* 数据变量对应的值加1 */
void addOne(char *num_name)
{
   int number;
   char *str = (char *)calloc(strlen(mvm_search(m, num_name))+1, sizeof(char));

   if (mvm_search(m, num_name) != NULL) {
      strcpy(str, mvm_search(m, num_name));
      number = string2Num(str);
      number++;
      str = num2String(number);
      mvm_delete(m, num_name);
      mvm_insert(m, num_name, str);
   }
}
/* 查找变量对应的值, 注意有可能变量对应的还是变量，故需要查找到常量为止 */
char *search(mvm* m, char* key)
{
   char *str = NULL, *data = NULL;

   do {
      str = mvm_search(m, key);
      if (str != NULL) {
         data = (char *)calloc(strlen(mvm_search(m, key))+1, sizeof(char));
         strcpy(data, mvm_search(m, key));
         key = data;
      }
   } while (str != NULL);
   return data;
}
/* 找到跳转的位置 */
int jumpPosition(Prog *p, int number)
{
   int i = 0, len, cnt = 0, position;
   while (cnt <= number) {
      if (p->codes[i][0] == '\"') {    /* 如果是字符串，则整个引号内视为一个 */
         len = (int)strlen(p->codes[i]);
         while(p->codes[i][len-1] != '\"') {
            i++;
            len = (int)strlen(p->codes[i]);
         } /* 退出循环时，i在后引号所在的位置 */
         i++;
         cnt++;
         position = i;
      } else if (!strsame(p->codes[i], "\n")) {
         position = i;
         i++;
         cnt++;
      } else {
         i++;
      }
   }
   return position; /* 返回跳转的位置 */
}
/* 将数字转换为字符串类型 */
char *num2String(double number)
{
   int i, flag = 0;
   char *str = (char *)calloc(20, sizeof(char));

   sprintf(str, "%lf", number);
   for (i = 0; i < (int)strlen(str); i++) {
      if (str[i] == '.') {
         flag = 1;
      }
      if (flag == 1 && str[i] == '0') {
         str[i] = '\0';
         if (str[i-1] == '.') {
            str[i-1] = '\0';
         }
      }
   }
   return str;
}
/* 将字符串转换为数字 */
double string2Num(char *str)
{
   return atof(str);
}
/*  */
char *strFromStrcon(Prog *p)
{
   int len = (int)strlen(p->codes[p->cur]), temp_len, count = 0;
   char *str = (char *)calloc(len+1, sizeof(char));

   strcpy(str, p->codes[p->cur]);
   while(p->codes[p->cur][len-1] != '"' && p->codes[p->cur][len-1] != '#') {
      temp_len = strlen(str);
      p->cur++;
      count++;
      len = (int)strlen(p->codes[p->cur]);
      str = (char *)realloc(str, (len + temp_len)*sizeof(char));
      strcat(str, " ");
      strcat(str, p->codes[p->cur]);
   }
   if (p->codes[p->cur][len-1] == '#') { /* 解码 */
      rot2Plain(str);
   }
   p->cur = p->cur - count; /* 增加的都减去 */
   return str;
}
/* 将rot13编码的字符串转换为原字符串 */
void rot2Plain(char *str)
{
   int i, len = (int)strlen(str);
   if (str[0] == '#') {
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
}
/* 去掉字符串的 “ ”, # # */
void deleteSign(char *str)
{
   int i, j = 0, len, start = 0, end;
   char *new_str = NULL;

   len = (int)strlen(str);
   end = len;
   new_str = (char *)calloc(len+1, sizeof(char));
   if (str[0] == '\"' || str[0] == '#') {
      start = 1;
   }
   if (str[len-1] == '\"' || str[len-1] == '#') {
      end = len-1;
   }
   for (i = start; i < end; i++) {
      new_str[j++] = str[i];
   }
   new_str[j] = '\0';
   strcpy(str, new_str);
}
/************************************************************/
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
/* 判断字符串字符为 （  */
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
         c = getc(fp);                 /* 存入可见字符以及空格和换行符 */
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
            ifResize(p, i);            /* 如果空间存满，则重新分配空间 */
         }
      }
   }
   return p;
}
/* 判断是否需要resize，若需要，则将存储空间大小扩大为原来的2倍 */
void ifResize(Prog *p, int num)
{
   int i, new_total;

   if (p == NULL) {
      return;
   }
   p->cur = num;
   if (p->cur >= p->total) {
      new_total = p->total * 2;
      p->codes = (char **)realloc(p->codes, new_total * sizeof(char *));
      check(p->codes);
      for (i = p->cur; i < new_total; i++) {
         p->codes[i] = rowInit();
      }
      p->total = new_total;
   }
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
/*******************************************************************/
void initialiseQueue(Queue *q)
{
   q->front = 0;
   q->back = 0;
}
/***************************************/
char removeQueue(Queue *q)
{
   char c;
   c = q->n[q->front];
   q->front = (q->front + 1) % MAX_QUEUE;
   return c;
}
/***************************************/
void insertQueue(char c, Queue *q)
{
   q->n[q->back] = c;
   q->back = (q->back + 1) % MAX_QUEUE;
}
/***************************************/
int emptyQueue(Queue q)
{
   if (q.front == q.back) {
      return 1;
   }
   return 0;
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
/* test */
/*
1、测试能否打开文件；
2、测试读入矩阵是否成功；
*/
