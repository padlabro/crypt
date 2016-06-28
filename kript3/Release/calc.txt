	
f = open("x.txt") 
g = open("y.txt") 
h = open("mod.txt") 
x=int(f.read()) 
y=int(g.read()) 
z=int(h.read())
s=x+y
print '\n\n+ :\n'+str(s)
s=x-y
print '\n\n- :\n'+str(s)
s=x*y
print '\n\n* :\n'+str(s)
s=x/y
print '\n\n/ :\n'+str(s)
s=x % y
print '\n\n| :\n'+str(s)
s=pow(x, y,z) 
print '\n\n ^ & | :\n'+str(s)