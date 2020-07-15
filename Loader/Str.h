//String
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//自定义运行参数
#define EXEC_SKIP	"0"	//直接运行，不需编译
#define EXEE_SKIP	"0"	//不需运行
#define EXEE_CONT	"1"	//运行完后不等待
#define EXEC_CDLD	'.'	//指向上一个目录

#define MAX_LEN	400
#define BAT_LEN	11
#define EXT_LEN	6
#define EXE_LEN 7
#define PKG_LEN	8

#define REFRESH(s)	printf("\r                                                                            \r%s",s);
#define SEGMENT		printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");

int getlen(char *s);
int getstr(char *s,int &t);
int catstr(char *d,char *s,int &t);
void get_nextval(const char *T,int next[]);

int KMP(const char *Text,const char* Pattern,int *next);
int extdel(char *s,int &t);
bool exec(char *s,int &t);
int findpkg(char *s,int &t);
int slash(char *s);

void catstr(char *dst,const char *src);
int cpystr(char *dst,const char *src);
void clrstr(char *str,int n);

bool equstr(char *dst,char *src);