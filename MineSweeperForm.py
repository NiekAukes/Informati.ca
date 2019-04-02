import MineSweeper as ms
import tkinter as tk

CellLength = 20
ScreenLength = 500
ms.maplength = ScreenLength // CellLength
ms.bombcount = 100
CellList = []


root = tk.Tk()
root.geometry("500x500")

First = 1
canvas = tk.Canvas(root, width=500, height=500)
canvas.pack()


one = tk.PhotoImage(file="Src/One.gif").subsample(96, 96)
two = tk.PhotoImage(file="Src/Two.gif").subsample(64, 64)
three = tk.PhotoImage(file="Src/Three.gif").subsample(16, 16)
four = tk.PhotoImage(file="Src/Four.gif").subsample(64, 64)
flag = tk.PhotoImage(file="Src/golf_flag.gif").subsample(16, 16)
imageSelector = {
    0: None,
    1: one,
    2: two,
    3: three,
    4: four,
    "*": flag
}



def Draw(x, y):
    tempimg = {1:"blue",2:"green",3:"red",4:"purple"}
    j = ms.seenmap[x][y]
    if (j != "#"):
        print(imageSelector[j], x, y)
        if (j == 0):
            canvas.create_rectangle(y * 20, x * 20, y * 20 + 20, x * 20 + 20, fill='gray20')
        else:
            if (j != "*"):
                canvas.create_rectangle(y * 20, x * 20, y * 20 + 20, x * 20 + 20, fill='gray20')
            canvas.create_image([y * 20 + 10, x * 20 + 10], image=imageSelector[j])
            #canvas.create_rectangle(y * 20, x * 20, y * 20 + 20, x * 20 + 20, fill=tempimg[j])
    else:
        canvas.create_rectangle(y * 20, x * 20, y * 20 + 20, x * 20 + 20, fill='grey')
                        

def LMB(event):
    global First
    if (First):
        print([round(event.x / CellLength + 0.5, None), round(event.y / CellLength + 0.5, None)])
        ms.buildMap([round(event.x / CellLength - 0.5, None), round(event.y / CellLength - 0.5, None)], Draw)
        First = False
    else:
        ms.execCell(round(event.y / CellLength - 0.5, None), round(event.x / CellLength - 0.5, None), Draw)
        k = ms.d2map[round(event.y / CellLength - 0.5, None)][round(event.x / CellLength - 0.5, None)]
        print(k, [round(event.x / CellLength - 0.5, None), round(event.y / CellLength - 0.5, None)])

def RMB(event):
    if (not First):
        print("RMB")
        ms.flagCell(round(event.y / CellLength - 0.5, None), round(event.x / CellLength - 0.5, None), Draw)

canvas.bind("<Button-1>", LMB)
canvas.bind("<Button-3>", RMB)


for i in range(0, ScreenLength, CellLength):
    CellList.append([])
    for j in range(0, ScreenLength, CellLength):
        a = canvas.create_rectangle(i, j, i + CellLength, j + CellLength, fill='grey')
        CellList[int(i//CellLength)].append(a)


root.mainloop()