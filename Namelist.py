names = input("Vul de namen in: ") 
seen = {} 
uniq = 0 
namelist = names.split() 
_namelist = [] 
for i in namelist: 
    if i not in seen: 
        seen[i] = 0  
        _namelist.append(i) 
        uniq += 1
    seen[i] += 1 
_namelist = sorted(_namelist) 
for i in _namelist: 
    print(i,'-',seen[i]) 
print("Er zijn", uniq,"verschillende namen.") 