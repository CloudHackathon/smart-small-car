import socket


class NetClient(object):

    host = '123.207.82.132'
    port = 8085
    bufferSize = 1024*3

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    print s.connect((host, port))

    def sendTo(self, command):
        print 'send:'.join(command)
        self.s.send(command)

    def receive(self):
        return self.s.recv(self.bufferSize)

if __name__ == '__main__':
    nc = NetClient()
    while 1:
        print nc.receive()