import tkinter
import keyboard
from WinWorld import World
from WinWorld import Object

win = tkinter.Tk()
win.geometry("600x400")
c = tkinter.Canvas(win, width=600, height=400, bg="blue")
c.pack()
l = c.create_line(0,0,100,20,width=5)
space = World()
space.CreateObject(Object)
while 1:




    #Update Method
    if (keyboard.is_pressed('escape')):
        print("update test")
        win.quit()
        break
    c.update()
    win.update()
