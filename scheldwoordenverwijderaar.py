lijstjenummero2 = input("Voer een lijstje in").split()
scheldwoorden = ["kut", "godverdomme", "fuck"]
scheldwoord = ""
##def geenScheldwoorden(lijstje):
##    for i in lijstje:
##        if lijstje[i] == scheldwoord:
##            lijstje[i] = len(lijstje[i]) * "*"
##
##print(geenScheldwoorden(lijstjenummero2))
def scheldwoordenverwijderaar1(lijstje):
    for i in lijstje:
            if i in scheldwoorden:
                scheldwoord = len(i) * "*"
                return scheldwoord
            elif i not in scheldwoorden:
                return i
                continue
            else:
                return ""
                continue

antwoord = []
for i in lijstjenummero2:
    if i in scheldwoorden:
        scheldwoord = len(i) *"*"
        antwoord.append(scheldwoord)
    elif i not in scheldwoorden:
        antwoord.append(i)
    else:
        continue
print(antwoord)