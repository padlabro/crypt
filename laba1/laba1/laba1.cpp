// laba1.cpp: длинная арифметика.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>


void LetsStart()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	printf("\nВведите :");
	printf("\n<Название_программы> <Файл_с_первым_большим_числом A> <Операция> <Файл_со_вторым_большим_числом B> <Результирующий_файл> [b] <Файл_с_модулем>\n\n");//[filename with module]\n\n");
	printf("Список_доступных_операций: \n");
	printf("\"+\" - Сложение\n");
	printf("\"-\" - Вычитание\n");
	printf("\"*\" - Умножение\n");
	printf("\"/\" - Деление\n");
	printf("\"%%\" - Остаток_от_деления\n");
	printf("\"^\" - Возведение_в_степень_по_модулю\n");
}

bool FoolProof(int argc, char* argv[])
{
	if (argc == 1)
		return false;

	if (argc > 7)
	{
		printf("\nВведены_лишние_параметры...\n");
		return false;
	}

	if ((argc > 0) && (argc < 6))
	{
		printf("\nНе_достаточно_данных...\n");
		return false;
	}

	if (strlen(argv[2]) > 1)
	{
		printf("Задана_неверная_операция...\n");
		return false;
	}

	return true;
}
//Длины файлов
void TakeSize (char *flag, char *fname, long &size)
{
	FILE *f=fopen(fname, flag);
	if (NULL==f)
		return;
	fseek(f,0,SEEK_END);
	size = ftell(f);
	fseek(f,0,SEEK_SET);
	fclose(f);
}
//Считывание файла
void ReadFile(char *flag, char *fname, long &size, bool &minus, int *mas)
{
	FILE *f=fopen(fname, flag);
	if (f==NULL)
	{
		printf("\nОшибка_чтения_файла %s",fname);
		return;
	}
	//считать знак
	fseek(f, 0, SEEK_SET);
	char sign;
	sign=fgetc(f);
	if (sign=='-')
	{
		minus=1;
		size-=1;
	}
	else
		fseek (f, 0, SEEK_SET);
	//считать число
	for (int i=size; i>0; i--)
		mas[i]=fgetc(f)-48;
	//Избавляемся от нулей перед числом
	for( int i=size;i>0;i--)
	{	
		if(mas[i] == 0)	
			size--;
		else break;
	}
}
//Запись в файл
void WriteFile (char *flag, int *c, long size, char *fname, bool minus)
{
	FILE *f=fopen(fname, flag);
	if (NULL==f)
	{
		printf("\nОшибка_записи_в_файл %s\n",fname);
		system("pause");/////
		return;
	}
	
	if (strcmp(flag,"wb")==0)
	{		
		if (size==0)
			fwrite(&c[0], sizeof(int), 1, f);
		else
		{
			if(minus==1)
				fwrite(&"-", sizeof(char), 1, f);

			for(int i=size; i>0; i--)
			{
				fwrite(&c[i], sizeof(int), 1, f); 
			}
		}
	}
	else
	{
		if (size==0)
			fprintf(f,"%d", 0);
		else
		{
			if(minus==1)
				fprintf(f,"%c",'-');

			for(int i=size; i>0; i--)
			{
				fprintf(f,"%d",c[i]); 
			}
		}
	}
	printf("Результат_записан_в_файл: %s.\n",fname);
	fclose(f);
}
//Удаление нулей
void DelNull(int *a, long &size)
{
	for(int i=size; i>0; i--)
	{
		if(a[i] == 0)
			size--;
		else break;
	}
}
//a>b
bool More (int * a, long sizeA, int * b, long sizeB)
{
	if (sizeA>sizeB)
		return true;
	else 
		if (sizeB>sizeA) 
			return false;
		else
			for (int i=sizeA; i>=0; i--)
			{
				if (a[i]>b[i]) 
					return true;
				else 
					if (b[i]>a[i]) 
						return false;
			}
	return false;
}
//a=b
bool Equally(int * a, long sizeA, int * b, long sizeB)
{
	if (sizeA!=sizeB) 
		return false;
	else
		for (int i=sizeA; i>=0; i--)
		{
			if (a[i]!=b[i])
				return false;
		}

	return true;
}
//Сумма
void Sum(int *a, int *b, long &size)
{
	for (int i=0; i < size; i++)
	{
		b[i] += a[i]; 
		b[i + 1] += (b[i] / 10); 
		b[i] %= 10; 
	}
	//Избавляемся от нулей перед числом
	DelNull(b, size);
}
//Вычитание
void Sub(int *a, int *b, int *c, long &size)
{
	int loan=0, c1=0;
	for(int i=0; i<=size; i++)
	{	
		c1=a[i]-b[i]-loan;
		if(c1>=0)	 
		{
			c[i]=c1;	
			loan=0;
		}
		else		 
		{
			c[i]=10+c1;	 
			loan=1;	
		}
	}
	DelNull(c, size);
}
//Умножение
void Mul(int *a, int *b, int *c, long sizeA, long sizeB, long &sizeC)
{
	for (int i=0; i<=sizeA; i++)
		for (int j=0; j<=sizeB; j++)
			c[i+j-1]=c[i+j-1]+a[i]*b[j];
	for (int i=0; i<sizeC; i++)
	{
		c[i+1]+=c[i]/10;
		c[i]=c[i]%10;
	}
	DelNull(c, sizeC);//избавляемся от нулей
}
//Деление на однозначное число
void LightDiv(int *a, int b, int *q, int &r, long sizeA)
{
	int k;
	for (int i=sizeA; i>0; i--)
	{
		k=r*10+a[i];
		q[i]=k/b; 
		r=k-q[i]*b;
	}
}
//Деление
void Div(int *a, long sizeA, int *b, long sizeB, int *q, long &sizeq, int *r, long &sizer, int *c, int *m)
{
	if(sizeA<sizeB)
		sizer=sizeA;
	else
		sizer=sizeB;
	long start=sizeA-sizer;
	int nm=1;
	for(int i=sizeA-sizer+1; i<=sizeA; i++)
	{
		r[nm]=a[i];
		nm++;
	}
	DelNull(r, sizer);
	for (int i=sizeB; i>0; i--)
		m[i]=b[i];
	long sizem=sizeB;
	if(!(Equally(a, sizeA, r, sizer)&&(More(b, sizeB, a, sizeA))))
	{
		int k;
		for (k=0; (More(r, sizer ,m, sizem)||(Equally(r, sizer, m, sizem))); k++)
		{
			Sub(r, m, c, sizer);
			for (int j=0; j<=sizer; j++)
			{
				r[j]=c[j];
			}
		}
		q[sizeq]=k;
	}
	sizeq++;
	for (int i=start; i>0; i--)
	{
		sizer++;
		for (int j=sizer-1; j>0; j--)
			r[j+1]=r[j];
		r[1]=a[i];
		int k;
		for (k=0; (More(r, sizer ,m , sizem)||(Equally(r, sizer, m, sizem))); k++)
		{
			DelNull(m, sizem);
			Sub(r, m, c, sizer);
			for (int j=0; j<=sizer; j++)
				r[j]=c[j];
		}
		q[sizeq]=k;
		sizeq++;
	}
	sizeq--;
	for (int i=1; i<=sizeq/2; i++)
	{
		int k=q[i];
		q[i]=q[sizeq-i+1];
		q[sizeq-i+1]=k;
	}
	DelNull(q, sizeq);
	return;
}
//Преобразование числа а в двоичную систему счисления
int *DecToBin(int *a, long sizeA, long &size)
{
	int *b=(int*)malloc((sizeA+2)*sizeof(int));
	for (int i=0; i<=sizeA; i++)
		b[i]=a[i];
	long sizeB=sizeA;
	int *d=(int*)malloc((1+2)*sizeof(int));//а в двоичном виде
	d[0]=0;
	d[1]=0;
	size=0;
	while(b[sizeB]!=0)
	{
		int r=0;
		int *q=(int*)malloc((sizeB+2)*sizeof(int));
		for (int i=0; i<=sizeB; i++)
			q[i]=0;
		long sizeq=sizeB;
		size++;
		int *vd=(int*)malloc((size+2)*sizeof(int));
		for (int i=0; i<=size; i++)
			vd[i]=d[i];
		LightDiv(b, 2, q, r, sizeB);
		DelNull(q, sizeq);
		for (int i=0; i<=sizeq; i++)
			b[i]=q[i];
		sizeB=sizeq;
		vd[size]=r;
		free(d);
		d=(int*)malloc((size+2)*sizeof(int));
		for (int i=0; i<=size; i++)
			d[i]=vd[i];
	}
	return d;
}
//Возведение в степень по модулю
int *Pow (int *a, long &sizeA, int *mod, long sizeM, int *b, long sizeB, long &sizea0)
{
	int *c;
	int *va;
	int *ha;
	int *haha;
	int *q;
	int *r;
	long sizeD=0;
	int *d;
	d=DecToBin(b, sizeB, sizeD);

	if (sizeA>sizeM)
	{
		ha=(int*) malloc((sizeA+2)*sizeof(int));
		haha=(int*) malloc((sizeA+2)*sizeof(int));
		q=(int*)malloc((sizeA+2)*sizeof(int));//частное
		r=(int*)malloc((sizeA+2)*sizeof(int));//остаток
		for(int i=0; i<=sizeA; i++)
		{
			ha[i]=0;
			haha[i]=0;
			q[i]=0;
			r[i]=0;
		}
		long sizer=sizeA, sizeq=1;
		Div(a, sizeA, mod, sizeM, q, sizeq, r, sizer, ha, haha);
		free(q);
		free(a);
		a=(int*) malloc((sizer+2)*sizeof(int));
		for (int i=0; i<=sizer; i++)
			a[i]=r[i];
		sizeA=sizer;		
		free(r);
	}
		
	int *a0=(int *) malloc((sizeA+2)*sizeof(int));
	sizea0=sizeA;
	for (int i=0; i<=sizea0; i++)
		a0[i]=a[i];
	for (int j=sizeD-1; j>=1; j--)
	{
		long sizeva=sizeA;
		va=(int *) malloc((sizeva+2)*sizeof(int));
		if (d[j]==0)
		{
			for (int i=0; i<=sizeva; i++)
				va[i]=0;
			va[1]=1;
		}
		else
			for (int i=0; i<=sizeva; i++)
				va[i]=a[i];
		DelNull(va, sizeva);
		long sizeC=sizea0+sizea0;
		c=(int *) malloc((sizeC+2)*sizeof(int));
		for (int i=0; i<=sizeC; i++)
			c[i]=0;
		Mul(a0, a0, c, sizea0, sizea0, sizeC); 
		if (sizeC>sizeM)
		{
			ha=(int*) malloc((sizeC+2)*sizeof(int));
			haha=(int*) malloc((sizeC+2)*sizeof(int));
			q=(int*)malloc((sizeC+2)*sizeof(int));//частное
			r=(int*)malloc((sizeC+2)*sizeof(int));//остаток
			for(int i=0; i<=sizeC; i++)
			{
				ha[i]=0;
				haha[i]=0;
				q[i]=0;
				r[i]=0;
			}
			long sizer=sizeC, sizeq=1;
			Div(c, sizeC, mod, sizeM, q, sizeq, r, sizer, ha, haha);
			free(q);
			free(c);
			c=(int*) malloc((sizer+2)*sizeof(int));
			for (int i=0; i<=sizer; i++)
				c[i]=r[i];
			sizeC=sizer;		
			free(r);
		}

		sizea0=sizeC+sizeva;
		free(a0);
		a0=(int *) malloc((sizea0+2)*sizeof(int));
		for (int i=0; i<=sizea0; i++)
			a0[i]=0;
		Mul(c, va, a0, sizeC, sizeva, sizea0);  
		if (sizea0>sizeM)
		{
			ha=(int*) malloc((sizea0+2)*sizeof(int));
			haha=(int*) malloc((sizea0+2)*sizeof(int));
			q=(int*)malloc((sizea0+2)*sizeof(int));//частное
			r=(int*)malloc((sizea0+2)*sizeof(int));//остаток
			for(int i=0; i<=sizea0; i++)
			{
				ha[i]=0;
				haha[i]=0;
				q[i]=0;
				r[i]=0;
			}
			long sizer=sizea0, sizeq=1;
			Div(a0, sizea0, mod, sizeM, q, sizeq, r, sizer, ha, haha);
			free(q);
			free(a0);
			a0=(int*) malloc((sizer+2)*sizeof(int));
			for (int i=0; i<=sizer; i++)
				a0[i]=r[i];
			sizea0=sizer;		
			free(r);
		}
		free(va);
		free(c);
	}
	return a0;
}
int main(int argc, char* argv[])
{
	LetsStart ();

	if (!FoolProof(argc,argv))
		return 0;

	char *fNameA=argv[1];
	char *DoIt=argv[2];
	char *fNameB=argv[3];
	char *fNameOut=argv[4];
	char *bin=argv[5];
	char *fNameM=argv[6];

	//char *fNameA="1.txt";
	//char *DoIt="%";
	//char *fNameB="2.txt";
	//char *fNameOut="3.txt";
	//char *bin="ss";
	//char *fNameM="mod.txt";

	//char *fmodule = argv[6];
	long sizeA=0, sizeB=0, size=0, sizeC=0;
	int *a, *b;
	bool minusA=0, minusB=0;

	char *ReadFlag=0;
	char *WriteFlag=0;
	if(strcmp(bin,"-b")==0)
	{
		ReadFlag = "rb";
		WriteFlag = "bw";
	}
	else
	{
		ReadFlag = "r";
		WriteFlag = "w";
	}
	
	TakeSize(ReadFlag, fNameA, sizeA);
	if(sizeA==0)
	{
		printf("\nФайл %s пуст\n", fNameA);
		return 0;
	}
	TakeSize(ReadFlag, fNameB, sizeB);
	if(sizeB==0)
	{
		printf("\nФайл %s пуст\n", fNameB);
		return 0;
	}
	if(sizeA>sizeB)
		size = sizeA+1;
	else size = sizeB+1;
	a = (int *) malloc((size+2)*sizeof(int));
	b = (int *) malloc((size+2)*sizeof(int));

	for(int i = 0; i <= size; i++)
	{
		a[i] = 0;
		b[i] = 0;
	}

	ReadFile(ReadFlag, fNameA, sizeA, minusA, a);
	for (int i=0; i<=sizeA; i++)
		if ((a[i]<0)||(a[i]>9))
		{
			printf("\nВ_файле_присутствуют_недопустимые_символы %s... \n", fNameA);
			return 0;
		}
	ReadFile(ReadFlag, fNameB, sizeB, minusB, b);
	for (int i=0; i<=sizeB; i++)
		if ((b[i]<0)||(b[i]>9))
		{
			printf("\nВ_файле_присутствуют_недопустимые_символы %s... \n", fNameB);
			return 0;
		}

	bool minus=0;
	//**********************
	//Сложение и вычитание *
	//**********************
	
	if (DoIt[0]=='-')
	{
		minusB=!minusB;
		DoIt[0]='+';
	}
	if (DoIt[0]=='+')
	{
		if (!((minusA) ^ (minusB)))//если c одинаковым знаком
		{
			minus=minusA;
			Sum(a,b,size);
			WriteFile (WriteFlag, b, size, fNameOut, minus);
			free(a);
			free(b);
			return 0;
		}
		int *c = (int*)malloc((size+2)*sizeof(int));
		for(int i = 0; i <= size; i++)
			c[i] = 0;
		if (Equally(a, sizeA, b, sizeB))
			size=1;
		else
		{
			if (More (a, sizeA, b, sizeB))
			{
				Sub(a,b,c,size);
				if (minusA)
					minus=1;
			}
			else 
			{
				Sub(b,a,c,size);
				if (minusB)
					minus=1;
			}
		}
		WriteFile (WriteFlag, c, size, fNameOut, minus);
		return 0;
	}
	//**********************
	//		Умножение	   *
	//**********************
	if (DoIt[0]=='*')
	{
		if((minusA) ^ (minusB))
			minus=1;
		long sizeC=sizeA+sizeB;
		int *c=(int*)malloc((sizeC+2)*sizeof(int));
		for (int i=0; i<=sizeC; i++)
			c[i]=0;
		Mul(a, b, c, sizeA, sizeB, sizeC);
		
		WriteFile (WriteFlag, c, sizeC, fNameOut, minus);
		free(c);
		return 0;
	}

	//**********************
	//		Деление		   *
	//**********************
	if ((DoIt[0]=='/')||(DoIt[0]=='%'))
	{
		if((minusA) ^ (minusB))
			minus=1;
		int *q=(int*)malloc((size+2)*sizeof(int));//частное
		int *r=(int*)malloc((size+2)*sizeof(int));//остаток
		int *c=(int*)malloc((size+2)*sizeof(int));
		int *m=(int*)malloc((size+2)*sizeof(int));
		for (int i=0; i<=size; i++)
		{
			q[i]=0;
			r[i]=0;
			c[i]=0;
			m[i]=0;
		}

		int R=0;long sizeq=size; long sizeC=size;
		long sizer=size; long sizem=size;
		if (sizeB==1)//если делимое-однозначное
		{
			LightDiv(a, b[1], q, R, sizeA);
			if ((minusA)&&(R!=0))
			{
				r[1]=1;
				Sum(r, q, sizeq);
			}
			else Sum(r,q,sizeq);
			Mul(b, q, c, sizeB, sizeq, sizeC);
			if (More(a, sizeA, c, sizeC))
				Sub(a, c, r, sizer);
			else
				Sub(c, a, r, sizer);
			if (DoIt[0]=='/')
				WriteFile(WriteFlag, q, sizeq, fNameOut, minus);
			if (DoIt[0]=='%')
				WriteFile(WriteFlag, r, sizer, fNameOut, 0);
			free(q),free(r),free(m),free(c),free(b),free(a);
			return 0;
		}
		sizeq=1;
		Div(a, sizeA, b, sizeB, q, sizeq, r, sizer, c, m);
		if ((minusA)&&(sizer!=0))
		{
			for (int i=0; i<=size; i++)
				m[i]=0;
			m[1]=1;
			sizeq++;
			Sum(m, q, sizeq);
		}
		for(int i = 0; i <= sizeC; i++)
			c[i] = 0;
		Mul(b, q, c, sizeB, sizeq, sizeC);
		if (More(a, sizeA, c, sizeC))
		{
			sizer=sizeA;
			Sub(a, c, r, sizer);
		}
		else
		{
			sizer=sizeC;
			Sub(c, a, r, sizer);
		}
		if (DoIt[0]=='/')
			WriteFile(WriteFlag, q, sizeq, fNameOut, minus);
		if (DoIt[0]=='%')
			WriteFile(WriteFlag, r, sizer, fNameOut, 0);
		free(q),free(r),free(m),free(c),free(b),free(a);
		return 0;
	}
	//**************************************
	//		Возведение в степень		   *
	//**************************************
	if(DoIt[0]=='^')
	{
		if (argc<7)
		{
			printf("\nМодуль_не_указан!\n");
			return 0;
		}
		if(minusB)
		{
			printf("\nОтрицательная_степень!\n");
			return 0;
		}
		bool minus=minusA;
		if (b[sizeB]%2==0)
			minus=0;
		long sizeM;
		bool minusM=0;
		TakeSize(ReadFlag, fNameM, sizeM);
		if(sizeM==0)
		{
			printf("\nФайл %s пуст\n", fNameM);
			return 0;
		}
		int *mod = (int *) malloc((sizeM+2)*sizeof(int));
		mod[0]=0;
		ReadFile(ReadFlag, fNameM, sizeM, minusM, mod);
		for (int i=0; i<=sizeM; i++)
		if ((mod[i]<0)||(mod[i]>9))
		{
			printf("\nВ_файле_присутствуют_недопустимые_символы %s... \n", fNameM);
			return 0;
		}
		if(minusM)
		{
			printf("\nОтрицательный_модуль!\n");
			return 0;
		}
		int *a0;
		long sizea0;
		a0=Pow(a, sizeA, mod, sizeM, b, sizeB, sizea0);
		WriteFile(WriteFlag, a0, sizea0, fNameOut, minus);
	}
	return 0;
}