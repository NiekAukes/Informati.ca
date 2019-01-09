In = input()
In = int(In)

def T(_n, _k): #Herhalingsmethode
    if _k == 0 or _k == _n-1:
        return 1 #Base Case
    else:
        return T(_n-1, _k-1) + T(_n-1, _k) #Recursion Case
for n in range(In): #counts for n in range of input
    nk = [] #declares empty list
    for k in range(n): #counts for k in range of n
        nk.append(T(n, k)) #appends the output o the recursion method
    print(nk) #prints the output of the recursion method
input("press 'Enter' to continue")