abc = input()
abc = abc.split()
T = True #declares passage
for i in range(3):
    abc[i] = int(abc[i]) #makes every value in abc an integer
for i in range(3):
    if abc[i] >= abc[i-1] + abc[i-2]: #if one value is greater than the combined of the others, triangle isn't possible
        print("Triangle not possible", abc[i], abc[i-1] + abc[i-2])
        T = False
        break 
if T:
    print("Triangle Possible")
    for i in range(3):
        if abc[i] == abc[i-1] == abc[i-2]: #searches for special triangle
            print("this is a superspecial triangle")
            break
        if abc[i] == abc[i-1]:
            print("this is a special triangle")
            break

