try:
    N = int(input("Kies een cijfer voor N tussen 0 en 24. Dit zijn het aantal sterretjes die bij de eerste rij te zien zijn."))
    M = int(input("Kies een cijfer voor M tussen 0 en 60. Dit zijn het aantal rijen die te zien zullen zijn. "))
    V = int(input("Kies een cijfer voor V tussen 0 en 5. Dit is het aantal sterretjes wat per rij groter wordt."))

    if N < 24 and N > 0 and M <60 and M > 0 and V >0 and V<5:
        for i in range (M):
            print( N * "*" + i * V * "*")
    else:
        print("The numbers were not in the specified range. Cancelled the action.")

except Exception as exc:
    print("Error occured! Try again.", exc)