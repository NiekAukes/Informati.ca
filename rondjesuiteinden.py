word = input("Vul uw woord(en) in HOOFDLETTERS in a.u.b. \n\n\n(Alvast bedankt!)") ##asks for input
rondjes = 0
uiteinden = 0 #maakt variabelen aan
check = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" #Alle letters van het alfabet, dit wordt gebruikt om de rondjes en uiteinden te checken.
circle = [1, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0] #Correspondeert met de alfabetlijst
ends =   [2, 0, 2, 0, 3, 3, 2, 4, 2, 2, 4, 2, 2, 2, 0, 1, 2, 2, 2, 3, 3, 2, 2, 4, 3, 2] #Idem
for i in word: #Neemt elk karakter van de input apart
    u = check.find(i) #Probeert om een karakter van de lijst check te vinden
    if u != -1: #Hiermee skipt de code de karakters die niet in de lijst check staan (speciale karakter, spaties, etc.)
        rondjes += circle[u] #Telt de rondjes bij de output op
        uiteinden += ends[u] #Telt de uiteinden bij de output op
print(word) #Print het originele woord
print("Rondjes:", rondjes) #Print de rondjes
print("Uiteinden:", uiteinden) #Print de uiteinden