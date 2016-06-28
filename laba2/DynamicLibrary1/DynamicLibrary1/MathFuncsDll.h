#ifdef MATHFUNCSDLL_EXPORTS
#define MATHFUNCSDLL_API __declspec(dllexport) 
#else
#define MATHFUNCSDLL_API __declspec(dllimport) 
#endif

namespace MathFuncs
{
    class LongInt
    {
	private:
		long size;
		bool minus;
		int *digits;
		bool TakeSize (char *flag, char *fname);
		bool ReadFile(char *flag, char *fname);
		void MATHFUNCSDLL_API DelNull();
		bool More (LongInt a, LongInt b);
		bool Equally(LongInt a, LongInt b);
		LongInt Sum(LongInt a, LongInt b, LongInt &c);
		LongInt Sub(LongInt a, LongInt b, LongInt &c);
		LongInt Addition(LongInt a, LongInt b, LongInt &c);
		LongInt Mul(LongInt a, LongInt b, LongInt &c);
		void LightDiv(LongInt a, int b, LongInt q, int &r);
		void Div(LongInt a, LongInt b, LongInt &q, LongInt &r, LongInt c, LongInt m);
		void Division(LongInt a, LongInt b, LongInt &q, LongInt &r, LongInt c, LongInt m);
		LongInt DecToBin(LongInt a);
    public: 		
		MATHFUNCSDLL_API LongInt();
		MATHFUNCSDLL_API ~LongInt();
		bool MATHFUNCSDLL_API Read(char *flag, char *fname);
		bool MATHFUNCSDLL_API operator>(const LongInt& B);
		bool MATHFUNCSDLL_API operator<(const LongInt& B);
		bool MATHFUNCSDLL_API operator>=(const LongInt& B);
		bool MATHFUNCSDLL_API operator<=(const LongInt& B);
		bool MATHFUNCSDLL_API operator==(const LongInt& B);
		bool MATHFUNCSDLL_API operator!=(const LongInt& B);
		LongInt MATHFUNCSDLL_API operator+(const LongInt& a);
		LongInt MATHFUNCSDLL_API operator-(const LongInt& a);
		LongInt MATHFUNCSDLL_API operator*(const LongInt& a);
		LongInt MATHFUNCSDLL_API operator/(const LongInt& a);
		LongInt MATHFUNCSDLL_API operator%(const LongInt& a);
		LongInt MATHFUNCSDLL_API Pow (LongInt a, LongInt b, LongInt mod);
		void MATHFUNCSDLL_API Print();
		bool MATHFUNCSDLL_API WriteFile (char *flag, char *fname);
    };
}