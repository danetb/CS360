import errno
import select
import socket
import sys
import traceback
import translator
import response
from httpserver import getTimeOut
from time import sleep
from invalidrequest import InvalidRequestError

class Poller:
    """ Polling server """
    def __init__(self, port, debug = False):
        self.host = ""
        self.port = port
        self.debug = debug
        self.open_socket()
        self.clients = {}
        self.size = 1024
        self.requestBuffer = {}
    def open_socket(self):
        """ Setup the socket for incoming clients """
        try:
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
            self.server.bind((self.host,self.port))
            self.server.listen(5)
            self.server.setblocking(0)
        except socket.error, (value,message):
            if self.server:
                self.server.close()
            print "Could not open socket: " + message
            sys.exit(1)

    def run(self):
        """ Use poll() to handle each incoming client."""
        self.poller = select.epoll()
        self.pollmask = select.EPOLLIN | select.EPOLLHUP | select.EPOLLERR
        self.poller.register(self.server,self.pollmask)
        while True:
            # poll sockets
            try:
                fds = self.poller.poll(timeout=1)
            except:
                return
            for (fd,event) in fds:
                # handle errors
                if event & (select.POLLHUP | select.POLLERR):
                    self.handleError(fd)
                    continue
                # handle the server socket
                if fd == self.server.fileno():
                    self.handleServer()
                    continue
                # handle client socket
                self.setTime(fd)
                self.handleClient(fd)
            self.markSweep()
    def markSweep(self):
        deadKeys = []
        for key, value in self.requestBuffer.iteritems():
            tdiff = value.getCurrentTime() - value.getTimeStamp()
            if int(tdiff) > getTimeOut():
                self.poller.unregister(key)
                self.clients[key].close()
                deadKeys.append(key)
        for k in deadKeys:
            #print "deleting k: " + str(k)
            self.poller.unregister(k)
            self.clients[k].close()
            del self.clients[k]
            del self.requestBuffer[k]
    def setTime(self, fd):
        self.requestBuffer[fd].setTimeStamp()
    def handleError(self,fd):
        self.poller.unregister(fd)
        if fd == self.server.fileno():
            # recreate server socket
            self.server.close()
            self.open_socket()
            self.poller.register(self.server,self.pollmask)
        else:
            # close the socket
            self.clients[fd].close()
            del self.clients[fd]
            del self.requestBuffer[fd]

    def handleServer(self):
        # accept as many clients as possible
        while True:
            try:
                (client,address) = self.server.accept()
            except socket.error, (value,message):
                #print "caught error on handleServer()"
                #print message
                # if socket blocks because no clients are available,
                # then return
                if value == errno.EAGAIN or errno.EWOULDBLOCK:
                    return
                print traceback.format_exc()
                sys.exit()
            # set client socket to be non blocking
            client.setblocking(0)
            self.clients[client.fileno()] = client
            self.requestBuffer[client.fileno()] = translator.HttpRequestParser(self.debug)
            self.poller.register(client.fileno(),self.pollmask)
            #print "client created: " + str(client.fileno())
            self.setTime(client.fileno())
    def handleClient(self,fd):
        try:
            data = self.clients[fd].recv(self.size)
        except socket.error, (value,message):
            print "Exception ENCOUNTER IN handleClient() -- " + str(fd) + " " + str(len(data))
            # if no data is available, move on to another client
            if value == errno.EAGAIN or errno.EWOULDBLOCK or errno.EINTR:
                return
            else:
                print "QUITTING FROM HANDLE_CLIENT()"
                print traceback.format_exc()
                print message
                sys.exit()
        if data:
            if self.debug:
                print "Request from client: " + str(fd)
                print data
                print
            try:    
                self.requestBuffer[fd].appendToRequest(data)
            except InvalidRequestError as err:
                self.sendResponse(err.getResponse(), fd)
                self.requestBuffer[fd].clearAll()
                hflag = False
        else:
            self.clients[fd].close()
         #   print "What no data..."
            return
        if self.requestBuffer[fd].stateIsComplete():
            self.enactRequest(fd)
            hflag = False
        #else:
    def enactRequest(self,fd):
        try:
            rqst = self.requestBuffer[fd].getRequest()
            resp = rqst.execute()
            rqst = self.requestBuffer[fd].clearAll()
        except BaseException as ex:
            resp = response.getCodedResponse(500)
            print "undetected internal error: " + ex.message
        self.sendResponse(resp, fd)
    def sendResponse(self, resp, fd):
        rText = resp.toHyperText()
        f = resp.getContentFile()
        sz = resp.getContentSize()
        byts = 0
        if not f == None:
            fullresponse = rText + f.read()
            if self.debug:
                print "Sending response to client: " + str(fd) 
            while byts < sz:
                try:
                    bsnt = self.clients[fd].send(fullresponse)
                    if self.debug:
                        print fullresponse
                    byts += bsnt
                except socket.error as (err, message):
                    if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
                        print "sock err sending response"
                        return
                fllr = fullresponse[byts:]
                fullresponse = fllr
            if self.debug:
                print
        del resp
