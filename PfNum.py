pfNum = input("vul een getal in: ")
pfNum = int(pfNum)
d = [] #Maakt lijst aan
for i in range(pfNum-1): #Neemt elk nummer kleiner dan de input
    Pfnum = float(pfNum) #Declareert een variabele gelijk aan de input
    if float(pfNum/(i+1)) == Pfnum/(i+1): #Als het getal even is, is het een echte deler
        d.append(i+1) #Voegt het nummer toe aan de delerlijst
if sum(d) == pfNum: #Als de som van de delers gelijk is aan het getal, is het een perfect getal.
    print("Het nummer", pfNum , "is een perfect getal")
    print("Delers:")
    for i in d:
        print(i)
else:
    print("Het getal ",pfNum," is niet een perfect getal.")