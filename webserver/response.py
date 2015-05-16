from httpheader import HeaderList
import os
import time

responseCodes = {200:"OK",400:"Bad Request", 403:"Forbidden",404:"Not Found",500:"Internal Server Error",501:"Not Implemented" }
CLRF = "\r\n"

RESPONSES_PATH = "web/responses/"

def getCodedResponse(code):
        r = Response()
        p = RESPONSES_PATH + str(code) + ".html"
        lastModified = os.stat(p).st_mtime
        size = os.path.getsize(p)
        f = open(p, 'rb')
        r.setContent(f, size, lastModified, "text/html")
        r.setCode(code)
        return r

class Response:
    HTTP_VERSION = "HTTP/1.1"
    def __init__(self, eflag = False):
        self.headerList = HeaderList()
        self.contentFile = None
        if not eflag:
            self.headerList.addDate()
            self.headerList.addHeader("Server","web.py")
    def setCode(self, c):
        self.code = c
    def getMessage(self):
        return responseCodes[self.code]
    def setContent(self,f,size, ts, mediaType = None):
        if not f == None:
            #print time.gmtime(ts)
            #timestamp = time.gmtime(ts)
            self.contentFile = f
            self.contentSize = size
            self.headerList.addContentHeaders(size,mediaType)
            self.headerList.addModified(ts)
    def getContentSize(self):
        return self.contentSize
    def toHyperText(self):
        return self.responseLineText() + self.headersText()#  + self.bodyText()
    def responseLineText(self):
        return self.HTTP_VERSION + " " + str(self.code) + " " + responseCodes[self.code] + CLRF
    def headersText(self):
        result = ""
        for key, value in self.headerList.getHeaderIterator():
            hdr = key + ": " + value + CLRF
            result += hdr
        result += CLRF
        return result
    def getContentFile(self):
        if not self.contentFile == None:
            return self.contentFile
        else:
            return None
    def bodyText(self):
        if not self.contentFile == None:
            return self.contentFile.read()
        else:
            return ""
