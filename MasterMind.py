import random
import os
CurrentRound = 0
AantalCodes = 6
AantalRondes = 20
Antwoord = []
Rondes = []
Feedback = []
goedetekens = "012345"
geraden = []
geenDuplicates = input("wel of geen zelfde cijfers? y/n: ")
for i in range(AantalRondes * 4):
    Rondes.append("#")
    Feedback.append("-")
#Creëert random nummers:
while(len(Antwoord) < 4):
    k = round(random.randint(0,AantalCodes - 1))
    if geenDuplicates == "y":
        if (k not in Antwoord):
            Antwoord.append(k)
    else:
        Antwoord.append(k)
#print(Antwoord)
def Input(gok):
    gok = gok.split()
    for i in range(len(gok)):
        if gok[i] in goedetekens:
            gok[i] = int(gok[i])
            Rondes[i + (CurrentRound * 4)] = gok[i]
        else:
            gok[i] = None

def GeefFeedback(cijfers):
    #input: lijst met 4 cijfers
    #output: aantal goede cijfers en goed geplaatste cijfers
    output = []
    k = []

    colorset = set(Antwoord) #maakt een set voor de codes om te checken of de code in het antwoord zit
    for i in range(4):
        #kijkt of er correcte picks zijn
        if (cijfers[i] == Antwoord[i]):
            output.append("#")
            geraden.append(cijfers[i])
            cijfers[i] = None
        elif (cijfers[i] in colorset):
            k.append("&")
            geraden.append(cijfers[i])
            for i in range(4):
                if cijfers[i] in geraden and cijfers[i] != None:
                        cijfers[i] = None
    return output + k

def Win():
    print("U heeft gewonnen")
    input("press enter to continue...")

def TekenSpeelgebied():
    os.system('cls')
    print("   ==============================")
    for i in range(AantalRondes):
        print(
        "   ",
        i + 1,
        "\t| ",
        Rondes[0 + i * 4],
        Rondes[1 + i * 4],
        Rondes[2 + i * 4],
        Rondes[3 + i * 4],
        " | ",
        Feedback[0 + i * 4],
         Feedback[1 + i * 4],
         Feedback[2 + i * 4],
         Feedback[3 + i * 4],
        " |")
    print("   ==============================")

TekenSpeelgebied()
while(CurrentRound < AantalRondes):
    gok = input("Geef een gokje voor ronde " + str(CurrentRound + 1) + ": ")
    Input(gok)
    f = GeefFeedback([Rondes[0 + (CurrentRound * 4)], Rondes[1 + (CurrentRound * 4)] ,Rondes[2 + (CurrentRound * 4)] ,Rondes[3 + (CurrentRound * 4)]])
    counter = 0
    for i in range(len(f)):
        Feedback[i + (CurrentRound * 4)] = f[i]
        if (f[i] == "#"):
            counter += 1
    TekenSpeelgebied()
    if (counter == 4):
        Win()
        CurrentRound = 49
    CurrentRound += 1
print(Antwoord)
input("press enter to continue...")