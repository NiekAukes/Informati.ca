word = input("Vul uw woord(en) in HOOFDLETTERS in a.u.b. \n\n\n(alvast bedankt)") 
rondjes = 0
uiteinden = 0 
check = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
circle = [1, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0] 
ends =   [2, 0, 2, 0, 3, 3, 2, 4, 2, 2, 4, 2, 2, 2, 0, 1, 2, 2, 2, 3, 3, 2, 2, 4, 3, 2] 
for i in word: 
    u = check.find(i) 
    if u != -1: 
        rondjes += circle[u] 
        uiteinden += ends[u] 
print(word) 
print("rondjes:", rondjes) 
print("uiteinden:", uiteinden) 

#test case: INFORMATICA, COMPUTER, InDiViDuEeL