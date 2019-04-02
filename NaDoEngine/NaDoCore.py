
SceneObjects = []
class GameObject:
    
    def __init__(self, Src, parent = None):
        self.SubObjects = []
        self.SubComponents = []
        self.PosX = 0
        self.PosY = 0
        if Src != None:
            for i in Src.SubObjects:
                o = GameObject(i)
                self.SubObjects.append(o)
            for i in Src.SubComponents:
                self.AddComponent(type(i))
                SceneObjects.append(self)

    def AddComponent(self, component):
        c = component()
        self.SubComponents.append(c)
        return c

class Component:
    def __init__(self):
        pass

class Camera:
    def __init__(self):
        pass

o = GameObject(None)