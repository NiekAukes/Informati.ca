i = 1
Ingredienten = "null"
prijsIngredienten = 0 #In cent
prijsIngredienten = float(prijsIngredienten)
prijsPizza = 0  #In cent

while True:
    diameter = input("Voer de grootte van de pizza in (klein, medium, of groot).")
    if diameter == "klein": #Diameter beslissen aan de hand van de input
        diameter = 20 #Diameter in cm
        break
    elif diameter == "medium":
        diameter = 25
        break
    elif diameter == "groot":
        diameter = 30
        break
    else:
        print("Ongeldige grootte.")
        continue

while True:
    if Ingredienten == "break":
        break
    else:
        Ingredienten = input("Voer een ingrediënt in. Als je uit deze loop wil, moet je break intypen.")
        i += 1
        if "champignons" in Ingredienten:
            prijsIngredienten += 1
        if "ansjovis" in Ingredienten:
            prijsIngredienten += 1.5
        if "ham" in Ingredienten:
            prijsIngredienten += 0.8
        if "salami" in Ingredienten:
            prijsIngredienten += 0.7
        if "ui" in Ingredienten:
            prijsIngredienten += 0.5
        if "baguette" in Ingredienten:
            prijsIngredienten += 10
        continue

prijsPizza = (((diameter / 2)**2) * 3.14) * 4 #Prijs in cent
prijsIngredientenOppervlakte = prijsIngredienten + ((diameter /2) **2) * 3.14

prijsInCent = prijsIngredientenOppervlakte + prijsPizza

print("De prijs van je pizza is ",int(prijsInCent / 100), "euro en",int(prijsInCent % 100), "cent.")