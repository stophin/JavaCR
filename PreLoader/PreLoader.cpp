//Java Complier PreLoader
#include "Str.h"

int argProc(int argc,char *argv[],char *s)
{
	if (argc==1)
	{
		int i=0;
		printf("■请输入要编译运行的文件名:\n");
		s[i++]='\"';
		if (getstr(s+i,i)==-1)
			return -1;
		//拖进窗口时一开始会有"的话就去掉
		if (s[1]=='\"')
		{
			s[1]='\0';
			catstr(s,s+2);
		}
		if (!exec(s,i))
		{
			extdel(s,i);	//删除扩展名(如果输入了的话)
			s[i++]='\"';
			s[i++]=' ';

			printf("■编译参数:\n");
			s[i++]='\"';
			if (getstr(s+i,i)==-1)
				return -1;
		}
		else
		{
			s[i++]='\"';
			s[i++]=' ';
			s[i++]='\"';
			s[i++]='0';
		}

		s[i++]='\"';
		s[i++]=' ';

		printf("■运行参数:\n");
		s[i++]='\"';
		if (getstr(s+i,i)==-1)
			return -1;
		s[i++]='\"';
		s[i]='\0';
		return 1;
	}
	if (argc==2)
	{
		int i=0;

		s[i++]='\"';
		if (catstr(s+i,argv[1],i)==-1)	//复制传递参数
			return -2;
		if (!exec(s,i))
		{
			if (extdel(s,i)==-1)	//删除扩展名
				return -2;
			s[i++]='\"';
			s[i++]=' ';
			printf("■要编译运行的文件名:\n");
			printf("%s\n",s);
			
			printf("■编译参数:\n");
			s[i++]='\"';
			if (getstr(s+i,i)==-1)
				return -1;
		}
		else
		{
			printf("■要编译运行的文件名:\n");
			printf("%s\n",s+1);

			s[i++]='\"';
			s[i++]=' ';
			s[i++]='\"';
			catstr(s+i++,EXEC_SKIP);
		}

		s[i++]='\"';
		s[i++]=' ';
		printf("■运行参数:\n");
		s[i++]='\"';
		if (getstr(s+i,i)==-1)
			return -1;
		s[i++]='\"';
		s[i]='\0';
		return 2;
	}
	else
	{
		int i=0;
		int k=0;

		s[i++]='\"';
		if (catstr(s+i,argv[1],i)==-1)	//复制传递参数
			return -3;
		if (!exec(s,i))
		{
			if (extdel(s,i)==-1)	//删除扩展名
				return -3;
			s[i++]='\"';
			s[i++]=' ';
			printf("■要编译运行的文件名:\n");
			printf("%s\n",s);
			
			printf("■编译参数:\n");
			s[i++]='\"';
			int j=i,l;
			for (k=2;k<argc;k++) {
				l=i;
				if (catstr(s+i,argv[k],i)==-1)	//复制传递参数
					return -3;
				if (argv[k][i-l-1]==';') 
				{
					if (i>0)
					{
						s[--i]='\0';
					}
					break;
				}
				s[i++]=' ';
			}
			printf("%s\n",s+j);
		}
		else
		{
			printf("■要编译运行的文件名:\n");
			printf("%s\n",s+1);
			
			s[i++]='\"';
			s[i++]=' ';
			s[i++]='\"';
			catstr(s+i++,EXEC_SKIP);
		}
		
		s[i++]='\"';
		s[i++]=' ';
		printf("■运行参数:\n");
		s[i++]='\"';
		if (argc>k)
		{
			int j=i;
			for (k++;k<argc;k++) {
				if (catstr(s+i,argv[k],i)==-1)	//复制传递参数
					return -3;
				s[i++]=' ';
			}
			if (i-j>0)
			{
				s[--i]='\0';
			}
			printf("%s\n",s+j);
			s[i++]='\"';
			s[i]='\0';
		}
		else
		{
			if (getstr(s+i,i)==-1)
				return -1;
			s[i++]='\"';
			s[i]='\0';
		}
		return 2;
	}
}

int main(int argc,char *argv[])
{
	char LoaderCMD[MAX_LEN]="loader.bat";
	char LoaderDIR[MAX_LEN];
	char *pLoaderDIR=LoaderDIR;
	int i,j,r=slash(argv[0]);
	
// 	puts("-------------------------");
// 	printf("%d\n",argc);
// 	for (i=0;i<argc;i++)
// 	{
// 		printf("___%s___\n",argv[i]);
// 	}
// 	puts("-------------------------");
// 	puts(LoaderCMD);
// 	puts("1234567890123456789012345678901234567890123456789012345678901234567890");


	//system("\"\"E:\\Java\\发布\\PreLoader\\test ddw\\loader.bat\" \"a\" \"\" \"\" \"E:\\Java\\发布\\PreLoader\\Debug\\\"\"");
	//为命令字串添加引号，形式为
	//" "bat" "参数" "参数" ... "
	//最外面一定要有一个总引号
	LoaderDIR[0]='\"';

	//添加局部引号
	LoaderDIR[1]='\"';
	pLoaderDIR+=2;
	//将程序运行路径复制过来
	for (i=0;i<r;i++)	
		pLoaderDIR[i]=argv[0][i];
	pLoaderDIR[i]='\0';

	//复制bat文件名
	for (j=i;LoaderCMD[j-i]!='\0';j++)
		pLoaderDIR[j]=LoaderCMD[j-i];
	//添加局部引号
	pLoaderDIR[j++]='\"';
	pLoaderDIR[j++]=' ';
	pLoaderDIR[j]='\0';

//	puts(LoaderDIR);
	//处理参数
	clrstr(LoaderCMD,MAX_LEN);
	if (argProc(argc,argv,LoaderCMD)<=0)
	{
		printf("■错误\n");
		getch();
		return -1;
	}
//	printf("___%s___\n",LoaderCMD);
	//将参数复制到命令字串中
	for (i=j;LoaderCMD[i-j]!='\0';i++)
		pLoaderDIR[i]=LoaderCMD[i-j];
	//添加局部引号
	pLoaderDIR[i++]=' ';
	pLoaderDIR[i++]='\"';

	//将程序运行的路径作为参数
	for (j=0;j<r;j++)
		pLoaderDIR[i+j]=pLoaderDIR[j];
	//添加局部引号
	pLoaderDIR[i+j++]='\"';

	pLoaderDIR[i+j++]=' ';
	pLoaderDIR[i+j]='\0';
	i=i+j;

	//添加总引号
	pLoaderDIR[i++]='\"';
	pLoaderDIR[i++]='\0';
// 	printf("___%s___\n",LoaderDIR);
// 	puts("-------------------------");
	//调用系统函数
	system(LoaderDIR);
	return 0;
}
