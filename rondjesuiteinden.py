word = input() ##asks for input
rondjes = 0
uiteinden = 0 ##declares var
check = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ##alfabet string
circle = [1, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0] ##information for circles
ends =   [2, 0, 2, 0, 3, 3, 2, 4, 2, 2, 4, 2, 2, 2, 0, 1, 2, 2, 2, 3, 3, 2, 2, 4, 3, 2] ##information for ends
for i in word: ##Takes chars of input seperately
    u = check.find(i) ##tries to find the char
    if u != -1: ##if the char is not found in the alfabet, the program will skip it
        rondjes += circle[u] ##adds circles to output
        uiteinden += ends[u] ##adds ends to output
print(word) ##outputs the origional word
print("rondjes:", rondjes) ##outputs the circles
print("uiteinden:", uiteinden) ##outputs the ends
input("press 'Enter' to continue")
##end of code