
#include <stdio.h>
#include <stdlib.h>
int* read_number(char* fname_in,char* bin,int &size,bool &sign); //+
int get_size(char* fname_in,char* bin, bool &sign);				//+
void reverse_num(int* &num,int size);							//-
bool is_zero(int* b,int size_b);								//-
void div2(int* a,int n);										//-
int* pow(int* a,int* b,int* mod,int size_a,int size_b,int size_m, bool sign_a,bool sign_b,bool sign_m,bool &sign_result,int &size_result); //+
int* summ(int* a,int* b,int size_a,int size_b,bool sign_a,bool sign_b,bool &sign_result,int &size_result); //+
int* sub(int* a,int* b,int size_a,int size_b,bool sign_a,bool sign_b,bool &sign_result,int &size_result); //+
int* mul(int* a,int* b,int size_a,int size_b,bool sign_a,bool sign_b,bool &sign_result,int &size_result); //+
int* div(int* a,int* b,int size_a,int size_b,bool sign_a,bool sign_b,bool &sign_result,int &size_result,bool act); //+
void write_number(char* fname,int *num,int size,bool sign,char *bin);				//+
bool is_bigger(int* b,int size_b,int* a,int size_a);			//-
bool is_equal(int* b,int size_b,int* a,int size_a);				//-

class BigInt
{
public:
	int* num;
	int size;
	bool sign;

	int get(char* fname_in,char* bin)
	{
		size = get_size(fname_in,bin,sign);
		num = read_number(fname_in,bin,size,sign);
		return size;
	}
	void save(char* fname_in,char* bin)
	{
		write_number(fname_in,num,size,sign,bin);
	}
	friend const BigInt operator+(BigInt const a, BigInt const b)
	{
		BigInt res;
		res.num = summ(a.num,b.num,a.size,b.size,a.sign,b.sign,res.sign,res.size);
		return res;
	}
	friend const BigInt operator-(BigInt const & a, BigInt const & b)
	{
		BigInt res;
		res.num = sub(a.num,b.num,a.size,b.size,a.sign,b.sign,res.sign,res.size);
		return res;
	}
	friend const BigInt operator*(BigInt const & a, BigInt const & b)
	{
		BigInt res;
		res.num = mul(a.num,b.num,a.size,b.size,a.sign,b.sign,res.sign,res.size);
		return res;
	}
	friend const BigInt operator/(BigInt const & a, BigInt const & b)
	{
		BigInt res;
		res.num = div(a.num,b.num,a.size,b.size,a.sign,b.sign,res.sign,res.size,0);
		return res;
	}
	friend const BigInt operator%(BigInt const & a, BigInt const & b)
	{
		BigInt res;
		res.num = div(a.num,b.num,a.size,b.size,a.sign,b.sign,res.sign,res.size,1);
		return res;
	}
	BigInt& operator=( BigInt const &a ) 
	{
		size=a.size;
		sign = a.sign;
		num = a.num;
		return *this;
	}
	BigInt()
	{
		num = NULL;
		sign = 0;
		size = 0;
	}



};

BigInt power(BigInt a,BigInt b,BigInt mod)
	{
		BigInt a1;
		a1 = a;
		a1.num = new int[a.size];
		for(int i=0;i<a.size;++i)
		{
			a1.num[i]=a.num[i];
			//printf("%d",a1.num[i]);
		}
		//printf("\n");
		BigInt b1;
		b1 = b;
		b1.num = new int[b.size];
		for(int i=0;i<b.size;++i)
		{
			b1.num[i]=b.num[i];
			//printf("%d",b1.num[i]);
		}
		//printf("\n");
		BigInt mod1;
		mod1 = mod;
		mod1.num = new int[mod.size];
		for(int i=0;i<mod.size;++i)
		{
			mod1.num[i]=mod.num[i];
			//printf("%d",mod1.num[i]);
		}
		//printf("\n");
		BigInt res;
		if(b.sign)
			printf("\nError: You can not use a negative power\n");
		else
			res.num = pow(a1.num,b1.num,mod1.num,a1.size,b1.size,mod1.size, a1.sign,b1.sign,mod1.sign,res.sign,res.size);
		return res;
	}
