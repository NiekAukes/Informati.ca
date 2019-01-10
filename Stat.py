row = input("Voer een rij getallen in.")       #Vraagt om een input bij de variabele 'row' (rij)
row = row.split()   #split de rij in aparte variabelen
seen = {}           #Maakt een dictionary met naam 'seen'
Db = {}             #maakt een dictionary aan voor de freqentie met naam 'Db'
for i in range(len(row)): #Telt voor elke i in de range van de lengte van row
    row[i] = float(row[i]) #maakt van elke variabele een float
    if row[i] not in seen: #Maakt een unieke lijst met de niet geziene variabelen
        seen[row[i]] = 0
    seen[row[i]] += 1
for num, hz in seen.items(): #Maakt een unieke lijst van frequenties met hun frequentie
    if hz not in Db:        #Als de frequentie niet in de dictionary met frequenties zit...
        Db[hz] = 0          #krijgt het een variabele nul
    Db[hz] += 1             #voor elke nummer in seen krijgt Db er 1 bij
row = sorted(row) #Sorteert de rij voor de mediaan
print("Minimum:", min(row))
print("Maximum:", max(row))
print("Gemiddelede:", sum(row) / len(row))
print("Mediaan:", (row[int((len(row)-1) // 2)] + row[(len(row)) // 2]) / 2) #Probeert een mediaan te vinden
if Db[max(Db.keys())] > 1: #Als de maximale frequentie van Db groter is dan 1
    print("Modus niet mogelijk")
else:
    print("Modus:", seen[max(seen.keys())])

