normques = {
	
}
diffques = {
	
}
easyques ={
	
}
Correct = 0
for ques in normques.keys():
	print(normques[ques])
	In = input()
	if normques[ques] == In:
		Correct += 1
saveCorrect = Correct
Correct = 0
if saveCorrect > 2:
	for ques in diffques.keys():
		print(diffques[ques])
		In = input()
		if diffques[ques] == In:
			Correct += 1
else:
	for ques in easyques.keys():
		print(easyques[ques])
		In = input()
		if easyques[ques] == In:
			Correct += 1
saveCorrect += Correct
if saveCorrect-Correct < 2:
	if Correct == 2:
		pass
	if Correct == 3:
		pass
	else:
		pass
else:
	if Correct > 1:
		pass
	else:
		pass