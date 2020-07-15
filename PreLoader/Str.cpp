//String
#include "Str.h"

int getlen(char *s)
{
	int i = 0;
	for (i=0;s[i]!='\0';i++);
	return i;
}

int getstr(char *s,int &t)
{
	char c;
	int i=0;
	int ch=0;
	while((c=getch())!=13)	//回车==13
	{
		if (c==8)	//退格==4
		{
			if (i!=0)
			{
				if (s[i-1]&0x80)	//中文
					s[--i]='\0';
				s[--i]='\0';
				REFRESH(s);
			}
			continue;
		}
		if (t+i+1>=MAX_LEN)
		{
			if (ch%2&&c&0x80&&s[i-1]&0x80)	//对最后一个字节进行处理，如果是中文前半部分，则删除掉
					s[i-1]='\0';
			REFRESH(s);
			break;
		}
		s[i++]=c;
		if (c&0x80)
			ch++;
		REFRESH(s);
	}
	printf("\n");
	if (t+i+1>=MAX_LEN)
	{
		printf("■超过最大字数限制\n");
		return -1;
	}
	t+=i;
	return 0;
}

int catstr(char *d,char *s,int &t)
{
	int i=0;
	while(s[i]!='\0')
	{
		if (t+i+1>=MAX_LEN)
			break;
		d[i++]=s[i];
	}
	if (t+i+1>=MAX_LEN)
	{
		printf("■超过最大字数限制\n");
		return -1;
	}
	t+=i;
	return 0;
}

int extdel(char *s,int &t)
{
	char ext[EXT_LEN]=".java";	//改变扩展名后请更改EXT_LEN
	int next[EXT_LEN];
	int r=-1;
	int i = 0;
	for (i=getlen(s)-1;i>0;i--)
	{
		r=KMP(s+i,ext,next);
		if (r!=-1)
			break;
	}

	if (r==-1)
		return -1;
	r+=i;
	for (i=0;i<EXT_LEN;i++)
		s[r+i]='\0';
	t=r;
	return 0;
}

bool exec(char *s,int &t)
{
	char ext[EXE_LEN]=".class";	//改变扩展名后请更改EXT_LEN
	int next[EXE_LEN];
	int r=-1;
	int i = 0;
	for (i=getlen(s)-1;i>0;i--)
	{
		r=KMP(s+i,ext,next);
		if (r!=-1)
			break;
	}

	if (r==-1)
		return false;
	r+=i;
	for (i=0;i<EXE_LEN;i++)
		s[r+i]='\0';
	t=r;
	return true;
}

int findpkg(char *s,int &t)
{
	char ext[PKG_LEN]="package";	//改变扩展名后请更改EXT_LEN
	int next[PKG_LEN];
	int r,i,l=getlen(s);
	for (i=0;i<l;i++)
	{
		r=KMP(s+i,ext,next);
		if (r!=-1)
			break;
	}

	if (r==-1)
		return -1;
	r+=i;
	t=r;
	return 0;
}

int slash(char *s)
{
	char slh[2]="\\";
	int next[2];
	int r=-1;
	int i = 0;
	for (i=getlen(s)-1;i>0;i--)
	{
		r=KMP(s+i,slh,next);
		if (r!=-1)
			break;
	}

	if (r==-1)
		return i;
	return r+i+1;
}

void get_nextval(const char *T,int next[])
{
	// 求模式串T的next函数值并存入数组 next。
	int j = 0, k = -1;
	next[0] = -1;
	
	while ( T[j/*+1*/] != '\0' )
	{
		if (k == -1 || T[j] == T[k])	
		{
			++j; ++k;
			if (T[j]!=T[k])	
				next[j] = k;
			else
				next[j] = next[k];
		}// if
		else
			k = next[k];
	}// while
}// get_nextval　

int KMP(const char *Text,const char* Pattern,int *next) 
{
	if( !Text||!Pattern||  Pattern[0]=='\0'  ||  Text[0]=='\0' )	
		return -1;//空指针或空串，返回-1。
	int len=0;
	const char * c=Pattern;
	while(*c++!='\0')//移动指针比移动下标快。
	{    
		++len;//字符串长度。
	}
	
	get_nextval(Pattern,next);//求Pattern的next函数值
	
	int index=0,i=0,j=0;
	while(Text[i]!='\0'  && Pattern[j]!='\0' )
	{
		if(Text[i]== Pattern[j])
		{
			++i;// 继续比较后继字符
			++j;
		}
		else
		{
			index += j-next[j];
			if(next[j]!=-1)
				j=next[j];// 模式串向右移动
			else
			{
				j=0;
				++i;
			}
		}
		
	}//while

	if(Pattern[j]=='\0')
		return index;// 匹配成功
	else
		return -1; 
}

void catstr(char *dst,const char *src)
{
	if (dst==NULL||src==NULL)
		return;
	int i,j;
	for (i=0;dst[i]!='\0';i++);
	for (j=0;src[j]!='\0';j++)
		dst[i+j]=src[j];
	dst[i+j]='\0';
	return;
}

int cpystr(char *dst,const char *src)
{
	if (dst==NULL)
		return 0;
	if (src==NULL)
	{
		dst[0]='\0';
		return 0;
	}
	int j;
	for (j=0;src[j]!='\0';j++)
		dst[j]=src[j];
	dst[j]='\0';
	return j;	
}

void clrstr(char *str,int n)
{
	if (str==NULL)
		return;
	int i;
	for (i=0;i<n&&str[i]!='\0';i++)
		str[i]='\0';
	return;
}

bool equstr(char *dst,char *src)
{
	if (dst==NULL&&src==NULL)
		return true;
	if (dst==NULL||src==NULL)
		return false;
	int sl,dl;
	sl=getlen(src);
	dl=getlen(dst);
	if (sl!=dl)
		return false;
	int i;
	for (i=0;src[i]!='\0'&&dst[i]!='\0';i++)
	{
		if (src[i]!=dst[i])
			return false;
	}
	return true;
}