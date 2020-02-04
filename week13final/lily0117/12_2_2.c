#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "nal.h"
#include "fmvm.h"

mvm *m;   /* 存放全局变量 */
Queue q;  /* 在if条件语句中匹配括号{ } */

char *search(mvm* m, char* key);
void insert(mvm *m, char *name, char *value);
/*************************************************************/
int main(int argc, char *argv[])
{
   FILE *fp;
   Prog *prog = NULL;

   if (argc < 2) {
     ON_ERROR("ERROR: Incorrect usage. Please try again.\n");
   }
   initialiseQueue(&q);          /* 初始化队列 */
   m = mvm_init();               /* 初始化数组，用以存储全局变量 */
   fp = tryOpenFile(argv[1]);
   prog = readIn(fp);
   fclose(fp);
   program(prog);
   #ifdef INTERP
   printf("Interpreted OK \n");
   #else
   printf("Parsed OK \n");
   #endif
   mvm_free(&m);
   freeMemory(&prog);
   return 0;
}
/********************** 读入文件内容 ***************************/
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
/* 读入文件，若空间存满，则将空间扩大为原来的2倍 */
Prog *readIn(FILE *fp)
{
   Prog *p = progInit();
   int i = 0, j = 0;
   char c = ' ';

   while(c != EOF) {
      j = 0;
      do {
         ifResize(p, i);               /* 若空间存满，则扩大为两倍 */
         c = getc(fp);
         if ((c >= '!' && c <= '~') || \
              c == ' ' || c == '\n') {
            p->codes[i][j++] = c;      /* 存入可见字符以及空格和换行符 */
         }
      } while(c != ' ' && c != '\n' && c != EOF);
      if (j > 1) {                     /* 排除无用的空格和换行符 */
         p->codes[i][j-1] = '\0';      /* j在已经赋值的位置上+1 */
         i++;
         ifResize(p, i);               /* 若空间存满，则扩大为两倍 */
         if (c == '\n') {              /* 保存每条有效指令末尾的换行符 */
            strcpy(p->codes[i++], "\n");
            ifResize(p, i);            /* 若空间存满，则扩大为两倍 */
         }
      }
   }
   p->actual = i - 1;                  /* 实际存储的行数 */
   return p;
}
/* 初始化空间 */
Prog *progInit(void)
{
   int i;
   Prog *p = (Prog *) calloc(1,sizeof(Prog));

   check(p);
   p->actual = 0;
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
/************************ 解析文件内容 ******************************/
/* the entrance of parser and interpter */
void program(Prog *p)
{
   p->cur = 0;
   expLeftBracket(p->codes[p->cur]);
   p->cur++;
   while (strsame(p->codes[p->cur], "\n")) { /* 跳过多余的换行符 */
      p->cur++;
   }
   instrs(p);
}
/* 执行指令 */
void instrs(Prog *p)
{
   while (strsame(p->codes[p->cur], "\n")) { /* 跳过多余的换行符 */
      p->cur++;
   }
   if (strsame(p->codes[p->cur], "")) {  /* 若等于空字符串，则表示代码执行结束，无} */
      expRightBracket(p->codes[p->cur]);
   }
   if (strsame(p->codes[p->cur], "}")) {
      return;
   }
   instruct(p);
   p->cur++;
   instrs(p);
}
/* 指令 */
void instruct(Prog *p)
{
   while (strsame(p->codes[p->cur], "\n")) {
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
      fprintf(stdout, "Error code: \"%s\"\n", p->codes[p->cur]);
      ERROR("Invalid instruction ! ");
   }
}
/* Parse key word - FILE */
void file(Prog *p)
{
   p->cur++;
   strcon(p);
   #ifdef INTERP
   executeFile(p->codes[p->cur]);
   #endif
   p->cur++;
}
/* execute the instructions in file */
void executeFile(char *file)
{
   int i, len = (int)strlen(file);
   FILE *f;
   char *file_name = (char *)calloc(len+END, sizeof(char));
   Prog *sub_prog = NULL;

   for (i = 1; i < len - 1; i++) {   /* read file name */
      file_name[i-1] = file[i];
   }
   file_name[i-1] = '\0';
   f = tryOpenFile(file_name);
   sub_prog = readIn(f);
   fclose(f);
   program(sub_prog);
   freeMemory(&sub_prog);
   free(file_name);
}
/* Parse key word - ABORT ******************有问题！！!!!!!!!！！！！！！**************/
void abortHere(void)
{
   #ifdef INTERP
   fprintf(stdout, "Interpreter OK\n");
   exit(EXIT_FAILURE);
   #endif
}
/* Parse key word - IN2STR */
void input(Prog *p)
{
   int flag = 0;  /* if key word = "IN2STR", flag = 0, otherwise flag = 1 */

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
      #ifdef INTERP
      inputAndInsert(p->codes[p->cur-2], flag);
      inputAndInsert(p->codes[p->cur], flag);
      #endif
   } else {
      flag = 1;
      numvar(p);
      #ifdef INTERP
      inputAndInsert(p->codes[p->cur], flag);
      #endif
   }
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
/* enter the contents corresponding to the variable（name）,
   decode them and delete " and #, then insert them to m */
void inputAndInsert(char *name, int flag)
{
   char input_str[INPUTSTRLEN];
   int i, repeat = 0;

   if (flag == 0) {
      scanf("%s", input_str);
      rot2Plain(input_str);               /* decode */
      deleteSign(input_str);              /* delete " " or # # */
   } else {
      do {                 /* 判断输入的是否为数字，若不是，则重新输入 */
         repeat = 0;
         scanf("%s", input_str);
          for (i = 0; i < (int)strlen(input_str); i++) {
             if (input_str[i] < '0' || input_str[i] > '9') {
                repeat = 1;
             }
          }
         fprintf(stdout, "please input a number:\n");
      } while (repeat != 0);
   }
   insert(m, name, input_str);
}
/* Parse key word - JUMP */
void jump(Prog *p)
{
   #ifdef INTERP
   int i, number, total = 0;
   #endif
   p->cur++;
   numcon(p);

   #ifdef INTERP
   for (i = 0; i < (int)strlen(p->codes[p->cur]); i++) {   /* read jump number */
      number = p->codes[p->cur][i] - '0';
      total = total * 10 + number;
   }
   printf("total = %d, p->actual = %d\n",total, p->actual);
   if (total > p->actual) {
      ERROR("Jump number exceed the actual number of code. \n");
   }
   p->cur = jumpPosition(p, total);
   instrs(p);
   #endif
}
/* find jump position */
int jumpPosition(Prog *p, int number)
{
   int i = 0, len, cnt = 0, position;
   while (cnt <= number) {
      if (p->codes[i][0] == '\"') {      /* things in quotes count as one word*/
         len = (int)strlen(p->codes[i]);
         while(p->codes[i][len-1] != '\"') {
            i++;
            len = (int)strlen(p->codes[i]);
         }
         i++;  /* 退出循环时，i在后引号所在的位置,故需先加1再赋值 */
         position = i;
         cnt++;
      } else if (!strsame(p->codes[i], "\n")) {
         position = i;
         i++;
         cnt++;
      } else {
         i++;
      }
   }
   return position; /* 返回需跳转到的位置 */
}
/* Parse key word - PRINT or PRINTN*/
void print(Prog *p)
{
   int flag = 0; /* flag = 0,不打印换行符，flag = 1，打印换行符 */
   #ifdef INTERP
   char *str = NULL;
   #endif

   if (strsame(p->codes[p->cur], "PRINT")) {
      flag = 1;
   }
   p->cur++;
   #ifdef INTERP
   if (p->codes[p->cur][0] == '$' || p->codes[p->cur][0] == '%') {
      str = search(m, p->codes[p->cur]);  /* 找到对应的字符串，由于插入时已经解码并删去了符号，故直接打印即可 */
      printf("%s", str);
      free(str);
   } else if (p->codes[p->cur][0] == '"' || p->codes[p->cur][0] == '#') {
      str = strFromStrcon(p); /* 读出“”中或##中的字符串并解码，删去符号输出 */
      printf("%s", str);
      free(str);
   } else { /* 否则为常量，直接打印 */
      printf("%s", p->codes[p->cur]);
   }
   if (flag == 1) {
      printf("\n");
   }
   #endif
   varcon(p);
}
/* √  Parse key word - RND */
void rnd(Prog *p)
{
   #ifdef INTERP
   int num;
   char *str = NULL;
   num = rand() % 100; /* 产生0-99的随机数 */
   #endif

   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   numvar(p);
   #ifdef INTERP
   str = num2String(num);
   insert(m, p->codes[p->cur], str);
   free(str);
   #endif
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
/*  Parse key word - IFEQUAL 或者 IFGREATER */
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
   int cmp_result1, cmp_result2, bracket_num;
   char *cmp_str1 = NULL, *cmp_str2 = NULL;

   insertQueue('{', &q);  /* 遇到比较语句就入队1个 { */
   cmp_str1 = parseString(p->codes[p->cur-5]); /* p->cur-5为第一个需要比较的内容 */
   cmp_str2 = parseString(p->codes[p->cur-3]); /* p->cur-3为第二个需要比较的内容 */
   cmp_result1 = strcmp(cmp_str1, cmp_str2);
   cmp_result2 = (int)string2Num(cmp_str1) - (int)string2Num(cmp_str2);
   if ((strsame(p->codes[p->cur-7], "IFEQUAL") && cmp_result1 == 0) || \
       (strsame(p->codes[p->cur-7], "IFGREATER" ) && cmp_result2 > 0)) {
      instrs(p);
   }
   bracket_num = q.back - q.front - 1; /* 执行完条件语句或条件不满足时，需要跳转到下一个 } 之后的位置 */
   while (q.back - q.front != bracket_num) {
     while (!strsame(p->codes[p->cur], "}")) {
         if (strsame(p->codes[p->cur], "{")) {
            insertQueue('{', &q);/* 处理嵌套循环, 遇到比较的关键词，则入队一个 { */
         }
         p->cur++;
     }
     removeQueue(&q);  /* 一旦遇到 } , 则出队一个 { ,保证括号的匹配 */
     p->cur++; /* 跳到 } 之后的位置 */
   }
   free(cmp_str1);
   free(cmp_str2);
}
/* 处理字符串，若为变量，返回变量代表的值，同时解码及去掉符号 */
char *parseString(char *str)
{
   char *parse_str = (char *)calloc((int)strlen(str)+END, sizeof(char));
   char *temp;

   strcpy(parse_str, str);
   if (str[0] == '$' || str[0] == '%') {
      temp = search(m, str);
      parse_str = (char *)realloc(parse_str, ((int)strlen(temp)+END) * sizeof(char));
      strcpy(parse_str, temp);
      free(temp);
   }
   rot2Plain(parse_str);
   deleteSign(parse_str);
   return parse_str;
}
/* √  Parse key word - INC */
void inc(Prog *p)
{
   p->cur++;
   expLeftParenthesis(p->codes[p->cur]);
   p->cur++;
   numvar(p);
   #ifdef INTERP
   addOne(p->codes[p->cur]);  /* 数据变量对应的值加1 */
   #endif
   p->cur++;
   expRightParenthesis(p->codes[p->cur]);
}
/* 数据变量对应的值加1 */
void addOne(char *num_name)
{
   int number;
   char *str1 = NULL, *str2 = NULL, *temp = NULL;

   temp = mvm_search(m, num_name);
   if (temp != NULL) {
      str1 = (char *)calloc(strlen(temp)+END, sizeof(char));
      strcpy(str1, temp);
      number = string2Num(str1);
      number++;
      str2 = num2String(number);
      insert(m, num_name, str2);
      free(str1);
      free(str2);
   }
}
/* √  Parse key word - SET */
void set(Prog *p)
{
   if (p->codes[p->cur][0] == '$' ) {
      strvar(p);
      p->cur++;
      expEqualSign(p->codes[p->cur]);
      p->cur++;
      #ifdef INTERP
      setInsert(p);
      #endif
      if (p->codes[p->cur][0] == '$' ) {
         strvar(p);
      } else {
         strcon(p);
      }
   } else if (p->codes[p->cur][0] == '%' ) {
      numvar(p);
      p->cur++;
      expEqualSign(p->codes[p->cur]);
      p->cur++;
      #ifdef INTERP
      setInsert(p);
      #endif
      if (p->codes[p->cur][0] == '%' ) {
         numvar(p);
      } else {
         numcon(p);
      }
   }
}
/* 将set的值插入全局数组中 */
void setInsert(Prog *p)
{
   char *value = NULL;

   if (p->codes[p->cur-2][0] == '$') { /* 给字符串变量赋值 */
      value = strFromStrcon(p);
      insert(m, p->codes[p->cur-2], value);
      free(value);
   } else { /* 否则给数据变量赋值 */
      insert(m, p->codes[p->cur-2], p->codes[p->cur]);
   }
}
/************************* 工具函数 ******************************/
/* 查找变量对应的值, 注意有可能变量对应的还是变量，故需要查找到常量为止 */
char *search(mvm* m, char* key)
{
   char *str = NULL, *data = NULL;

   do {
      str = mvm_search(m, key);
      if (str != NULL) {
         data = (char *)calloc((int)strlen(str)+END, sizeof(char));
         strcpy(data, str);
         key = data;
      }
   } while (str != NULL);
   return data;
}
/* 若name在全局变量列表中已经存在，删去后插入新值 */
void insert(mvm *m, char *name, char *value)
{
   if (mvm_search(m, name) != NULL) {
     mvm_delete(m, name);
   }
   mvm_insert(m, name, value);
}
/* 将数字转换为字符串类型 */
char *num2String(int number)
{
   char *str = (char *)calloc(INPUTNUMLEN, sizeof(char));

   sprintf(str, "%d", number);
   return str;
}
/* 将字符串转换为数字 */
int string2Num(char *str)
{
   return atoi(str);
}
/* 获取字符串常量，将”“和##中的内容复制到一个字符串中,解码，去掉符号后返回 */
char *strFromStrcon(Prog *p)
{
   int len = (int)strlen(p->codes[p->cur]), temp_len, count = 0;
   char *str = (char *)calloc(len+END, sizeof(char));

   strcpy(str, p->codes[p->cur]);
   if (p->codes[p->cur][0] == '"' || p->codes[p->cur][0] == '#') {
      while(p->codes[p->cur][len-1] != '"' && p->codes[p->cur][len-1] != '#') {
         temp_len = strlen(str) + 1;  /* +1用来存空格 */
         p->cur++;
         count++;
         len = (int)strlen(p->codes[p->cur]);
         str = (char *)realloc(str, (len+END + temp_len)*sizeof(char));
         strcat(str, " ");
         strcat(str, p->codes[p->cur]);
      }
      p->cur = p->cur - count; /* 减去增加的位置 */
   }
   rot2Plain(str);
   deleteSign(str);
   return str;
}
/* 将编码字符串转换为原字符串 */
void rot2Plain(char *str)
{
   int i, len = (int)strlen(str);
   if (str[0] == '#') {
      for (i = 0; i < len; i++) {
         if ((str[i] >= 'a' && str[i] <= 'm') || \
             (str[i] >= 'A' && str[i] <= 'M')) {
            str[i] =  str[i] + LETTERENCODE;
         }
         else if ((str[i] >= 'n' && str[i] <= 'z') || \
                 (str[i] >= 'N' && str[i] <= 'Z')) {
            str[i] = str[i] - LETTERENCODE;
         }
         else if (str[i] >= '0' && str[i] <= '4') {
            str[i] = str[i] + NUMBERENCODE;
         }
         else if (str[i] >= '5' && str[i] <= '9') {
            str[i] = str[i] - NUMBERENCODE;
         }
      }
   }
}
/* 去掉字符串的 “ ”, # # */
void deleteSign(char *str)
{
   int i, j = 0, len;
   char *new_str = NULL;

   if (str[0] == '"' || str[0] == '#') {
      len = (int)strlen(str);
      new_str = (char *)calloc(len+END, sizeof(char));
      for (i = 1; i < len - 1; i++) {
         new_str[j++] = str[i];
      }
      new_str[j] = '\0';
      strcpy(str, new_str);
      free(new_str);
   }
}
/************************* 变量或常量 ****************************/
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
      ERROR("Expected a \" $ \" , ");
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
      ERROR("Expected a \" % \" , ");
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
/* 解析 STRCON, 字符常量, " " */
void strcon1(Prog *p)
{
   int len = (int)strlen(p->codes[p->cur]);

   if (p->codes[p->cur][len - 1] != '"') { /* 如果字符串长度超过一个单词才进行此判断 */
      do {
         p->cur++;
         /* 如果判断单词为换行符，则表示字符串结尾缺少引号 */
         if (strsame(p->codes[p->cur], "\n")) {
                ERROR("Expected a \" , ");
         }
         len = (int)strlen(p->codes[p->cur]);
      } while(p->codes[p->cur][len-1] != '"');
      p->cur++;
   }
}
/* 解析 STRCON, 字符常量, # # */
void strcon2(Prog *p)
{
   int len = (int)strlen(p->codes[p->cur]);

   if (p->codes[p->cur][len - 1] != '#') { /* 如果字符串长度超过一个单词才判断 */
      do {
         p->cur++;
         /* 如果判断单词为换行符，则表示字符串结尾缺少# */
         if (strsame(p->codes[p->cur], "\n")) {
                ERROR("Expected a \" # \" , ");
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
         printf("Error code: \"%s\"\n", p->codes[p->cur]);
         ERROR("Incorrect number constant representation ? ");
      }
   }
}
/********************* 判断字符是否符合标准 **********************/
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
/************************ queue ***************************/
/* 初始化队列 */
void initialiseQueue(Queue *q)
{
   q->front = 0;
   q->back = 0;
}
/* 出队 */
char removeQueue(Queue *q)
{
   char c;
   c = q->n[q->front];
   q->front = (q->front + 1) % MAX_QUEUE;
   return c;
}
/* 入队 */
void insertQueue(char c, Queue *q)
{
   q->n[q->back] = c;
   q->back = (q->back + 1) % MAX_QUEUE;
}
/************************ free ***************************/
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
