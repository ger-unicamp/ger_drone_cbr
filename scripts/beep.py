import sys


import rospy
from sound_play.msg import SoundRequest
from sound_play.libsoundplay import SoundClient

if __name__ == "__main__":
    rospy.init_node('beep_node', anonymous = True)

    soundhandle = SoundClient()
    rospy.sleep(1)

    soundhandle.play(2, 1.0)

    rospy.sleep(1)  