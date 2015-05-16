import os
TIMEOUT = None
ConfigVar = None
def getTimeOut():
    return TIMEOUT
class ServerModelConfig:
    hostDict = {}
    mediaDict = {}
    def __init__(self):
        f = open('web.conf','r')
        self.readContents(f)
    def readContents(self,f):
        for line in f:
            splt = line.split()
            if len(splt) > 0:
                if splt[0] == "host":
                    self.hostDict[splt[1]] = splt[2]
                elif splt[0] == "media":
                    self.mediaDict[splt[1]] = splt[2]
                elif splt[0] == "parameter":
                    self.readParam(splt[1],splt[2])
    def readParam(self,a,b):
        if a == "timeout":
            self.timeout = int(b)
            TIMEOUT = self.timeout
class ServerModel:
    def __init__(self):
        self.config = ConfigVar
    def getResource(self, host, relativepath):
        if relativepath == "":
            relativepath = "index.html"
        try:
            pth = self.config.hostDict[host] + "/" + relativepath
        except KeyError:
            #no match for given host
            try:
                pth = self.config.hostDict["default"] + "/" + relativepath
            except KeyError:
                print "default host not set. exiting."
                sys.exit()
        #print "fetching resource w/ path: " + pth
        f = open(pth, 'rb')
        size = os.path.getsize(pth)
        lastModified = os.stat(pth).st_mtime
        msplt = pth.split(".")
        ext = msplt[-1]
        mediaType = None
        try:
            mediaType = self.config.mediaDict[ext]
        except KeyError:
            mediaType = "text/plain" #default setting if no match found
        return (f, size, lastModified, mediaType)
ConfigVar = ServerModelConfig()
TIMEOUT = ConfigVar.timeout
