In = input("Vul het aantal rijen in: ")
In = int(In)

def T(_n, _k): 
    if _k == 0 or _k == _n-1:
        return 1 
    else:
        return T(_n-1, _k-1) + T(_n-1, _k) 
for n in range(In): 
    nk = [] 
    for k in range(n): 
        nk.append(T(n, k)) 
    print(nk) 
    #test case: 6, 10, 16