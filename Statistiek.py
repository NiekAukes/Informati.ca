rij = input("Voer een rij getallen in.")       #Vraagt om een input bij de variabele 'row' (rij)
rij = rij.split()   #split de rij in aparte variabelen
gezien = {}           #Maakt een dictionary met naam 'seen'
for i in range(len(rij)): #Telt voor elke i in de range van de lengte van row
    rij[i] = float(rij[i]) #maakt van elke variabele een float
    if rij[i] not in gezien: #Maakt een unieke lijst met de niet geziene variabelen
        gezien[rij[i]] = 0
    gezien[rij[i]] += 1
rij = sorted(rij) #Sorteert de rij voor de mediaan
print("Minimum:", min(rij))
print("Maximum:", max(rij))
print("Gemiddelede:", sum(rij) / len(rij))
print("Mediaan:", (rij[int((len(rij)-1) // 2)] + rij[(len(rij)) // 2]) / 2) #Probeert een mediaan te vinden
j = max(gezien.values())
for i in gezien:
    if gezien[i] == j:
        print("Modus:",i)
#test case: (10 10 6 7 3 33 2 3 9 9 60 54 34 12), (50 5000 392 56758 300 9000 23 1 65 343 2355 3245 2222 998), (10 8 8 4)