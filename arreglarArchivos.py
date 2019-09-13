f=open("log10KB", "r")
lineas = []
n = 10000

for i in range(n):
    linea = f.readline()
    lineas.append(linea)

f.close()

f2=open("log10KBfixed","w+")
for i in range(n):
    f2.write(lineas[i])

f2.close()
