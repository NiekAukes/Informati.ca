import random as rm

#variables
d2map = []
seenmap = []
maplength = 11
bombcount = 20

letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#functions

def printmap(mp):
    print("\t  A  B  C  D  E  F  G  H  I  J  K")
    print("\t==================================")
    for i in range(len(mp)):
        k = str(i) + "\t| "
        for j in range(len(mp[i])):
            k += str(mp[i][j]) + "  "
        print(k)

def buildMap(Start):
    y = letter.find(Start[0])
        
    x = int(Start[1])
    if (x < 1 or x > maplength - 1 or y < 1 or y > maplength - 1):
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
            print(i, j, d2map[i][j] != "b")
            if (d2map[i][j] != "b"):
                for a in range(3):
                    for b in range(3):
                        if i + a > 0 and i + a < maplength - 1 and j + b > 0 and j + b < maplength - 1:
                            if d2map[a + i - 1][b + j - 1] == "b":
                                count += 1
            
                d2map[i][j] = count
    
    for i in untouched:
        print(i[0],i[1])
        print(seenmap)
        seenmap[i[0]][i[1]] = d2map[i[0]][i[1]]
        
def Input(Message):
    st = input(Message)
    st = st.split()
    return st
                


#executable
for i in range(maplength):
    d2map.append([])
    seenmap.append([])
    for j in range(maplength):
        d2map[i].append("#")
        seenmap[i].append("#")

start = Input("what is your startingpoint: ")
print(start)
buildMap(start)
printmap(d2map)
printmap(seenmap)

Continue = 1
while Continue:
    In = Input("Next Guess: ")
    In
