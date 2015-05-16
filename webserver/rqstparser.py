class HttpRequestParser:
    #Indicate what part of request is still being recieved
    from request import HttpRequest
    from request import HttpGet
    RQ_LINE = 0;
    RQ_HEAD = 1;
    RQ_BODY = 2;
    COMPLETE = 3;
    CRLF = "\r\n"
    def __init__(self):
        self.state = self.TYPE
        self.bodyLength = -1
        self.allRequests = {"GET":HttpGet()}
    def appendContent(self,msg):
        
        if self.state == self.TYPE:
            if CRLF in msg:
                nusplt = msg.split(CRLF,1)
                self.parseRQLine(nusplt[0])
                self.state = self.HEADERS
                msg = nusplt[1]
            else
                self.parseRQLine(msg)
        if self.state == self.HEADERS:
            self.parseHeaders(self,msg)
        if self.state == self.BODY:
        return
    def parseRQLine(self,msg):
        #Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
        print "parseRQLine(): " + msg
        splt = msg.split()
        self.request = self.allRequests[splt[0]]
        self.request.setURI(splt[1])
        self.testVersion(splt[2])
    def parseHeaders(self,msg):
        print "parseHeaders(): " + msg
        splt = msg.split(CRLF + CLRF)
        for hdr in splt:

        
    def getState(self):
        return self.state

    def testVersion(self,v):

        
