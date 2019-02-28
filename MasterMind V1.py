import random
CurrentRound = 0
Antwoord = []
Rondes = []
Feedback = []
AantalGoed = []
Klaar = False
slechtetekens = ",. ?!qwertyuiopasdfghjklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM |{}[]\`~"
for i in range(40): #Vult de lijsten met lege ruimte zodat de ASCII GUI (dat mooie ding met al die hashes en lijntjes) niet crasht.
    Rondes.append("#")
    Feedback.append("-")
    AantalGoed.append(0)
for i in range(4): #Creëert random nummers
    Antwoord.append(round(random.randint(0,5)))

def RefreshNumbersAndCheckIfWon():
    ronde1 = Rondes[0:3]
    ronde2 = Rondes[4:7]
    ronde3 = Rondes[8:11]
    ronde4 = Rondes[12:15]
    ronde5 = Rondes[16:19]
    ronde6 = Rondes[20:23]
    ronde7 = Rondes[24:27]
    ronde8 = Rondes[28:31]
    ronde9 = Rondes[32:35]
    ronde10 = Rondes[36:39]
    if Rondes[0:3] != Antwoord[0:3] and Rondes[4:7] != Antwoord[0:3] and Rondes[8:11] != Antwoord[0:3] and Rondes[12:15] != Antwoord[0:3] and Rondes[16:19] != Antwoord and Rondes[20:23] != Antwoord[0:3] and Rondes[24:27] != Antwoord[0:3] and Rondes[28:31] != Antwoord[0:3] and Rondes[32:35] != Antwoord[0:3] and Rondes[36:39] != Antwoord[0:3]:
        Klaar = True

def ZetGokInTabel(gok):
    gok = gok.split()
    for i in range(len(gok)):
        if gok[i] in slechtetekens:
            gok[i] = ""
        elif gok[i] not in slechtetekens:
            gok[i] = int(gok[i])
            Rondes[i + ((CurrentRound - 1) * 4)] = gok[i]

def GeefFeedback(cijfers): #INPUT EEN LIJST
    #input: LIJST met 4 cijfers
    #output: aantal goede cijfers en goed geplaatste cijfers
    output = []
    integerset = set(Antwoord) #maakt een set voor de nummers van het antwoord om te checken of het antwoord in de gok zit
    for i in range(len(cijfers)):
        OnPlaceCurrentCode = 0 #flag voor correcte plaats
        OnCurrentCode = 0 #flag voor correcte kleur
        #kijkt of er correcte picks zijn
        if cijfers[i] == Antwoord[i]:
            Feedback[i + ((CurrentRound - 1)* 4)] = 1
        elif cijfers[i] in Antwoord[0:3]:
            AantalGoed[i + ((CurrentRound - 1)* 4)] = 1
    return Feedback

def TekenSpeelgebied():
    print("                   Nummer in Antwoord | Aantal Goed")
    print("        ===========================================")
    print("Ronde 1 | ",Rondes[0], Rondes[1] ,Rondes[2] ,Rondes[3]," | ",Feedback[0], Feedback[1], Feedback[2], Feedback[3]," |",sum(AantalGoed[0:3])," |")               #Ronde 1
    print("Ronde 2 | ",Rondes[4], Rondes[5] ,Rondes[6] ,Rondes[7]," | ", Feedback[4], Feedback[5], Feedback[6], Feedback[7]," |",sum(AantalGoed[4:7])," |")              #Ronde 2
    print("Ronde 3 | ",Rondes[8], Rondes[9] ,Rondes[10] ,Rondes[11]," | ", Feedback[8], Feedback[9], Feedback[10], Feedback[11]," |",sum(AantalGoed[8:11])," |")         #Ronde 3
    print("Ronde 4 | ",Rondes[12], Rondes[13] ,Rondes[14] ,Rondes[15]," | ", Feedback[12], Feedback[13], Feedback[14], Feedback[15]," |",sum(AantalGoed[12:15])," |")    #Ronde 4
    print("Ronde 5 | ",Rondes[16], Rondes[17] ,Rondes[18] ,Rondes[19]," | ", Feedback[16], Feedback[17], Feedback[18], Feedback[19]," |",sum(AantalGoed[16:19])," |")    #Ronde 5
    print("Ronde 6 | ",Rondes[20], Rondes[21] ,Rondes[22] ,Rondes[23]," | ", Feedback[20], Feedback[21], Feedback[22], Feedback[23]," |",sum(AantalGoed[20:23])," |")    #Ronde 6
    print("Ronde 7 | ",Rondes[24], Rondes[25] ,Rondes[26] ,Rondes[27]," | ", Feedback[24], Feedback[25], Feedback[26], Feedback[27]," |",sum(AantalGoed[24:27])," |")    #Ronde 7
    print("Ronde 8 | ",Rondes[28], Rondes[29] ,Rondes[30] ,Rondes[31]," | ", Feedback[28], Feedback[29], Feedback[30], Feedback[31]," |",sum(AantalGoed[28:31])," |")    #Ronde 8
    print("Ronde 9 | ",Rondes[32], Rondes[33] ,Rondes[34] ,Rondes[35]," | ", Feedback[32], Feedback[33], Feedback[34], Feedback[35]," |",sum(AantalGoed[32:35])," |")    #Ronde 9
    print("Ronde 10| ",Rondes[36], Rondes[37] ,Rondes[38] ,Rondes[39]," | ", Feedback[36], Feedback[37], Feedback[38], Feedback[39]," |",sum(AantalGoed[36:39])," |")    #Ronde 10
    print("        ===========================================")

#Ronde 1:
print(Antwoord)
CurrentRound = 1
gok1 = input("Geef een gokje voor de eerste ronde.")
ZetGokInTabel(gok1)
RefreshNumbersAndCheckIfWon()
GeefFeedback(Rondes[0:3])
TekenSpeelgebied()
#Ronde 2:
if Klaar != True:
    CurrentRound = 2
    gok2 = input("Gok voor de tweede keer de getallen.")
    ZetGokInTabel(gok2)
    RefreshNumbersAndCheckIfWon()
    GeefFeedback(Rondes[4:7])
    TekenSpeelgebied()
#Ronde 3:
if Klaar != True:
    CurrentRound = 3
    gok3 = input("Geef een derde gok.")
    ZetGokInTabel(gok3)
    RefreshNumbersAndCheckIfWon()
    GeefFeedback(Rondes[8:11])
    TekenSpeelgebied()
#Ronde 4:

#Ronde 5:

#Ronde 6:

#Ronde 7:

#Ronde 8:

#Ronde 9:

#Ronde 10:
if Klaar:
    print("Je hebt gewonnen! Gefeliciteerd. Pak maar een koekje.")