names = input() #retrieves input
seen = {} #declares dict.
uniq = 0 #declares var
namelist = names.split() #splits names into list
_namelist = [] #declares parallel list
for i in namelist: #retrieves all names seperately
    if i not in seen: #if the name is not yet seen,
        seen[i] = 0  #it creates a value for it
        _namelist.append(i) #and appends it to the parallel list
    seen[i] += 1 #increases the value for frequency
print("name", "frequency")
_namelist = sorted(_namelist) #sorts the list
for i in _namelist: #takes all unique names individually
    print(i, seen[i]) #prints the values
print(uniq) #prints the uniqueness of the namelist