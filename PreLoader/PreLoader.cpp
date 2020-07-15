//Java Complier PreLoader
#include "Str.h"

int argProc(int argc,char *argv[],char *s)
{
	if (argc==1)
	{
		int i=0;
		printf("��������Ҫ�������е��ļ���:\n");
		s[i++]='\"';
		if (getstr(s+i,i)==-1)
			return -1;
		//�Ͻ�����ʱһ��ʼ����"�Ļ���ȥ��
		if (s[1]=='\"')
		{
			s[1]='\0';
			catstr(s,s+2);
		}
		if (!exec(s,i))
		{
			extdel(s,i);	//ɾ����չ��(��������˵Ļ�)
			s[i++]='\"';
			s[i++]=' ';

			printf("���������:\n");
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

		printf("�����в���:\n");
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
		if (catstr(s+i,argv[1],i)==-1)	//���ƴ��ݲ���
			return -2;
		if (!exec(s,i))
		{
			if (extdel(s,i)==-1)	//ɾ����չ��
				return -2;
			s[i++]='\"';
			s[i++]=' ';
			printf("��Ҫ�������е��ļ���:\n");
			printf("%s\n",s);
			
			printf("���������:\n");
			s[i++]='\"';
			if (getstr(s+i,i)==-1)
				return -1;
		}
		else
		{
			printf("��Ҫ�������е��ļ���:\n");
			printf("%s\n",s+1);

			s[i++]='\"';
			s[i++]=' ';
			s[i++]='\"';
			catstr(s+i++,EXEC_SKIP);
		}

		s[i++]='\"';
		s[i++]=' ';
		printf("�����в���:\n");
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
		if (catstr(s+i,argv[1],i)==-1)	//���ƴ��ݲ���
			return -3;
		if (!exec(s,i))
		{
			if (extdel(s,i)==-1)	//ɾ����չ��
				return -3;
			s[i++]='\"';
			s[i++]=' ';
			printf("��Ҫ�������е��ļ���:\n");
			printf("%s\n",s);
			
			printf("���������:\n");
			s[i++]='\"';
			int j=i,l;
			for (k=2;k<argc;k++) {
				l=i;
				if (catstr(s+i,argv[k],i)==-1)	//���ƴ��ݲ���
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
			printf("��Ҫ�������е��ļ���:\n");
			printf("%s\n",s+1);
			
			s[i++]='\"';
			s[i++]=' ';
			s[i++]='\"';
			catstr(s+i++,EXEC_SKIP);
		}
		
		s[i++]='\"';
		s[i++]=' ';
		printf("�����в���:\n");
		s[i++]='\"';
		if (argc>k)
		{
			int j=i;
			for (k++;k<argc;k++) {
				if (catstr(s+i,argv[k],i)==-1)	//���ƴ��ݲ���
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


	//system("\"\"E:\\Java\\����\\PreLoader\\test ddw\\loader.bat\" \"a\" \"\" \"\" \"E:\\Java\\����\\PreLoader\\Debug\\\"\"");
	//Ϊ�����ִ�������ţ���ʽΪ
	//" "bat" "����" "����" ... "
	//������һ��Ҫ��һ��������
	LoaderDIR[0]='\"';

	//��Ӿֲ�����
	LoaderDIR[1]='\"';
	pLoaderDIR+=2;
	//����������·�����ƹ���
	for (i=0;i<r;i++)	
		pLoaderDIR[i]=argv[0][i];
	pLoaderDIR[i]='\0';

	//����bat�ļ���
	for (j=i;LoaderCMD[j-i]!='\0';j++)
		pLoaderDIR[j]=LoaderCMD[j-i];
	//��Ӿֲ�����
	pLoaderDIR[j++]='\"';
	pLoaderDIR[j++]=' ';
	pLoaderDIR[j]='\0';

//	puts(LoaderDIR);
	//�������
	clrstr(LoaderCMD,MAX_LEN);
	if (argProc(argc,argv,LoaderCMD)<=0)
	{
		printf("������\n");
		getch();
		return -1;
	}
//	printf("___%s___\n",LoaderCMD);
	//���������Ƶ������ִ���
	for (i=j;LoaderCMD[i-j]!='\0';i++)
		pLoaderDIR[i]=LoaderCMD[i-j];
	//��Ӿֲ�����
	pLoaderDIR[i++]=' ';
	pLoaderDIR[i++]='\"';

	//���������е�·����Ϊ����
	for (j=0;j<r;j++)
		pLoaderDIR[i+j]=pLoaderDIR[j];
	//��Ӿֲ�����
	pLoaderDIR[i+j++]='\"';

	pLoaderDIR[i+j++]=' ';
	pLoaderDIR[i+j]='\0';
	i=i+j;

	//���������
	pLoaderDIR[i++]='\"';
	pLoaderDIR[i++]='\0';
// 	printf("___%s___\n",LoaderDIR);
// 	puts("-------------------------");
	//����ϵͳ����
	system(LoaderDIR);
	return 0;
}
