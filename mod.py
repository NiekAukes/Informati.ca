import random
import pip
import pygame
lenX = int(input("length of x: "))
lenY = int(input("length of y: "))

Radius = int(input("radius of cell: "))
speed = int(input("speed: "))
t = [[]]
stack = []
pygame.init()

hWnd = pygame.display.set_mode((lenX * Radius, lenY * Radius))

pygame.display.set_caption("Maze - by Niek Aukes")
flag_run = True

class Vector2:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __mod__(self, other):
        dX = abs(self.x - other.x)
        dY = abs(self.y - other.y)
        return ((dX**2) + (dY**2))**0.5

class Cell:

    def __init__(self, position):
        self.position = position
        self.wallUp = True
        self.wallLeft = True
        self.wallDown = True
        self.wallRight = True
    def Draw(self):
        pygame.draw.rect(hWnd, (255,255,255), (self.position.x * Radius, self.position.y * Radius, Radius, Radius))
        if self.wallDown:
            pygame.draw.rect(hWnd, (0,0,0), (self.position.x * Radius, self.position.y * Radius, Radius, Radius * 0.1))
            pygame.display.update()
            print("updated wallDown", self.position.y * Radius , self.position.y, Radius)
        if self.wallUp:
            pygame.draw.rect(hWnd, (0,0,0), (self.position.x * Radius, self.position.y * Radius + Radius, Radius, Radius * 0.1))
            pygame.display.update()
            print("updated wallUp")
        if self.wallLeft:
            pygame.draw.rect(hWnd, (0,0,0), (self.position.x * Radius, self.position.y * Radius, Radius * 0.1, Radius))
            pygame.display.update()
            print("updated wallLeft")
        if self.wallRight:
            pygame.draw.rect(hWnd, (0,0,0), (self.position.x * Radius + Radius, self.position.y * Radius, Radius * 0.1, Radius))
            pygame.display.update()
            print("updated wallRight")



hWnd.fill((0,0,255))
for x in range(lenX):
    t.append(list())
    for y in range(lenY):
        t[x].append(None)
t[0][0] = Cell(Vector2(0, 0))
currCell = t[0][0]
currCell.Draw()
pygame.display.update()


def ToNext():
    flag_pass = True
    w_Up = False
    w_Down = False
    w_Left = False
    w_Right = False
    while flag_pass:
        p = round(random.random() * 4)
        if w_Up and w_Down and w_Left and w_Right:
            return 0
        if p == 0:
            newX = currCell.position.x
            newY = currCell.position.y + 1
            if newX >= 0 and newY >= 0 and newX < lenX and newY < lenY and t[newX][newY] == None:
                t[newX][newY] = Cell(Vector2(newX, newY))
                flag_pass = False
                currCell.wallUp = False
                t[newX][newY].wallDown = False
                return t[newX][newY]
            else:
                w_Up = True
                flag_pass = True
        elif p == 1:
            newX = currCell.position.x + 1
            newY = currCell.position.y
            if newX >= 0 and newY >= 0 and newX < lenX and newY < lenY and t[newX][newY] == None:
                t[newX][newY] = Cell(Vector2(newX, newY))
                flag_pass = False
                currCell.wallRight = False
                t[newX][newY].wallLeft = False
                return t[newX][newY]
            else:
                w_Right = True
                flag_pass = True
        elif p == 2:
            newX = currCell.position.x - 1
            newY = currCell.position.y
            if newX >= 0 and newY >= 0 and newX < lenX and newY < lenY and t[newX][newY] == None:
                t[newX][newY] = Cell(Vector2(newX, newY))
                flag_pass = False
                currCell.wallLeft = False
                t[newX][newY].wallRight = False
                return t[newX][newY]
            else:
                w_Left = True
                flag_pass = True
        elif p == 3:
            newX = currCell.position.x
            newY = currCell.position.y - 1
            if newX >= 0 and newY >= 0 and newX < lenX and newY < lenY and t[newX][newY] == None:
                t[newX][newY] = Cell(Vector2(newX, newY))
                flag_pass = False
                currCell.wallDown = False
                t[newX][newY].wallUp = False
                return t[newX][newY]
            else:
                w_Down = True
                flag_pass = True
        




n = 0

while flag_run:
    n+=1
    for event in pygame.event.get():
        if event.type == pygame.QUIT: 
            break
    pygame.time.delay(speed)
    var = ToNext()
    if var != 0:
        stack.append(var)
        currCell.Draw()
        print(var.position.x, var.position.y, currCell.position.x, currCell.position.y)
        currCell = var
        var.Draw()
        pygame.display.update()
    else:
        print(0)
        flag_backtrack = True
        while flag_backtrack:
            if len(stack) == 1:
                flag_run = False
                break
            else:
                stack.pop()
                currCell = stack[-1]
                var = ToNext()
                if var != 0:
                    stack.append(var)
                    currCell.Draw()
                    currCell = var
                    print(var.position.x, var.position.y)
                    var.Draw()
                    flag_backtrack = False
                    pygame.display.update()
                else: print(n)


input("EOF")