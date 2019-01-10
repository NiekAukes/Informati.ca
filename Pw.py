normques = { #maakt een dictionary van de normale vragen (normal questions wordt normques)

}
diffques = { #maakt een dictionary voor de moeilijke vragen (difficult questions wordt diffques)

}
easyques ={ #maakt een dictionary voor de makkelijke vragen (easy questions wordt easyques)

}
Correct = 0 #Declareert een variabele Correct met een var 0
for ques in normques.keys(): #Voor alle vragen in normale vragen (dictionaries werken met keys en values, door .keys te specificeren selecteer je aleen de 'keys', dus de vragen)
	print(normques[ques]) #print een vraag
	In = input()		#vraagt om een input
	if normques[ques] == In: #als het antwoord gelijk is aan de vraag, dan
		Correct += 1	 #maakt de code de variabele Correct Ã©Ã©n groter, dus er is dan Ã©Ã©n goed antwoord meer.
saveCorrect = Correct #Maakt een variabele saveCorrect aan met hetzelfde var als Correct (saveCorrect is de variabele voor alle GOEDE opgeslagen antwoorden voor de NORMALE vragen.)
Correct = 0
if saveCorrect > 2:	#als de opgeslagen goede antwoorden meer dan twee zijn (dus als je meer dan twee goede antwoorden hebt)
	for ques in diffques.keys():	#vraagt de code de lastige vragen
		print(diffques[ques])
		In = input()		#..en vraagt weer om een input
		if diffques[ques] == In:
			Correct += 1 	#...en voor elk goed antwoord komt er bij Correct weer 1 bij.
else:	#als er MINDER dan twee antwoorden goed waren....
	for ques in easyques.keys():	#stelt de code makkelijke vragen i.p.v. moeilijke.
		print(easyques[ques])
		In = input()		#...en vraagt weer om een input, etc.
		if easyques[ques] == In:
			Correct += 1
saveCorrect += Correct			#Bij de opgeslagen goede antwoorden van de normale vragen worden de goede antwoorden van de vorige vragen opgeteld.

if saveCorrect-Correct < 2:	#Als de totale goede antwoorden min de laatste
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