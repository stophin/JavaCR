//String
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//�Զ������в���
#define EXEC_SKIP	"0"	//ֱ�����У��������
#define EXEE_SKIP	"0"	//��������
#define EXEE_CONT	"1"	//������󲻵ȴ�
#define EXEC_CDLD	'.'	//ָ����һ��Ŀ¼

#define MAX_LEN	400
#define BAT_LEN	11
#define EXT_LEN	6
#define EXE_LEN 7
#define PKG_LEN	8

#define REFRESH(s)	printf("\r                                                                            \r%s",s);
#define SEGMENT		printf("������������������������������������������������������������������������������\n");

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