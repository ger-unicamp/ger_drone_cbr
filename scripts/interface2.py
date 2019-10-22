#!/usr/bin/env python

import rospy
from std_msgs.msg import Int8
from std_msgs.msg import String

from Tkinter import *

def publicar(num):
    if num <=4:
        publicador = rospy.Publisher('/iniciarFase', Int8, queue_size=10)
        mensagem = num
        publicador.publish(mensagem)
    elif num == 5:
        publicador = rospy.Publisher('/comandoInterface', String, queue_size=10)
        mensagem = "finalizar"
        publicador.publish(mensagem)
    elif num == 6:
        publicador = rospy.Publisher('/comandoInterface', String, queue_size=10)
        mensagem = "pousar"
        publicador.publish(mensagem)


class Application:
    
    def but1(self):
        publicar(1)

    def but2(self):
        publicar(2)

    def but3(self):
        publicar(3)

    def but4(self):
        publicar(4)

    def but5(self):
        publicar(5)

    def but6(self):
        publicar(6)

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
        self.fase1["command"] = self.but1
        self.fase1.pack(side=LEFT)

        self.fase2 = Button(self.primeiro)
        self.fase2["text"] = "Fase 2"
        self.fase2["font"] = ("Verdana", "15")
        self.fase2["width"] = 10
        self.fase2["command"] = self.but2
        self.fase2.pack(side=RIGHT)

        self.fase3 = Button(self.segundo)
        self.fase3["text"] = "Fase 3"
        self.fase3["font"] = ("Verdana", "15")
        self.fase3["width"] = 10
        self.fase3["command"] = self.but3
        self.fase3.pack(side=LEFT)

        self.fase4 = Button(self.segundo)
        self.fase4["text"] = "Fase 4"
        self.fase4["width"] = 10
        self.fase4["font"] = ("Verdana", "15")
        self.fase4["command"] = self.but4
        self.fase4.pack(side=RIGHT)

        self.titulo2 = Label(self.terceiro, text="Comandos")
        self.titulo2["font"] = ("Verdana", "20", "bold")
        self.titulo2.pack()

        self.comando1 = Button(self.quarto)
        self.comando1["text"] = "Volta Pra Base"
        self.comando1["font"] = ("Verdana", "15", "bold")
        self.comando1["width"] = 15
        self.comando1["fg"] = "red"
        self.comando1["command"] = self.but5
        self.comando1.pack(side=LEFT)

        self.comando2 = Button(self.quarto)
        self.comando2["text"] = "Pousar"
        self.comando2["font"] = ("Verdana", "15", "bold")
        self.comando2["width"] = 15
        self.comando2["fg"] = "red"
        self.comando2["command"] = self.but6
        self.comando2.pack(side=RIGHT)

    


if __name__ == '__main__':
    try:

        rospy.init_node('interface', anonymous=True)
        rate = rospy.Rate(150)
        root = Tk()
        Application(root)
        
        while not rospy.is_shutdown():
            root.update()
            rate = rospy.Rate(150)

    except rospy.ROSInterruptException:
        pass