pfNum = input()
pfNum = int(pfNum)
d = [] #declares list
for i in range(pfNum-1): #takes every number smaller than the input
    Pfnum = float(pfNum) #declares var equal to the float of pfnum
    if float(pfNum/(i+1)) == Pfnum/(i+1): #if the number turns out equal, then the number is a real divider
        d.append(i+1) #appends the number to the divider list
if sum(d) == pfNum: #if the sum of the dividers is equal to the number, it is a 0erfect number
    print("this is a perfect number")
    print("delers:")
    for i in d:
        print(i)
else:
    print("this is not a perfect number")