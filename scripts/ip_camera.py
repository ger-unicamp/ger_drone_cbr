#!/usr/bin/env python

import urllib
import cv2
import numpy as np
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

def converteImagem(imagem):
    bridge = CvBridge()
    return bridge.cv2_to_imgmsg(imagem, "bgr8") 

def publicaImagem(imagem):
    publicador = rospy.Publisher('/camera_baixo/raw', Image, queue_size=100)
    publicador.publish(imagem)

if __name__ == "__main__":
    #endereco = "http://192.168.43.1:8080/shot.jpg"
    endereco = "http://192.168.43.55:8080/shot.jpg"
    #Roteador endereco = "http://192.168.1.101:8080/shot.jpg"
    rospy.init_node('ip_camera', anonymous=True)
    rate = rospy.Rate(60)    


    while not rospy.is_shutdown():
        imgResp = urllib.urlopen(endereco)
        imgNp = np.array(bytearray(imgResp.read()), dtype = np.uint8)
        img = cv2.imdecode(imgNp, -1)
        
        imagemROS = converteImagem(img)
        publicaImagem(imagemROS)
        rate.sleep()

