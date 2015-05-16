from httpserver import ServerModel
from httpserver import ServerModelConfig
from response import Response
from response import getCodedResponse
from httpheader import HeaderList
import os
accss = ServerModel()

class HttpRequest:
    def __init__(self):
        self.headerList = HeaderList()
    def execute(self):
        raise NotImplementedException()
    def setURI(self, uri):
        self.uri = uri
    def addHeader(self,a,b):
        self.headerList.addHeader(a,b)
class Get(HttpRequest):
    def execute(self):
        response = Response()
        if "/" in self.uri:
            splt = self.uri.split("/", 1)
            host = splt[0]
            rpath = splt[1]
        else:
            host = "default"
            rpath = ""
        #Set content body of header
        try:
            (f, size, ts, mediaType) = accss.getResource(host,rpath)
        except IOError as e:
            if e.errno == 2:
                r2 = getCodedResponse(404)
                print "error: file not found"
            elif e.errno == 13:
                r2 = getCodedResponse(403)
                print "error: insufficient priveleges"
            elif e.errno == 21:
                r2 = getCodedResponse(403)
                print "error: resource is directory"
            return r2
        response.setContent(f, size, ts, mediaType)
        response.setCode(200)
        return response
#class Head(HttpRequest):

#class Put(HttpRequest):

#class Post(HttpRequest):

#class Options(HttpRequest):

#class Delete(HttpRequest):

#class Connect(HttpRequest):

#class Trace(HttpRequest):
