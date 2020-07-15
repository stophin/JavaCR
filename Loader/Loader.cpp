//Java Complier Loader
#include "Str.h"
#include "string"
using namespace std;

int PackagePreread(char *fn,char *package)
{
	//Package属性：
	//不能有多个，且package语句必须在除
	//注释外的第一行才可以通过编译
	int r=0,count=0;
	char buf[MAX_LEN],temp[MAX_LEN];
	cpystr(buf,fn);
	catstr(buf,".java");

	FILE *fp=fopen(buf,"r");
	if (fp==NULL)
		return 0;
	while(fgets(buf,MAX_LEN,fp)!=NULL)
	{
		cpystr(temp,buf);
		temp[7]='\0';
		if (equstr(temp,"package"))
		{
			//printf("%s_\n",buf);
			if (count==0)
			{
				for (r=8;buf[r]!='\0';r++)
				{
					if (!(buf[r]==' '||buf[r]=='\t'))
						break;
				}
				r=cpystr(package,buf+r);
				for (;r>0;r--)
				{
					if (package[r]==';')
						break;
				}
				for (r--;r>0;r--) {
					if (package[r]!=' ')
						break;
				}
				package[r+1]='\0';
				//printf("%s_\n",package);
			}
			count++;
		}
		else if (temp[0]!='\0'&&temp[1]!='\0')
		{
			temp[2]='\0';
			if (!(equstr(temp,"  ")||!equstr(temp,"//")||!equstr(temp,"/*")))
			{
				break;	
			}
		}
	}
	fclose(fp);
	return count;
}

int JarPrepare(char *javacparam,char *jarfile)
{
	int j = 0;
	for (j=0;javacparam[j]!='\0';j++);
	if (j<4)
		return 0;
	char temp;
	temp=javacparam[4];
	javacparam[4]='\0';

	if (equstr(javacparam,"jar "))
	{
		javacparam[4]=temp;
		int i = 0;
		for (i=4;javacparam[i]!='\0';i++)
		{
			if (javacparam[i]==';')
				break;
		}
		if (javacparam[i]=='\0')
			return 0;
		return i;
	}
	else
	{
		javacparam[4]=temp;
		return 0;
	}
}

int DirpathPare(char *javacparam,char * dirpath)
{
	int i;
	for (i=0;javacparam[i]!='\0'&&javacparam[i]!=' ';i++);
	if (javacparam[i]==' ')
	{
		javacparam[i]='\0';
		bool dir=equstr(javacparam,"-d");
		javacparam[i]=' ';
		if (dir)
		{
			int j=++i;
			for (i++;javacparam[i]!='\0'&&javacparam[i]!=' ';i++);
			char temp=javacparam[i];
			javacparam[i]='\0';
			cpystr(dirpath,javacparam+j);
			javacparam[i]=temp;
			return i;
		}
	}
	return 0;
}

