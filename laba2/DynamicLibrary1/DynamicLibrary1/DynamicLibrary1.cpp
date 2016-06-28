// MathFuncsDll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "MathFuncsDll.h"
#include <iostream>
#include <stdexcept>
#include <windows.h>

using namespace std;

namespace MathFuncs
{
	LongInt::LongInt()
	{
		size=0; 
		minus=0;
		digits=NULL;
	}
	LongInt::~LongInt()
	{
		//free(digits);
	}
	void LongInt::Print()
	{
		if(minus)
			printf("-");
		for(int i=size; i>0; i--)
			printf("%d", digits[i]);
		printf("(%d)", size);
	}
	bool LongInt::TakeSize (char *flag, char *fname)
	{
		FILE *f=fopen(fname, flag);
		if (NULL==f)
			return false;
		fseek(f,0,SEEK_END);
		size = ftell(f);
		fseek(f,0,SEEK_SET);
		fclose(f);
		return true;
	}
	bool LongInt::ReadFile(char *flag, char *fname)
	{	SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		FILE *f=fopen(fname, flag);
		if (f==NULL)
		{
			printf("\nОшибка_чтения_файла %s",fname);
			return false;
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
		{
			fseek (f, 0, SEEK_SET);
		}
		//считать число
		if (strcmp(flag,"rb")==0)
		{
			for (int i=size; i>0; i--)
				digits[i]=fgetc(f)-48;
		}
		else 
			for (int i=size; i>0; i--)
				digits[i]=fgetc(f)-48;
		DelNull();
		//Избавляемся от нулей перед числом
		return true;
	}
	bool LongInt::Read(char *flag, char *fname)
	{
		if (!TakeSize(flag, fname))
			return false;
		if(size==0)
		{
			printf("Файл %s пуст\n", fname);
			return false;
		}
		digits = (int *) malloc((size+2)*sizeof(int));
		for(int i = 0; i <= size; i++)
			digits[i] = 0;
		if (!ReadFile(flag, fname))
			return false;
		if (flag!="rb")
		for (int i=0; i<=size; i++)
			if ((digits[i]<0)||(digits[i]>9))
			{
				printf("В_файле_присутствуют_недопустимые_символы %s... ", fname);
				return false;
			}
		if (size==0)
		{
			printf("В_файле_присутствуют_недопустимые_символы %s... ", fname);
			return false;
		}
		return true;
	}
	void LongInt::DelNull()
	{
		for(int i=size; i>0; i--)
		{
			if(digits[i] == 0)
				size--;
			else break;
		}
	}
	bool LongInt::WriteFile (char *flag, char *fname) 
	{
		FILE *f=fopen(fname, flag);
		if (NULL==f)
		{
			printf("\nОшибка_записи_в_файл %s\n",fname);
			return false;
		}
		
		if (strcmp(flag,"wb")==0)
		{		
			if (size==0)
				fwrite(&digits[0], sizeof(int), 1, f);
			else
			{
				if(minus==1)
					fwrite(&"-", sizeof(char), 1, f);

				for(int i=size; i>0; i--)
				{
					fwrite(&digits[i], sizeof(int), 1, f); 
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
					fprintf(f,"%d",digits[i]); 
				}
			}
		}
		printf("Результат_записан_в_файл: %s.\n",fname);
		fclose(f);
		return true;
	}
	bool LongInt::More (LongInt a, LongInt b)
	{
		if (a.size>b.size)
			return true;
		else 
			if (b.size>a.size) 
				return false;
			else
				for (int i=a.size; i>=0; i--)
				{
					if (a.digits[i]>b.digits[i]) 
						return true;
					else 
						if (b.digits[i]>a.digits[i]) 
							return false;
				}
		return false;//тут равно
	}
	bool LongInt::Equally(LongInt a, LongInt b)
	{
		if (a.size!=b.size) 
			return false;
		else
			for (int i=a.size; i>=0; i--)
			{
				if (a.digits[i]!=b.digits[i])
					return false;
			}
		return true;
	}
	bool LongInt::operator>(const LongInt& a)
	{
		if (minus ^ a.minus)
			if (a.minus)
				return true;
			else
				return false;
		else
			if (minus && a.minus)
				if (More(a, *this))
					return true;
				else
					return false;
			else
				if(More(*this, a))
					return true;
				else
				return false;
	}
	bool LongInt::operator<(const LongInt& a)
	{
		if (minus ^ a.minus)
			if (minus)
				return true;
			else
				return false;
		else
			if (minus && a.minus)
				if (More(*this, a))
					return true;
				else
					return false;
			else
				if(More(a, *this))
					return true;
				else
				return false;
	}
	bool LongInt::operator>=(const LongInt& a)
	{
		if (minus ^ a.minus)
			if (a.minus)
				return true;
			else
				return false;
		else
			if (minus && a.minus)
				if (More(*this, a))
					return true;
				else
					return false;
			else
				if(More(a, *this))
					return false;
				else
				return true;
	}
	bool LongInt::operator<=(const LongInt& a)
	{
		if (minus ^ a.minus)
			if (minus)
				return true;
			else
				return false;
		else
			if (minus && a.minus)
				if (More(a, *this))
					return false;
				else 
					return true;
			else
				if(More(*this, a))
					return false;
				else
				return true;
	}
	bool LongInt::operator==(const LongInt& a)
	{
		if (minus ^ a.minus)
			return false;
		else
			if (Equally(*this, a))
				return true;
			else
				return false;
	}
	bool LongInt::operator!=(const LongInt& a)
	{
		if (minus ^ a.minus)
			return true;
		else
			if (Equally(*this, a))
				return false;
			else
				return true;
	}
	LongInt LongInt::Sum(LongInt a, LongInt b, LongInt &c)
	{
		long mins;
		long maxs;
		if(a.size>b.size)
		{
			mins=b.size;
			maxs=a.size;
		}
		else
		{
			mins=a.size;
			maxs=b.size;
		}
		int v=0;
		for (int i=0; i <= mins; i++)
		{
			c.digits[i] = a.digits[i] + b.digits[i] + v; 
			v = (c.digits[i] / 10); 
			c.digits[i] %= 10; 
		}
		for (int i=mins+1; (i <= maxs); i++)
		{
			if(mins==a.size)
				c.digits[i] = b.digits[i] + v; 
			else
				c.digits[i] = a.digits[i] + v; 
			v = (c.digits[i] / 10);
			c.digits[i] %= 10; 
		}
		c.digits[maxs]+=v;
		c.DelNull();
		return c;
	}
	LongInt LongInt::Sub(LongInt a, LongInt b, LongInt &c)
	{
		c.size=a.size;
		int loan=0, c1=0;
		for(int i=0; i<=c.size; i++)
		{	
			if (i<=b.size)
				c1=a.digits[i]-b.digits[i]-loan;
			else
				c1=a.digits[i]-loan;
			if(c1>=0)	 
			{
				c.digits[i]=c1;	
				loan=0;
			}
			else		 
			{
				c.digits[i]=10+c1;	 
				loan=1;	//ставим флаг(заем)
			}
		}
		c.DelNull();//избавляемся от нулей
		return c;
	}
	LongInt LongInt::operator+(const LongInt& a)
	{
		LongInt c;
		if (a.size>size)
			c.size=a.size+1;
		else
			c.size=size+1;
		c.digits=(int*)malloc((c.size+2)*sizeof(int));
		for(int i = 0; i <= c.size; i++)
			c.digits[i] = 0;
		c=Addition(*this, a, c);
		return c;		
	}
	LongInt LongInt::operator-(const LongInt& a)
	{
		LongInt c;
		if (a.size>size)
			c.size=a.size;
		else
			c.size=size;
		c.digits=(int*)malloc((c.size+2)*sizeof(int));
		for(int i = 0; i <= c.size; i++)
			c.digits[i] = 0;
		minus=!minus;
		c=Addition(*this, a, c);
		c.minus=!c.minus;
		minus=!minus;
		return c;		
	}
	LongInt LongInt::Addition(LongInt a, LongInt b, LongInt &c)
	{
		if (!((a.minus) ^ (b.minus)))//если c одинаковым знаком
			{
				c.minus=a.minus;
				c=Sum(a, b, c);
				return c;
			}
			if (Equally(a, b))
				c.size=1;
			else
			{
				if (More (a, b))
				{
					c=Sub(a, b, c);
					if (a.minus)
						c.minus=1;
				}
				else 
				{
					c=Sub(b, a, c);
					if (b.minus)
						c.minus=1;
				}
			}
			return c;
	}
	LongInt LongInt::Mul(LongInt a, LongInt b, LongInt &c)
	{
		for (int i=0; i<=a.size; i++)
			for (int j=0; j<=b.size; j++)
				c.digits[i+j-1]=c.digits[i+j-1]+a.digits[i]*b.digits[j];
		for (int i=0; i<c.size; i++)
		{
			c.digits[i+1]+=c.digits[i]/10;
			c.digits[i]=c.digits[i]%10;
		}
		c.DelNull();
		return c;
	}
	LongInt LongInt::operator*(const LongInt& a)
	{
		LongInt c;
		c.size=a.size+size;
		if((minus) ^ (a.minus))
			c.minus=1;
		c.digits=(int*)malloc((c.size+2)*sizeof(int));
		for (int i=0; i<=c.size; i++)
			c.digits[i]=0;
		Mul(a, *this, c);
		return c;
	}
	LongInt LongInt::operator/(const LongInt& a)
	{
		LongInt q, r, c, m;
		long maxs;
		if (size>a.size)
			maxs=size+1;
		else
			maxs=a.size;
		q.size=r.size=c.size=m.size=maxs;
		q.digits=(int*)malloc((maxs+2)*sizeof(int));//частное
		r.digits=(int*)malloc((maxs+2)*sizeof(int));//остаток
		c.digits=(int*)malloc((maxs+2)*sizeof(int));
		m.digits=(int*)malloc((maxs+2)*sizeof(int));
		for (int i=0; i<=maxs; i++)
		{
			q.digits[i]=0;
			r.digits[i]=0;
			c.digits[i]=0;
			m.digits[i]=0;
		}
		Division(*this, a, q, r, c, m);
		return q;
	}
	void LongInt::LightDiv(LongInt a, int b, LongInt q, int &r)
	{
		int k;
		for (int i=a.size; i>0; i--)
		{
			k=r*10+a.digits[i];
			q.digits[i]=k/b; 
			r=k-q.digits[i]*b;
		}
	}
	void LongInt::Div(LongInt a, LongInt b, LongInt &q, LongInt &r, LongInt c, LongInt m)
	{
		if(a.size<b.size)
			r.size=a.size;
		else
			r.size=b.size;
		long start=a.size-r.size;
		int nm=1;
		for(int i=a.size-r.size+1; i<=a.size; i++)
		{
			r.digits[nm]=a.digits[i];
			nm++;
		}
		r.DelNull();
		for (int i=b.size; i>0; i--)
			m.digits[i]=b.digits[i];
		m.size=b.size;
		m.DelNull();
		if(!(Equally(a, r)&&(More(b, a))))
		{
			int k;
			for (k=0; (More(r, m)||(Equally(r, m))); k++)
			{
				c.size=r.size;
				c=Sub(r, m, c);
				for (int j=0; j<=r.size; j++)
				{
					r.digits[j]=c.digits[j];
				}
				r.size=c.size;
			}
			q.digits[q.size]=k;
		}
		q.size++;
		for (int i=start; i>0; i--)
		{
			r.size++;
			for (int j=r.size-1; j>0; j--)
				r.digits[j+1]=r.digits[j];
			r.digits[1]=a.digits[i];
			int k;
			r.DelNull();
			for (k=0; (More(r, m)||(Equally(r, m))); k++)
			{
				r.DelNull();
				m.DelNull();
				c.size=r.size;
				c=Sub(r, m, c);
				for (int j=0; j<=r.size; j++)
					r.digits[j]=c.digits[j];
				r.size=c.size;
			}
			q.digits[q.size]=k;
			q.size++;
		}
		q.size--;
		for (int i=1; i<=q.size/2; i++)
		{
			int k=q.digits[i];
			q.digits[i]=q.digits[q.size-i+1];
			q.digits[q.size-i+1]=k;
		}
		q.DelNull();
		return;
	}
	void LongInt::Division(LongInt a, LongInt b, LongInt &q, LongInt &r, LongInt c, LongInt m)
	{
		if((a.minus) ^ (b.minus))
			q.minus=1;
		int R=0;
		if (b.size==1)//если делимое-однозначное
		{
			LightDiv(a, b.digits[1], q, R);
			if ((a.minus)&&(R!=0))
			{
				r.digits[1]=1;
				m=Sum(r, q, m);
			}
			else m=Sum(r, q, m);
			for (int i=0; i<=m.size; i++)
				q.digits[i]=m.digits[i];
			q.size=m.size;
			c=Mul(b, q, c);
			if (More(a, c))
				r=Sub(a, c, r);
			else
				r=Sub(c, a, r);
			return ;
		}
		q.size=1;
		Div(a, b, q, r, c, m);
		if ((a.minus)&&(r.size!=0))
		{
			for (int i=0; i<=q.size; i++)
				c.digits[i]=q.digits[i];
			for (int i=0; i<=q.size; i++)
				q.digits[i]=0;
			for (int i=0; i<=m.size; i++)
				m.digits[i]=0;
			m.digits[1]=1;
			m.DelNull();
			q.size++;
			q=Sum(m, c, q);
		}
		for(int i = 0; i <= c.size; i++)
			c.digits[i] = 0;
		Mul(b, q, c);
		if (More(a, c))
		{
			r.size=a.size;
			r=Sub(a, c, r);
		}
		else
		{
			r.size=c.size;
			r=Sub(c, a, r);
		}
		return;
	}
	LongInt LongInt::operator%(const LongInt& a)
	{
		LongInt q, r, c, m;
		long maxs;
		if (size>a.size)
			maxs=size+1;
		else
			maxs=a.size;
		q.size=r.size=c.size=m.size=maxs;
		q.digits=(int*)malloc((maxs+2)*sizeof(int));//частное
		r.digits=(int*)malloc((maxs+2)*sizeof(int));//остаток
		c.digits=(int*)malloc((maxs+2)*sizeof(int));
		m.digits=(int*)malloc((maxs+2)*sizeof(int));
		for (int i=0; i<=maxs; i++)
		{
			q.digits[i]=0;
			r.digits[i]=0;
			c.digits[i]=0;
			m.digits[i]=0;
		}
		Division(*this, a, q, r, c, m);
		return r;
	}
	LongInt LongInt::DecToBin(LongInt a)
	{
		LongInt b;
		b.digits=(int*)malloc((a.size+2)*sizeof(int));
		for (int i=0; i<=a.size; i++)
			b.digits[i]=a.digits[i];
		b.size=a.size;
		LongInt d;
		d.digits=(int*)malloc((1+2)*sizeof(int));//а в двоичном виде
		d.digits[0]=0;
		d.digits[1]=0;
		d.size=0;
		while(b.digits[b.size]!=0)
		{
			int r=0;
			LongInt q;
			q.size=b.size;
			q.digits=(int*)malloc((q.size+2)*sizeof(int));
			for (int i=0; i<=b.size; i++)
				q.digits[i]=0;
			d.size++;
			LongInt vd;
			vd.size=d.size;
			vd.digits=(int*)malloc((vd.size+2)*sizeof(int));
			for (int i=0; i<=vd.size; i++)
				vd.digits[i]=d.digits[i];
			LightDiv(b, 2, q, r);
			q.DelNull();
			for (int i=0; i<=q.size; i++)
				b.digits[i]=q.digits[i];
			b.size=q.size;//sizeB=sizeq;
			vd.digits[d.size]=r;
			free(d.digits);
			d.digits=(int*)malloc((d.size+2)*sizeof(int));
			for (int i=0; i<=d.size; i++)
				d.digits[i]=vd.digits[i];
		}
		return d;
	}
	LongInt LongInt::Pow (LongInt a, LongInt b, LongInt mod)
	{
		LongInt fa;
		if(b.minus)
		{
			printf("Отрицательная_степень!\n");
			return b;
		}
		if(mod.minus)
		{
			printf("Модуль_не_указан!\n");
			return mod;
		}
		LongInt c;
		LongInt va;
		LongInt ha;
		LongInt haha;
		LongInt q;
		LongInt r;
		LongInt d;
		d.size=0;
		d=DecToBin(b);
		
		if (a.size>=mod.size)
		{
			ha.size=haha.size=q.size=r.size=a.size;
			ha.digits=(int*) malloc((ha.size+2)*sizeof(int));
			haha.digits=(int*) malloc((haha.size+2)*sizeof(int));
			q.digits=(int*)malloc((q.size+2)*sizeof(int));//частное
			r.digits=(int*)malloc((r.size+2)*sizeof(int));//остаток
			for(int i=0; i<=a.size; i++)
			{
				ha.digits[i]=0;
				haha.digits[i]=0;
				q.digits[i]=0;
				r.digits[i]=0;
			}
			q.size=1;
			Division(a, mod, q, r, ha, haha);
			free(q.digits);
			free(a.digits);
			a.digits=(int*) malloc((r.size+2)*sizeof(int));
			for (int i=0; i<=r.size; i++)
				a.digits[i]=r.digits[i];
			a.size=r.size;		
			free(r.digits);
		}
		LongInt a0;
		a0.digits=(int *) malloc((a.size+2)*sizeof(int));
		a0.size=a.size;
		for (int i=0; i<=a0.size; i++)
			a0.digits[i]=a.digits[i];
		for (int j=d.size-1; j>=1; j--)
		{
			LongInt va;
			va.size=a.size;
			va.digits=(int *) malloc((va.size+2)*sizeof(int));
			if (d.digits[j]==0)
			{
				for (int i=0; i<=va.size; i++)
					va.digits[i]=0;
				va.digits[1]=1;
			}
			else
				for (int i=0; i<=va.size; i++)
					va.digits[i]=a.digits[i];
			va.DelNull();
			
			c.size=a0.size+a0.size;//long sizeC=sizea0+sizea0;
			c.digits=(int *) malloc((c.size+2)*sizeof(int));
			for (int i=0; i<=c.size; i++)
				c.digits[i]=0;
			c = Mul(a0, a0, c); 
			if (c.size>=mod.size)
			{
				ha.size=haha.size=q.size=r.size=c.size;
				ha.digits=(int*) malloc((ha.size+2)*sizeof(int));
				haha.digits=(int*) malloc((haha.size+2)*sizeof(int));
				q.digits=(int*)malloc((q.size+2)*sizeof(int));//частное
				r.digits=(int*)malloc((r.size+2)*sizeof(int));//остаток
				for(int i=0; i<=c.size; i++)
				{
					ha.digits[i]=0;
					haha.digits[i]=0;
					q.digits[i]=0;
					r.digits[i]=0;
				}
				q.size=1;
				Div(c, mod, q, r, ha, haha);
				free(q.digits);
				free(c.digits);
				c.digits=(int*) malloc((r.size+2)*sizeof(int));
				for (int i=0; i<=r.size; i++)
					c.digits[i]=r.digits[i];
				c.size=r.size;		
				free(r.digits);
			}
			a0.size=c.size+va.size;
			free(a0.digits);
			a0.digits=(int *) malloc((a0.size+2)*sizeof(int));
			for (int i=0; i<=a0.size; i++)
				a0.digits[i]=0;
			a0=Mul(c, va, a0);
			if (a0.size>=mod.size)
			{
				ha.size=haha.size=q.size=r.size=a0.size;
				ha.digits=(int*) malloc((ha.size+2)*sizeof(int));
				haha.digits=(int*) malloc((haha.size+2)*sizeof(int));
				q.digits=(int*)malloc((q.size+2)*sizeof(int));//частное
				r.digits=(int*)malloc((r.size+2)*sizeof(int));//остаток
				for(int i=0; i<=a0.size; i++)
				{
					ha.digits[i]=0;
					haha.digits[i]=0;
					q.digits[i]=0;
					r.digits[i]=0;
				}
				q.size=1;
				Div(a0, mod, q, r, ha, haha);
				free(q.digits);
				free(a0.digits);
				a0.digits=(int*) malloc((r.size+2)*sizeof(int));
				for (int i=0; i<=r.size; i++)
					a0.digits[i]=r.digits[i];
				a0.size=r.size;		
				free(r.digits);				
			}
			free(va.digits);
			free(c.digits);
		}
		return a0;
	}
}