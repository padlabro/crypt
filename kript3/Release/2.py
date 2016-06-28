import sys 
from ctypes import cdll
lib = cdll.LoadLibrary('./2.so')
class BigInt(object):
	def __init__(self):
		self.obj = lib.BigInt_new()
	def get(self,filename,bin):
		lib.BigInt_get(self.obj,filename,bin)
	def save(self,filename,bin):
		lib.BigInt_save(self.obj,filename,bin)
	def power(self,a,p,mod):
		self.obj = lib.BigInt_power(a.obj,p.obj,mod.obj)
	def __add__(self, other):
		self.obj =  lib.BigInt_add(self.obj,other.obj)
	def __sub__(self, other):
		self.obj =  lib.BigInt_sub(self.obj,other.obj)
	def __mul__(self, other):
		self.obj =  lib.BigInt_mul(self.obj,other.obj)
	def __div__(self, other):
		self.obj =  lib.BigInt_div(self.obj,other.obj)
	def __mod__(self, other):
		self.obj =  lib.BigInt_mod(self.obj,other.obj)



if __name__ == "__main__":
	
	if len(sys.argv)<6:
		print "Need more parameters"
		raise SystemExit(1)
	f = BigInt()
	f.get(sys.argv[1],sys.argv[5])
	g = BigInt()
	g.get(sys.argv[3],sys.argv[5])
	b=0
	if sys.argv[2]=='+':
		f+g
		b=1
	if sys.argv[2]=='-':
		f-g
		b=1
	if sys.argv[2]=='*':
		f*g
		b=1
	if sys.argv[2]=='/':
		f/g
		b=1
	if sys.argv[2]=='%':
		f%g
		b=1
	if sys.argv[2]=='^':
		if len(sys.argv)>6:
			mod = BigInt()
			mod.get(sys.argv[6],sys.argv[5])
		else:
			print "Need more parameters"
			raise SystemExit(1)
		f.power(f,g,mod)
		b=1
	if b==0:
		print "Unknown operator"+ sys.argv[2]
	else:
		f.save(sys.argv[4],sys.argv[5])