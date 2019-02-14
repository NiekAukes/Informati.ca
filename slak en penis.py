def telStrings(kString, gString):
    count = 0
    for i in range(len(gString) - len(kString) + 1):
        if kString in gString[i:i+len(kString)]:
            count += 1
            print("Item found")
    return "There were",count," times where the program found ",kString," in ",gString