int main(int argc,char *argv[])
{
	char Command[MAX_LEN]="",package[MAX_LEN],temp[MAX_LEN],jarfile[MAX_LEN],dirpath[MAX_LEN];
	char *filename=NULL,*javacparam=NULL,*javaparam=NULL;
	int pcount=0,namepos,nameposlast,i,j;
	int jarprepare=0,dirpare=0;
	bool javacont=false;
	
// 	puts("-------------------------");
// 	printf("%d\n",argc);
// 	for (i=0;i<argc;i++)
// 	{
// 		printf("___%s___\n",argv[i]);
// 	}
// 	puts("-------------------------");

	//参数处理
	if (argc<=1)
		return -1;

	if (argc>=2)
		filename=argv[1];
	if (argc>=3)
		javacparam=argv[2];
	if (argc>=4)
		javaparam=argv[3];
	
	//文件明不存在或者为空
	if (filename==NULL||filename[0]=='\0')
	{
		return -1;
	}

	//获取文件名和路径之间的间断指针
	namepos=slash(filename);				//从前往后数
	nameposlast=getlen(filename)-namepos;	//从后往前数

	//预处理包
	clrstr(package,MAX_LEN);
	//多个包时获取第一个包信息
	pcount=PackagePreread(filename,package);
	if (pcount!=0)	//有包信息
	{
		//当出现多个包时，默认采用第一个包信息
		//虽然多个包不能编译通过，但是可以使用
		//第一个包信息来运行编译好的文件
		if (pcount>1)
		{
			printf("■注意:出现多个package\n");
		}
//		printf("%d__%s__\n",pcount,package);

		//复制到临时变量处理
		//先复制路径
		cpystr(temp,filename);
		temp[namepos]='\0';
		catstr(temp,package);
		//将.全部变为\用于文件路径
		for (i=namepos;temp[i]!='\0';i++)
		{
			if (temp[i]=='.')
				temp[i]='\\';
		}
		catstr(temp,"\\");
		//添加文件名和扩展名
		catstr(temp,filename+namepos);
		catstr(temp,".class");
//		printf("__%s__\n",temp);
		//查找是否存在
		FILE *fp=fopen(temp,"r");
		if (fp)
		{
			printf("■文件已经在下列包中存在\n");
			for (i=0;javacparam[i]!='\0'&&javacparam[i]==EXEC_CDLD;i++)
			{
				printf("../");
			}
			printf("%s\n",package);
			fclose(fp);
		}
	}
	
	//编译参数改变过程
	if (equstr(javacparam,EXEC_SKIP))
	{
		SEGMENT;
		printf("■已跳过编译\n");
		SEGMENT;
	}
	else
	{
		//jar包
		jarprepare=JarPrepare(javacparam,jarfile);
		if (jarprepare>0)
		{
			char temp=javacparam[jarprepare];
			javacparam[jarprepare]='\0';
			cpystr(jarfile,javacparam+4);
			javacparam+=jarprepare+1;
		}

		//将文件名和编译参数连接起来
		cpystr(Command,"javac ");
		catstr(Command,"\"");
		catstr(Command,filename);
		catstr(Command,".java");
		catstr(Command,"\" ");

		//获取-d的信息
		dirpare=DirpathPare(javacparam,dirpath);
		if (dirpare>0)
		{
			javacparam+=dirpare;
		}
		else
		{
			cpystr(dirpath,".");
		}
//		printf("__%s__\n",dirpath);

		//如果有包信息则添加包参数
		if (dirpare||pcount!=0)
		{
			catstr(Command," -d ");
			catstr(Command,dirpath);
		}
		//更改路径信息
		for (i=0;dirpare==0&&javacparam[i]!='\0'&&javacparam[i]==EXEC_CDLD;i++)
		{
			if (i==0&&pcount==0)
			{
				catstr(Command," -d ");
				continue;
			}
			if (i==0&&pcount!=0)
			{
				catstr(Command,"./");
				continue;
			}
			catstr(Command,"../");
			continue;
		}
		catstr(Command,javacparam+i);
//		catstr (Command,"-sourcepath ");
//		catstr(Command,"\"");
// 		catstr(Command,filename);
// 		Command[getlen(Command)-nameposlast]='\0';
//		catstr(Command,"\"");
		printf("■%s\n",Command);
		SEGMENT;
		if (system(Command)==-1)
		{
			SEGMENT;
			printf("■编译出错\n");
			return -1;
		}
		SEGMENT;
		if (argc<=3)
			return -2;
	}

	if (javaparam&&pcount<=0)
	{
		//更改运行条件，相当于包
		cpystr(temp,filename);
		temp[namepos]='\0';
		j=getlen(temp);
		for (j--;j>=0&&temp[j]!='\\';j--);
		for (i=0;javaparam[i]==EXEC_CDLD;i++)
		{
			for (j--;j>0&&temp[j]!='\\';j--);
			if (j<=0)
				break;
			filename[j]='\0';
		}
		if (i)
		{
			j++;
			strcat(filename,"\\");
			strcat(filename,filename+namepos);
			namepos=getlen(filename)-nameposlast;
			for (i=0;temp[i]!='\0';i++)
			{
				if (temp[i]=='\\')
					temp[i]='.';
			}
			temp[i-1]='\0';
		//	printf("%s__%s\n",temp+j,filename);
			cpystr(package,temp+j);
			pcount=-1;
		}
	}

	//jar包生成
	if (jarprepare>0)
	{
		cpystr(Command,"jar cvf ");
		
		catstr(Command,dirpath);
		catstr(Command,"/");
		catstr(Command,jarfile);
		catstr(Command,".jar ");
		
		catstr(Command,"-C ");
 		catstr(Command,dirpath);
		catstr(Command," ");
		if (pcount!=0)
		{
			cpystr(jarfile,package);
			for (int i=0;jarfile[i]!='\0';i++)
			{
				if (jarfile[i]=='.')
					jarfile[i]='\\';
			}
			catstr(Command,jarfile);
			catstr(Command,"\\");
		}
		catstr(Command,filename+namepos);
		catstr(Command,".class");
		printf("■%s\n",Command);
		SEGMENT;
		if (system(Command)==-1)
		{
			SEGMENT;
			printf("■打包出错\n");
			return -1;
		}
		SEGMENT;
	}
	
	//检查运行参数是否不需运行
	if (equstr(javaparam,EXEE_SKIP))
	{
		SEGMENT;
		printf("■未运行\n");
		return 0;
	}
	else if (equstr(javaparam,EXEE_CONT))
	{
		javacont=true;
		javaparam+=1;
	}
	//将文件名和运行参数连接起来
	cpystr(Command,"java ");
	
	if (dirpare)
	{
		catstr(Command,"-cp ");
		catstr(Command,dirpath);
		catstr(Command," ");
	}
	//更改路径信息
	for (i=0;dirpare==0&&javaparam[i]!='\0'&&javaparam[i]==EXEC_CDLD;i++)
	{
		if (i==0)
		{
			catstr(Command,"-cp ");
		}
		catstr(Command,"../");
	}
	for (j=0;javacparam[j]!='\0'&&javacparam[j]==EXEC_CDLD;j++)
	{
		if (javaparam[j]=='\0')
		{
			catstr(Command,"-cp ");
		}
		catstr(Command,"../");
	}
	if (i!=0||j!=0)
	{
		catstr(Command," ");
	}
	// 	catstr(Command,"\"");
	// 	catstr(Command,filename);
	// 	Command[getlen(Command)-nameposlast]='\0';
	// 	catstr(Command,"\"");
	//如果有包信息，将包信息添加到文件名前
	if (pcount!=0)
	{
		catstr(Command,package);
		catstr(Command,".");
	}
	catstr(Command,filename+namepos);
	if (pcount>=0)
	{
		catstr(Command," ");
		catstr(Command,javaparam);
	}
	else
	{
// 		cpystr(temp,"cd ");
// 		catstr(temp,filename);
// 		temp[namepos+3]='\0';
// 		printf("%s\n",temp);
// 		system(temp);
// 		sprintf(temp,"set CLASSPATH=%%CLASSPATH%%;.;");
// 		catstr(temp,filename);
// 		temp[namepos+28]='\0';
// 		printf("%s\n",temp);
// 		system(temp);
	}
	printf("■%s\n",Command);
	SEGMENT;
	if (system(Command)==-1)
	{
		SEGMENT;
		printf("■运行出错\n");
		return -1;
	}
	SEGMENT;
	printf("■运行成功!!\n");
	if (!javacont)
	{
		getch();
	}
	return 0;
}
