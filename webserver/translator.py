import request as Rq
from invalidrequest import InvalidRequestError
#from request import Request
from request import Get
from response import Response
from response import getCodedResponse
import time

CRLF = "\r\n"
allRequests = {"GET":Rq.Get()}
class HttpRequestParser:
    #Indicate what part of request is still being recieved
    LINE_METHOD = 0
    LINE_URI = 1
    LINE_VERSION = 2
    LINE_JUNK = 3
    HEADER = 4;
    COMPLETE = 5;
    def __init__(self, d = False):
        self.request = None
        self.clearAll()
        self.debug = d
    def clearAll(self):
        self.state = self.LINE_METHOD
        self.contentLength = -1
        self.content = ""
        self.msgBffr = ""
    def stateIsLine(self):
        if self.state == self.LINE_METHOD or self.state == self.LINE_URI or self.state == self.LINE_VERSION:
            return True
        else:
            return False
    def stateIsLineMethod(self):
        if self.state == self.LINE_METHOD:
            return True
        else:
            return False
    def stateIsLineURI(self):
        if self.state == self.LINE_URI:
            return True
        else:
            return False
    def stateIsLineVersion(self):
        if self.state == self.LINE_VERSION:
            return True
        else:
            return False
    def stateIsLineJunk(self):
        if self.state == self.LINE_JUNK:
            return True
        else:
            return False
    def stateIsHeader(self):
        if self.state == self.HEADER:
            return True
        else:
            return False
    def stateIsComplete(self):
        if self.state == self.COMPLETE:
            return True
        else:
            return False
    def stateMove(self):
        self.state += 1
    def getCurrentTime(self):
        return time.time()
    def setTimeStamp(self):
        self.time = time.time()
    def getTimeStamp(self):
        return self.time
    def appendToRequest(self,msg):
        self.msgBffr += msg
        if self.stateIsLineMethod():
            self.parseRQLineMethod()
        if self.stateIsLineURI():
            self.parseRQLineURI()
        if self.stateIsLineVersion():
            self.parseRQLineVersion()
        if self.stateIsLineJunk():
            self.parseRQLineJunk()
        if self.stateIsHeader():
            if self.msgBffr.startswith(CRLF):
                self.pollFromBuffer(CRLF)
                self.stateMove()
                return
            transit_flag = False
            if self.bufferHasSequence(CRLF + CRLF):
                transit_flag = True
            while self.bufferHasSequence(CRLF):
                self.parseHeader()
            if transit_flag:
                self.stateMove()
    #PRE-CONDITION: buffr must contain seq
    def pollFromBuffer(self, seq):
        splt = self.msgBffr.split(seq, 1)
        if len(splt) == 2:
            self.msgBffr = splt[1]
        else:
            self.msgBffr = ""
        return splt[0]
    def clearBuffer(self):
        s = self.msgBffr
        self.msgBffr = ""
        return s
    def bufferHasSequence(self, seq):
        if seq in self.msgBffr:
            return True
        else:
            return False
    def getNextTokenInLine(self):
        method = self.pollFromBuffer(" ")
        if CRLF in method:
            if self.debug:
                print "Reached CRLF prematurely. Request line malformed."
            r = getCodedResponse(400)
            raise InvalidRequestError(r)
        return method
    def parseRQLineMethod(self):
        if not self.bufferHasSequence(" "):
            return
        method = self.getNextTokenInLine()
        try:
            self.request = allRequests[method]
        except KeyError:
            if self.debug:
                print "Method '" + method + "' not implemented"
            r = getCodedResponse(501)
            raise InvalidRequestError(r)
        self.stateMove()
    def parseRQLineURI(self):
        if not self.bufferHasSequence(" "):
            return
        uri = self.getNextTokenInLine()
        self.request.setURI(uri)
        self.stateMove()
    def parseRQLineVersion(self):
        if not self.bufferHasSequence(CRLF):
            if self.bufferHasSequence(" "):
                version = self.getNextTokenInLine()
                self.stateMove()
        else:
            lineEnd = self.pollFromBuffer(CRLF)
            if " " in lineEnd:
                version = lineEnd.split(" ", 1)[0]
            else:
                version = lineEnd
            #Do something with version. But what exactly?
            self.stateMove()
            self.stateMove() #CRLF is present. Need to advance past junk parsing.
    def parseRQLineJunk(self):
        if self.bufferHasSequence(CLRF):
            self.pollFromBuffer(CLRF)
            self.stateMove()
        else:
            self.clearBuffer()
    #PRE-CONDITION: buffr has sequence CRLF
    def parseHeader(self):
        headr = self.pollFromBuffer(CRLF)
        hsplt = headr.split(": ",1)
        if len(hsplt) == 2:
            self.request.addHeader(hsplt[0],hsplt[1])
            #For now, ignore badly formatted headers
    def getRequest(self):
        return self.request