int* read_number(char* fname_in,char* bin,int &size,bool &sign)
{
	size = get_size(fname_in,bin,sign);
	if(!size)
	{
		printf("Error: empty file %s",fname_in);
		return NULL;
	}
	
	int* num = NULL;
	num=(int*) malloc((size + 2)*sizeof(int));
	for(int i=0;i<size;++i)
		num[i]=0;
	if(bin[1]!='b')
	{
		FILE* f=fopen(fname_in,"r");
		if(f==NULL)
		{
			printf("\nError: can't read %s\n",fname_in);
			return NULL;
		}
		char c;
		if(sign)
			fscanf(f,"%c",&c);
		for (int i=0;i<size;++i)
		{
			fscanf(f,"%c",&c);
			num[i]=c-'0';
		}
		fclose(f);
		reverse_num(num,size);
		while (num[size-1] == 0)
		--size;
		return num;
	}
	else
	{
		FILE * f = fopen( fname_in , "rb" );
 
		if(f==NULL)
		{
			printf("\nError: can't read %s\n",fname_in);
			return 0;
		}
		 
		char c;
		if(sign)
			fread(&c, sizeof(char), 1, f);
		for(int i=0;i<size;++i)
		{
			fread(&c, sizeof(char), 1, f);
			num[i]=c-'0';
		}
		fclose(f);
		reverse_num(num,size);
		while (num[size-1] == 0)
			--size;
		return num;
	}
	


}

int get_size(char* fname_in,char* bin, bool &sign)
{
	if(bin[1]!='b')
	{
		FILE* f=fopen(fname_in,"r");
		if(f==NULL)
		{
			printf("\nError: can't read %s\n",fname_in);
			return 0;
		}
		char c;
		int i=0;
		if(fscanf(f,"%c",&c)==-1)
			return 0;
		if (c>='0' && c<='9')
			++i;
		else
			if(c=='-')
				sign=1;
			else
			{
				printf("\nError: Incorrect data %s\n",fname_in);
				fclose(f);
				return 0;
			}
		while(fscanf(f,"%c",&c)!=-1)
		{
			if (c>='0' && c<='9')
				++i;
			else
			{
				printf("\nError: Incorrect data %s\n",fname_in);
				return 0;
			}
		}
		fclose(f);
		return i;
	}
	else
	{
		FILE * f = fopen( fname_in , "rb" );
 
		if(f==NULL)
		{
			printf("\nError: can't read %s\n",fname_in);
			return 0;
		}
		 
		  // определяем размер файла
		  fseek(f , 0 , SEEK_END);                          // устанавливаем позицию в конец файла
		  long lSize = ftell(f);                            // получаем размер в байтах
		  rewind (f);  
		  char c=0;
		  int i=0;
		  fread(&c, sizeof(char), 1, f);
		  if(c=='-')
		  {
			  sign=1;
			  --lSize;
		  }
		  else
			if(!(c>='0' && c<='9'))
			{
				printf("\nError: Incorrect data %s\n",fname_in);
				return 0;
			}
		for(int i=0;i<lSize;++i)
		{
			fread(&c, sizeof(char), 1, f);
			if(!(c>='0' && c<='9'))
			{
				printf("\nError: Incorrect data %s\n",fname_in);
				return 0;
			}
		}
			fclose(f);
			return lSize;
		
	}

}


void reverse_num(int* &num,int size)
{
	int buff;
	for(int i=0;i<size/2;++i)
	{
		buff=num[i];
		num[i]=num[size-i-1];
		num[size-i-1]=buff;
	}
	
}


