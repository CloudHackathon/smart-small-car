import socket


class NetClient(object):

    host = '123.207.82.132'
    port = 8080

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    print s.connect((host, port))

    def sendTo(self, command):
        print 'send:'.join(command)
        self.s.send(command)

