names = input("Vul de namen in: ") #Vraagt om namen
seen = {} #Maakt er een 'dictionary'(dubbele lijst) van. (Namen en Keren Gezien)
uniq = 0 #Declareert een variabele
namelist = names.split() #Split namen naar een lijst
_namelist = [] #Maakt een parallele lijst aan
for i in namelist: #Vraagt alle namen apart van elkaar
    if i not in seen: #Als de naam nog niet gezien is,
        seen[i] = 0  #...creeërt het een value daarvoor
        _namelist.append(i) #.. en voegt het aan de parallele lijst toe
        uniq += 1
    seen[i] += 1 #Als het gezien is, voegt het 1 toe aan de frequentie van 'gezien', zodat het programma het herkent als gezien.
_namelist = sorted(_namelist) #sorteert de lijst
for i in _namelist: #Neemt alle namen apart,
    print(i,'-',seen[i]) #print de frequentie van die naam uit,
print("Er zijn", uniq,"verschillende namen.") #... en print de uniekheid van die namen uit.