int* summ(int* a,int* b,int size_a,int size_b,bool sign_a,bool sign_b,bool &sign_result,int &size_result)
{
	int* res;
	if(sign_a==sign_b)
	{
		size_result=size_a+2;
		if(size_a<size_b)
		{
			size_result=size_b+2;
		}
		res = (int*) malloc((size_result + 2)*sizeof(int));
		int* a1 = (int*) malloc((size_result + 2)*sizeof(int));
		int* b1 = (int*) malloc((size_result + 2)*sizeof(int));
		for(int i=0;i<size_result;++i)
			res[i]=0;
		for(int i=0;i<size_result;++i)
			b1[i]=0;
		for(int i=0;i<size_result;++i)
			a1[i]=0;
		for(int i=0;i<size_b;++i)
			b1[i]=b[i];
		for(int i=0;i<size_a;++i)
			a1[i]=a[i];

		sign_result=sign_a;
		for(int i=0;i<size_result;++i)
		{
			res[i]+=a1[i]+b1[i];
			if(res[i]>9)
			{
				res[i]%=10;
				res[i+1]+=1;
			}
		}
		free(a1);
		free(b1);
	}
	else if(sign_a)
		res = sub(a,b,size_a,size_b,1,1,sign_result,size_result);
	else
		res = sub(a,b,size_a,size_b,0,0,sign_result,size_result);

	while (res[size_result-1] == 0)
		size_result-- ;
	return res;
}

void write_number(char* fname,int *num,int size,bool sign,char *bin)
{
	if(bin[1]!='b')
	{
		FILE *f=fopen(fname,"w");
		if(f==NULL)
		{
			printf("Error: can't write to %s",fname);
			return;
		}
		if(sign)
			fprintf(f,"-");
		reverse_num(num,size);
		for(int i=0;i<size;++i)
			fprintf(f,"%d",num[i]);
		fclose(f);
	}
	else
	{

		FILE* f=fopen(fname,"wb");
		if(f==NULL)
		{
			printf("Error: can't write to %s",fname);
			return;
		}
		char c='-';
		if(sign)
			fwrite(&c,sizeof(char),1,f);
		reverse_num(num,size);
		for(int i=0;i<size;++i)
		{
			c=num[i]+'0';
			fwrite(&c,sizeof(char),1,f);
		}
		fclose(f);
	}

}


int* sub(int* a,int* b,int size_a,int size_b,bool sign_a,bool sign_b,bool &sign_result,int &size_result)
{
	int* res;
	if(sign_a == sign_b)
	{
		size_result=size_a+1;
		if(size_a<size_b)
		{
			size_result=size_b+1;
		}

		res = (int*) malloc((size_result + 2)*sizeof(int));
	
		int* a1 = (int*) malloc((size_result + 2)*sizeof(int));
		int* b1 = (int*) malloc((size_result + 2)*sizeof(int));
		for(int i=0;i<size_result;++i)
			res[i]=0;
		for(int i=0;i<size_result;++i)
			b1[i]=0;
		for(int i=0;i<size_result;++i)
			a1[i]=0;
		for(int i=0;i<size_b;++i)
			b1[i]=b[i];
		for(int i=0;i<size_a;++i)
			a1[i]=a[i];

		sign_result=sign_a;
		if(is_bigger(b,size_b,a,size_a))
		{
			int* buff=b1;
			b1=a1;
			a1=buff;
			int buf=size_b;
			size_b=size_a;
			size_a=buf;
			sign_result=!sign_result;
		}
		for(int i=0;i<size_result;++i)
		{
			res[i]+=a1[i]-b1[i];
			if(res[i]<0 && i<size_result-1)
			{
				res[i]+=10;
				res[i+1]-=1;
			}
		}
		free(a1);
		free(b1);
	}
	else if(sign_a)
		res = summ(a,b,size_a,size_b,1,1,sign_result,size_result);
	else
		res = summ(a,b,size_a,size_b,0,0,sign_result,size_result);
	while (res[size_result-1] == 0)
		size_result-- ;
	
	return res;

}



