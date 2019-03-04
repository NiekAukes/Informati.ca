import random as rm
import os

#variables
d2map = []
seenmap = []
maplength = 11
AreasCleared = 9
bombcount = 20
Continue = 1
letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

flag_char = "*"
space_char = "#"
#functions

def printmap(mp):
    os.system('cls')
    print("\t  0  1  2  3  4  5  6  7  8  9  10")
    print("\t==================================")
    for i in range(len(mp)):
        k = str(letter[i]) + "\t| "
        for j in range(len(mp[i])):
            k += str(mp[i][j]) + "  "
        print(k)

def endGame(Win):
    if (not Win):
        Continue = 0
        printmap(d2map)
        printmap(seenmap)
        input("you Lost, press enter to continue")
    else:
        pass

def flagCell(x, y, callback):
    if not (x <= 0 or x >= maplength or y <= 0 or y >= maplength):
        if (seenmap[x][y] != d2map[x][y]):
            if (seenmap[x][y] != "*"):
                seenmap[x][y] = "*"
            else:
                seenmap[x][y] = "#"
    if (callback != None):
        callback(x, y)

def execCell(x, y, callback):
    if not (x < 0 or x >= maplength or y < 0 or y > maplength):
        if (seenmap[x][y] != d2map[x][y] and seenmap[x][y] != "*"):
            if (d2map[x][y] == "b"):
                endGame(0)
            elif (d2map[x][y] == 0):
                for i in range(3):
                    for j in range(3):
                        if not (x + i - 1 < 0 or x + i - 1 >= maplength or y + j - 1 < 0 or y + j - 1 >= maplength or (j == 1 and i == 1)):
                            execCell(x + i - 1, y + j - 1, callback)
                            seenmap[x][y] = d2map[x][y]
            else:
                seenmap[x][y] = d2map[x][y]
    if (callback != None):
        print("callback here")
        callback(x, y)

def buildMap(Start, c):
    for i in range(maplength):
        d2map.append([])
        seenmap.append([])
        for j in range(maplength):
            d2map[i].append("#")
            seenmap[i].append("#")
    y = Start[0]
        
    x = Start[1]
    if (x < 0 or x > maplength or y < 0 or y > maplength):
        print("invalid character")
        return 0
    untouched = []
    for i in range(3):
        for j in range(3):
            untouched.append([x + i - 1,y + j - 1])
    count = 0
    while (count < bombcount):
        xcalc = rm.randint(0,maplength-1)
        ycalc = rm.randint(0,maplength-1)
        if d2map[xcalc][ycalc] != "b" and [xcalc,ycalc] not in untouched:
            d2map[xcalc][ycalc] = "b"
            count += 1
    #printmap(d2map)
    for i in range(maplength):
        for j in range(maplength):
            count = 0
            if (d2map[i][j] != "b"):
                for a in range(3):
                    for b in range(3):
                        if not (a + i - 1 < 0 or a + i - 1 >= maplength or b + j - 1 < 0 or b + j - 1 >= maplength):
                            if d2map[a + i - 1][b + j - 1] == "b":
                                count += 1
            
                d2map[i][j] = count
    
    for i in untouched:
        execCell(i[0], i[1], callback=c)
        
def Input(Message):
    st = input(Message)
    st = st.split()
    flag = 0
    if (st[0] == "f"):
        flag = 1
    x = letter.find(st[-2])
    y = int(st[-1])
    return [x, y, flag]
                


#executable

if (__name__ == "__main__"):
    start = Input("what is your startingpoint: ")
    print(start)
    buildMap(start, None)
    printmap(d2map)
    printmap(seenmap)

    while Continue:
        In = Input("Next Guess: ")
        print(In)
        if (In[2] == 1):
            flagCell(In[0], In[1])
        else:
            execCell(In[0], In[1], None)
        printmap(seenmap)
