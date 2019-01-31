even = []
oneven = []
x = 0

lijstje = input("lfkjsdklfjasldjflka")
lijstje = lijstje.split()
for i in lijstje:
    lijstje[x] = int(lijstje[x])
    if lijstje[x] % 2 == 0:
        even.append(lijstje[x])
        x += 1
    else:
        oneven.append(lijstje[x])
        x += 1

print("Er zijn ",len(even),"even getallen en",len(oneven),"oneven getallen in.\nDe getallen zijn:\n",even,"\n",oneven)