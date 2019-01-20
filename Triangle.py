try:
    abc = input("Voer drie getallen in voor kant a, b en c\nVolgorde van getallen: a, b, c")  #Vraagt om een input voor abc
    abc = abc.split() #split de getallen in een lijst

    for i in range(3):
        abc[i] = int(abc[i]) #Maakt elke variabele in de lijst abc een integer
    T = True #Declareert een flag (signaal voor een functie of proces)

    for i in range(3):
        if abc[i] >= abc[i-1] + abc[i-2]: #Als één value groter is dan de combinatie van de andere twee, is een driehoek niet mogelijk.
            print("Driehoek is niet mogelijk met de getallen", abc[i],",", abc[i-1],"en",abc[i-2])
            T = False
            break
    if T: #als het wel mogelijk is om een driehoek te maken:
        print("Driehoek mogelijk met de getallen", abc[i], ",", abc[i-1], "en", abc[i-2])
        for i in range(3):
            if abc[i] == abc[i-1] == abc[i-2]: #Zoekt voor een speciale driehoek (alle zijden 60 graden)
                print("Dit is een superspeciale driehoek :D")
                break
            if abc[i] == abc[i-1]:
                print("Dit is een gelijkbenige driehoek :)")
                break

except (ValueError, TypeError):
    print("Ongeldige input. Programma gestopt.\nRun de code overnieuw om opnieuw te proberen.") #Als je geen int invoert stopt het de code.

#test case: (20 10 11), (30 18 18), (12 3 4)