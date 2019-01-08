import math
prijsIngredienten = 0 #In cent
prijsIngredienten = float(prijsIngredienten)
prijsPizza = 0  #In cent

diameter = input("Voer de grootte van de pizza in (klein, medium, of groot).")
Ingredienten = input("Wat voor ingredienten wil je erop?")

if diameter == "klein": #Diameter beslissen aan de hand van de input
    diameter = 20 #Diameter in cm
elif diameter == "medium":
    diameter = 25
elif diameter == "groot":
    diameter = 30
else:
    print("Ongeldige grootte.")

Ingredienten = Ingredienten.split() #Ingrediënten splitten in een list
lijstIngredienten = ["champignons", "ansjovis", "ham", "salami", "ui", "paprika", "mosselen", "baguette"]
for lijstIngredienten in Ingredienten:
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

prijsPizza = (((diameter / 2)**2) * math.pi) * 4 #Prijs in cent
prijsIngredientenOppervlakte = prijsIngredienten + ((diameter /2) **2) * math.pi

prijsInCent = prijsIngredientenOppervlakte + prijsPizza

print("De prijs van je pizza is ",int(prijsInCent / 100), "euro en ",int(prijsInCent % 100), "cent.")