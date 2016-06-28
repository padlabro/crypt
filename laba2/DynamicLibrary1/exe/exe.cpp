// exe.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "MathFuncsDll.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>

using namespace MathFuncs;

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
	if ((argv[2][0]=='^')&&(argc < 7))
	{
		printf("\nМодуль_не_задан...\n");
		return false;
	}

	if (strlen(argv[2]) > 1)
	{
		printf("Задана_неверная_операция...\n");
		return false;
	}

	return true;
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

	long size=0;
	LongInt a, b;

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
	
	if (!a.Read(ReadFlag, fNameA))
		return 0;
	if (!b.Read(ReadFlag, fNameB))
		return 0;;
	LongInt c;
	if (DoIt[0]=='-')
		c=a-b;
	if (DoIt[0]=='+')
		c=a+b;
	if (DoIt[0]=='*')
		c=a*b;
	if (DoIt[0]=='/')
		c=a/b;
	if (DoIt[0]=='%')
		c=a%b;
	if (DoIt[0]=='^')
	{
		LongInt mod;
		if(!mod.Read(ReadFlag, fNameM))
			return 0;
		c=c.Pow(a, b, mod);
	}
	//c.Print();	
	if (!c.WriteFile(WriteFlag, fNameOut))
		return 0;
	return 0;
}


