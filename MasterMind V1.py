import random
CurrentRound = 0
Antwoord = []
Rondes = []
Feedback = []
slechtetekens = ",. ?!qwertyuiopasdfghjklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM |{}[]\`~"
for i in range(40):
    Rondes.append("#")
    Feedback.append("-")
#Creëert random nummers:
for i in range(4):
    Antwoord.append(round(random.randint(0,5)))

def Input(gok):
    gok = gok.split()
    for i in range(len(gok)):
        if gok[i] in slechtetekens:
            gok[i] = ""
        elif gok[i] not in slechtetekens:
            gok[i] = int(gok[i])
            Rondes[i] = gok[i]

def GeefFeedback(cijfers):
    #input: lijst met 4 cijfers
    #output: aantal goede cijfers en goed geplaatste cijfers
    output = []

    colorset = set(Antwoord) #maakt een set voor de codes om te checken of de code in het antwoord zit
    for i in range(4):
        OnPlaceCurrentCode = 0 #flag voor correcte plaats
        OnCurrentCode = 0 #flag voor correcte kleur

        #kijkt of er correcte picks zijn
        if (cijfers[i] == Antwoord[i]):
            Feedback[i + ((CurrentRound - 1)* 4)] = 0
        elif (cijfers[i] in colorset):
            Feedback[i + ((CurrentRound - 1)* 4)] = 1
    return Feedback

def TekenSpeelgebied():
    print("=========================")
    print("| ",Rondes[0], Rondes[1] ,Rondes[2] ,Rondes[3]," | ",Feedback[0], Feedback[1], Feedback[2], Feedback[3]," |")           #Ronde 1
    print("| ",Rondes[4], Rondes[5] ,Rondes[6] ,Rondes[7]," | ", Feedback[4], Feedback[5], Feedback[6], Feedback[7]," |")          #Ronde 2
    print("| ",Rondes[8], Rondes[9] ,Rondes[10] ,Rondes[11]," | ", Feedback[8], Feedback[9], Feedback[10], Feedback[11]," |")      #Ronde 3
    print("| ",Rondes[12], Rondes[13] ,Rondes[14] ,Rondes[15]," | ", Feedback[12], Feedback[13], Feedback[14], Feedback[15]," |")  #Ronde 4
    print("| ",Rondes[16], Rondes[17] ,Rondes[18] ,Rondes[19]," | ", Feedback[16], Feedback[17], Feedback[18], Feedback[19]," |")  #Ronde 5
    print("| ",Rondes[20], Rondes[21] ,Rondes[22] ,Rondes[23]," | ", Feedback[20], Feedback[21], Feedback[22], Feedback[23]," |")  #Ronde 6
    print("| ",Rondes[24], Rondes[25] ,Rondes[26] ,Rondes[27]," | ", Feedback[24], Feedback[25], Feedback[26], Feedback[27]," |")  #Ronde 7
    print("| ",Rondes[28], Rondes[29] ,Rondes[30] ,Rondes[31]," | ", Feedback[28], Feedback[29], Feedback[30], Feedback[31]," |")  #Ronde 8
    print("| ",Rondes[32], Rondes[33] ,Rondes[34] ,Rondes[35]," | ", Feedback[32], Feedback[33], Feedback[34], Feedback[35]," |")  #Ronde 9
    print("| ",Rondes[36], Rondes[37] ,Rondes[38] ,Rondes[39]," | ", Feedback[36], Feedback[37], Feedback[38], Feedback[39]," |")  #Ronde 10
    print("=========================")



#Ronde 1:
CurrentRound += 1
gok1 = input("Geef een gokje voor de eerste ronde")
Input(gok1)
GeefFeedback([Rondes[0], Rondes[1] ,Rondes[2] ,Rondes[3]])
TekenSpeelgebied()
#Ronde 2:
CurrentRound += 1
#Ronde 3:

#Ronde 4:

#Ronde 5:

#Ronde 6:

#Ronde 7:

#Ronde 8:

#Ronde 9:

#Ronde 10: