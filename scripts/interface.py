from tkinter import *
import interface2

class Application:
    def __init__(self, master=None):
        self.fontePadrao = ("Verdana", "20", "bold")

        self.primeiro = Frame(master)
        self.primeiro["pady"] = 20
        self.primeiro.pack()
        self.segundo = Frame(master)
        self.segundo["padx"] = 200
        self.segundo.pack()
        self.terceiro = Frame(master)
        self.terceiro["padx"] = 20
        self.terceiro.pack()
        self.quarto = Frame(master)
        self.quarto["pady"] = 20
        self.quarto.pack()
        self.titulo = Label(self.primeiro, text="Fases")
        self.titulo["font"] = self.fontePadrao
        self.titulo.pack()

        self.fase1 = Button(self.primeiro)
        self.fase1["text"] = "Fase 1"
        self.fase1["font"] = ("Verdana", "15")
        self.fase1["width"] = 10
        self.fase1["command"] = interface2.publicar(1)
        self.fase1.pack(side=LEFT)

        self.fase2 = Button(self.primeiro)
        self.fase2["text"] = "Fase 2"
        self.fase2["font"] = ("Verdana", "15")
        self.fase2["width"] = 10
        self.fase2["command"] = interface2.publicar(2)
        self.fase2.pack(side=RIGHT)

        self.fase3 = Button(self.segundo)
        self.fase3["text"] = "Fase 3"
        self.fase3["font"] = ("Verdana", "15")
        self.fase3["width"] = 10
        self.fase3["command"] = interface2.publicar(3)
        self.fase3.pack(side=LEFT)

        self.fase4 = Button(self.segundo)
        self.fase4["text"] = "Fase 4"
        self.fase4["font"] = ("Verdana", "15")
        self.fase4["width"] = 10
        self.fase4["command"] = interface2.publicar(4)
        self.fase4.pack(side=RIGHT)

        self.titulo2 = Label(self.terceiro, text="Comandos")
        self.titulo2["font"] = ("Verdana", "20", "bold")
        self.titulo2.pack()

        self.comando1 = Button(self.quarto)
        self.comando1["text"] = "Volta Pra Base"
        self.comando1["font"] = ("Verdana", "15", "bold")
        self.comando1["width"] = 15
        self.comando1["fg"] = "red"
        self.comando1["command"] = interface2.publicar(5)
        self.comando1.pack(side=LEFT)

        self.comando2 = Button(self.quarto)
        self.comando2["text"] = "Pousar"
        self.comando2["font"] = ("Verdana", "15", "bold")
        self.comando2["width"] = 15
        self.comando2["fg"] = "red"
        self.comando2["command"] = interface2.publicar(6)
        self.comando2.pack(side=RIGHT)

root = Tk()
Application(root)
root.mainloop()