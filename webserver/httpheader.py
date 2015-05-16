from wsgiref.handlers import format_date_time
from datetime import datetime
from time import mktime
class HeaderList:
    def __init__(self):
        self.pairs = {}
    def addHeader(self,a,b):
        type(a) is str
        type(b) is str
        self.pairs[a] = b
    def addDate(self):
        now = datetime.now()
        stamp = mktime(now.timetuple())
        self.addHeader("Date", format_date_time(stamp))
    def addContentHeaders(self, length, contentType):
        type(length) is int
        type(contentType) is str
        self.addHeader("Content-Length",str(length))
        if not contentType == None:
            self.addHeader("Content-Type",contentType)
    def addModified(self, ts):
        self.addHeader("Last-Modified", format_date_time(ts))
    def getHeaderIterator(self):
        return self.pairs.iteritems()
