import math
I = float(input("Enter number for I (in %)"))
I_o = float(input("Enter number for I_o (in %)"))

n = math.log((I / I_o)) / math.log(0.5)
print("n in the formula I = I_o * (1/2)**n is "+str(n))

# I = Io * (1/2)**n

#12.5 = 100 * (0.5)**n
#0.125 = (1/2)**n
#log(0.125) = n * log(1/2)
#n = log(0.125) / log(1/2)

#dus I = Io * (1/2)**n
#    I / Io = (1/2)**n
#    log(I/Io) = n * log(1/2)
#    n = log(I/Io) / log(1/2)