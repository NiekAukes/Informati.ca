pfNum = input("vul een getal in: ")
pfNum = int(pfNum)
d = [] 
for i in range(pfNum-1): 
    Pfnum = float(pfNum) 
    if float(pfNum/(i+1)) == Pfnum/(i+1): 
        d.append(i+1) 
if sum(d) == pfNum: 
    print("Het nummer", pfNum , "is een perfect getal")
    print("Delers:")
    for i in d:
        print(i)
else:
    print("Het getal ",pfNum," is niet een perfect getal.")
