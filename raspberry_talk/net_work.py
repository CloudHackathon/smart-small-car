# -*- coding: UTF-8 -*-
# author:fayren

import socket

# TENCENT_CVM_HOST = '123.207.82.132'
TENCENT_CVM_HOST = '123.207.82.132'
TENCENT_CVM_PORT = 8083


class Network(object):
    """网络连接"""
    def __init__(self):
        self.tcpSocket = None
        self.isEnable = False

    def __del__(self):
        self.tcpSocket.close()

    def connect(self, host=TENCENT_CVM_HOST, port=TENCENT_CVM_PORT):
        try:
            self.tcpSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            addr = (host, port)
            try:
                self.tcpSocket.connect(addr)
            except socket.error, msg:
                self.tcpSocket.close()
                print "failed to connect to remote server %s:%d,because of %s" % (host, port, msg)
                return False
            # 没有出现异常
            return True
        except socket.error, msg:
            self.tcpSocket.close()
            print "failed to create socket... %s " % msg
        self.isEnable = False
        return False

    def isEnable(self):
        return self.isEnable

    def recv(self):
        packet = self.tcpSocket.recv(4096)
        return packet