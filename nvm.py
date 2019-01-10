x = 0
try:
    N = int(input("Enter a number for N between 0 and 24. This will be the beginning number of *s that the code will produce: "))
    M = int(input("Enter a number for M between 0 and 60. This will be how man rows of *s there will be returned: "))
    V = int(input("Enter a number for V between 0 and 5. This will be how many *s the code will output more than the previous line: "))

    if N < 24 and N > 0 and M <60 and M > 0 and V >0 and V<5:
        for i in range (M):
            print( N * "*" + x * V * "*")
            x += 1
    else:
        print("The numbers were not in the specified range. Cancelled the action.")

except Exception as exc:
    print("Error occured! Try again.", exc)