bool is_bigger(int* b,int size_b,int* a,int size_a)
{
	while (a[size_a-1] == 0)
		size_a-- ;
	while (b[size_b-1] == 0)
		size_b-- ;

		if(size_a<size_b)
			return true;
		if(size_a>size_b)
			return false;
		
		for(int i =size_a-1;i>-1;--i)
			if(b[i]>a[i])
				return true;
			else if (b[i]<a[i])
				return false;
		
		return false;
}


int* mul(int* a,int* b,int size_a,int size_b,bool sign_a,bool sign_b,bool &sign_result,int &size_result)
{
	size_result = size_a + size_b + 1;
	int* res = (int*) malloc((size_result + 2)*sizeof(int));
	if(sign_a && sign_b)
		sign_result=0;
	else
		sign_result= sign_a || sign_b;

	for(int i=0;i<size_result;++i)
		res[i]=0;
	for (int ix = 0; ix < size_a; ix++)
	  for (int jx = 0; jx < size_b; jx++)
		 res[ix + jx] += a[ix] * b[jx];
 
	for (int ix = 0; ix < size_result; ix++)
	{
		res[ix + 1] +=  res[ix] / 10;
		res[ix] %= 10;
	}
	 
	while (res[size_result-1] == 0)
		size_result-- ;
	return res;
}




int* div(int* a,int* b,int size_a,int size_b,bool sign_a,bool sign_b,bool &sign_result,int &size_result,bool act)
{
	int* res;
	if(is_bigger(b,size_b,a,size_a))
	{
		if(!act)
		{
			size_result=1;
			res=(int*) malloc((size_result + 2)*sizeof(int));
			res[0]=0;
			res[1]=0;
			return res;
		}
		else
			{
			if(sign_a && sign_b)
				sign_result=1;
			if(!sign_a && sign_b)
				sign_result=0;
			size_result=size_a;
			res=(int*) malloc((size_result + 2)*sizeof(int));
			for(int i=0;i<size_result;++i)
				res[i]=a[i];
			return res;
		}
	}
	reverse_num(a,size_a);
	int size_q=size_a;
	int* q=(int*) malloc((size_q + 2)*sizeof(int));
	int size_cur=0;
	int* cur=(int*) malloc((size_q + 2)*sizeof(int));
	int size_r=size_b;
	for(int i=0;i<size_a;++i)
	{
		q[i]=0;
		cur[i]=0;
	}
	if(sign_a && sign_b)
		sign_result=0;
	else
		sign_result= sign_a || sign_b;
	int iq=0;
	for(int i=0;i<size_a;++i)
	{
		cur[size_cur]=a[i];
		
		++size_cur;
		
		reverse_num(cur,size_cur);
		if(is_bigger(b,size_b,cur,size_cur))
		{
			iq++;
			reverse_num(cur,size_cur);
			continue;
		}
		bool sign=0;
		
		while((is_bigger(cur,size_cur,b,size_b) || is_equal(cur,size_cur,b,size_b)) && !sign)
		{
			++q[iq];
			int aaa=size_cur;
			
			int* new_cur=sub(cur,b,aaa,size_b,0,0,sign,size_cur);
			free(cur);
			cur=new_cur;
			

		}
		++iq;
		reverse_num(cur,size_cur);
		
	}
	size_q=iq;
	reverse_num(cur,size_cur);	
	reverse_num(q,size_q);
	while (q[size_q-1] == 0)
		size_q-- ;
	while (cur[size_cur-1] == 0)
		size_cur-- ;
	if(act)
	{
		if(sign_a && sign_b)
				sign_result=1;
		if(!sign_a && sign_b)
				sign_result=0;
		free(q);
		res=cur;
		size_result=size_cur;
	}
	else
	{
		free(cur);
		res=q;
		size_result=size_q;
	}
  return res;
}



