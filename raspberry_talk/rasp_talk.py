# -*- coding: UTF-8 -*-
# author: fayren

import os
from net_work import *
import time

notice = {
    1: "watch out bro! I'm coming through!",
    2: "hello,can you help me? the lift!"
}


class Raspberry(object):
    """树莓派操作"""
    def __init__(self):
        self.__network = Network()

    def speak(self, packet):
        if packet.isdigit():
            idx = int(packet)
            if idx >=0 and idx < len(notice):
                msg = notice[int(packet)]
            else:
                msg = 'oops...I have no idea what to say'
        else:
            msg = packet
        cmd = "espeak -ven+f3 -k5 -s150 '%s'" % msg
        os.system(cmd)

    def run(self):
        try:
            if self.__network.connect():
                while True:
                    # 一直等待接受消息
                    packet = self.__network.recv()
                    print "xiaofei recv:" + packet
                    if packet is None:
                        print "connection broken..."
                        break
                    self.speak(packet=packet)
            else:
                print "connection failed..."
        except socket.error, msg:
            print "failed to connect to the server, msg:%s" % msg

if __name__ == '__main__':
    rasp = Raspberry()
    rasp.run()

