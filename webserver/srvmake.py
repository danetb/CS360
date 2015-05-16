import errno
import select
import socket
import sys
import traceback
import struct


class SrvMake:
    def __init__(self):
        self.scktList = list()
#        self.scktInfoList =
  #  def addToList(self,srvsckt):
 #   def 
    def addSock(self, host, port, stype = socket.SOCK_STREAM):
        sfam = socket.AF_INET
        srvsckt = socket.socket(sfam, stype)
        if host == "-l":
            host = "localhost"
        try:
            srvsckt.bind((host,port))
        except socket.error, (value, message):
            print "Unable to bind to that address."
            print "error: " + message
            print "value: " + str(value)
            return
        try:
            srvsckt.listen(5)
        except socket.error, (value, message):
            print "Unable to listen on socket."
            print "error: " + message
            print "value: " + str(value)
            return
        self.scktList.append(srvsckt)
        print "new (AF_INET,SOCK_STREAM) socket now bound & listening on:"
        print "host: " + host
        print "port: " + str(port)
    def addClientSock(self, stype = socket.SOCK_STREAM):
        sfam = socket.AF_INET
        srvsckt = socket.socket(sfam, stype)
        self.scktList.append(srvsckt)
    def connectSock(self, sockindx, host, port):
        try:
            sc = self.scktList[sockindx]
        except IndexError:
            print "socket with that number don't exist"
            return
        try:
            self.scktList[sockindx].connect((host,port))
        except socket.error, (value,message):
            print "Could not connect socket"
            print "error: " + message
    def acceptFromSock(self,sockindx):
        try:
            s = self.scktList[sockindx]
        except IndexError:
            print "that socket hasn't been created."
            return
        s.setblocking(0)
        try:
            newSock, addr = s.accept()
        except socket.error, (value,message):
            print "Could not accept on " + str(s.getsockname())
            print "error: " + message
        s.setblocking(1)
        self.scktList.append(newSock)
    def sendFromSock(self,sockindx,response):
        try:
            s = self.scktList[sockindx]
        except IndexError:
            print "that socket hasn't been created."
            return
        try:
            s.send(response)
        except socket.error, (value,message):
            print "Could not send on " + str(s.getsockname())
            print "error: " + message;
    def recvFromSock(self,sockindx,count):
        try:
            s = self.scktList[sockindx]
        except IndexError:
            print "that socket hasn't been created."
            return
        s.setblocking(0)
        try:
            data = s.recv(count)
            print data
        except socket.error, (value,message):
            print "Could not recv on " + str(s.getsockname())
            print "error: " + message;
        s.setblocking(1)
    def run(self):
        while(True):
            sys.stdout.write("> ");
            line = sys.stdin.readline()
            splt = line.split()
            try:
                if(splt[0] == "smake"):
                    if(len(splt) > 3):
                        stype = self.getSockType(splt[3])
                        self.addSock(splt[1],int(splt[2]),stype)
                    else:
                        self.addSock(splt[1],int(splt[2]))
                elif(splt[0] == "cmake"):
                    if(len(splt) > 1):
                        stype = self.getSockType(splt[1])
                        self.addClientSock(stype)
                    else:
                        self.addClientSock()
                elif(splt[0] == "list"):
                    self.listSockets()
                elif(splt[0] == "connect"):
                    self.connectSock(int(splt[1]),splt[2],int(splt[3]))
                elif(splt[0] == "accept"):
                    self.acceptFromSock(int(splt[1]))
                elif(splt[0] == "send"):
                    self.sendFromSock(int(splt[1]), splt[2])
                elif(splt[0] == "recv"):
                    self.recvFromSock(int(splt[1]),int(splt[2]))
                else:
                    print "invalid command"
            except IndexError:
                print "not enough arguments"
            except ValueError:
                print "bad format"
    def getSockType(self, stype):
        if stype == "stream":
            return socket.SOCK_STREAM
        elif stype == "dgram":
            return socket.SOCK_DGRAM
        elif stype == "raw":
            return socket.SOCK_RAW
        else:
            return socket.SOCK_STREAM #use this as default val
    def listSockets(self):
        for x in range(0, len(self.scktList)):
            sys.stdout.write(str(x) + " -> ")
            print self.scktList[x].getsockname()
#    def getTCPInfo(self, s):
#        fmt = "B"*7+"I"*21
#        x = struct.unpack(fmt, s.getsockopt(socket.IPPROTO_TCP, socket.TCP_INFO, 92))
#        print x

srv = SrvMake()
srv.run()
print "run() returned from"
srvr = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
srvr.bind(("localhost",5432))
srvr.listen(5)
#data = srvr.recv(1024)
clnt = srvr.accept()
data = srvr.recv(1024)
print "Done listening!!!\n"
print data
#srvr.setblocking(0)
