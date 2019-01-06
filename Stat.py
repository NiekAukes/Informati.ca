row = input()
row = row.split()
seen = {}
Db = {}
for i in range(len(row)): #counts from every i in range of the length of row
    row[i] = float(row[i]) #turns every value of row into a float
    if row[i] not in seen: #makes a unique list of all numbers with their frequency
        seen[row[i]] = 0
    seen[row[i]] += 1
for num, hz in seen.items(): #makes a unique list of frequencies with their frequency
    if hz not in Db:
        Db[hz] = 0
    Db[hz] += 1
row = sorted(row) #sorts row for median
print("min:", min(row))
print("max:", max(row))
print("avg:", sum(row) / len(row))
print("med:", (row[int((len(row)-1) // 2)] + row[(len(row)) // 2]) / 2) #oof
if 2 == Db[max(Db.keys())]: #if the maximum frequency is greater than 1, modus isn't possible
    print("modus not possible")
else:
    print("mod:", max(seen))

    