bool is_equal(int* b,int size_b,int* a,int size_a)
{

		while (a[size_a-1] == 0)
		size_a-- ;
	while (b[size_b-1] == 0)
		size_b-- ;


		if(size_a!=size_b)
			return false;
		
		for(int i =size_a-1;i>-1;--i)
			 if (b[i]!=a[i])
				return false;
		
		return true;
}


void div2(int* a,int n)
{
	for(int i=0;i<n;++i)
	{
		a[i]/=2;
		if(a[i+1]%2)
			a[i]+=5;
	}
}


int* pow(int* a,int* b,int* mod,int size_a,int size_b,int size_m, bool sign_a,bool sign_b,bool sign_m,bool &sign_result,int &size_result)
{
	int* new_a=div(a,mod,size_a,size_m,sign_a,sign_m,sign_a,size_a,1);

	free(a);
	a=new_a;
	
	int* res = (int*) malloc((1 + 2)*sizeof(int));
	int* ed = (int*) malloc((1 + 2)*sizeof(int));
	res[0]=1;
	res[1]=0;
	size_result=1;
	ed[0]=1;
	ed[1]=0;
	while (!is_zero(b,size_b))
	{
		if (b[0]%2==1) 
		{
			int* new_res = mul(res,a,size_result,size_a,sign_result,sign_a,sign_result,size_result);
			free(res);
			
			res=new_res;
			
			new_res=div(res,mod,size_result,size_m,sign_result,sign_m,sign_result,size_result,1);
			free(res);
			res=new_res;
		
			int* new_b=sub(b,ed,size_b,1,0,0,sign_b,size_b);
			free(b);
			b=new_b;
		}
		else 
		{
			new_a = mul(a,a,size_a,size_a,sign_a,sign_a,sign_a,size_a);
			
			free(a);
			a=new_a;
			new_a=div(a,mod,size_a,size_m,sign_a,sign_b,sign_a,size_a,1);
			
			free(a);
			a=new_a;
			div2(b,size_b);
		}
		
	}
	return res;
}


bool is_zero(int* b,int size_b)
{
	for(int i=0;i<size_b;++i)
		if(b[i])
			return false;
	return true;
}
//новые функции для питона
extern "C" {
    __declspec(dllexport) BigInt*  BigInt_new(){ return new BigInt(); }//нрвый объект такого типа
   __declspec(dllexport) void BigInt_get(BigInt* num,char* fname_in,char* bin ){ num->get(fname_in,bin);}//чтение
  __declspec(dllexport)  void BigInt_save(BigInt* num,char* fname_in,char* bin ){ num->save(fname_in,bin);}
  __declspec(dllexport)  BigInt* BigInt_power(BigInt* a,BigInt* b,BigInt*  mod){BigInt* ff=new BigInt(); ff[0]= power(a[0],b[0],mod[0]);return ff;}
  __declspec(dllexport)  BigInt* BigInt_add(BigInt* a,BigInt* b){ BigInt* ff=new BigInt();ff[0]=a[0] + b[0];return ff;}
  __declspec(dllexport)  BigInt* BigInt_sub(BigInt* a,BigInt* b){ BigInt* ff=new BigInt();ff[0]=a[0] - b[0];return ff;}
  __declspec(dllexport) BigInt* BigInt_mul(BigInt* a,BigInt* b){ BigInt* ff=new BigInt();ff[0]=a[0] * b[0];return ff;}
  __declspec(dllexport) BigInt* BigInt_div(BigInt* a,BigInt* b){ BigInt* ff=new BigInt();ff[0]=a[0] / b[0];return ff;}
  __declspec(dllexport) BigInt* BigInt_mod(BigInt* a,BigInt* b){ BigInt* ff=new BigInt();ff[0]=a[0] % b[0];return ff;}
}
