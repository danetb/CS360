class PyTry:
    mrGlobal = "french onion"
    def __init__(self):
        self.pymap = {"SOMECLASS":SomeClass1()}
    def proc1(self,a,b):
        res = a.split(b,1)
        print "len(res): " + str(len(res))
        for i in range(0,len(res)):
            print str(i) + " " + res[i]
class SomeClass1:
    def rout1(self,b):
        print "A:" + b + "Z:"
pt = PyTry()
pt.proc1("Someqjeqjeducksqjegoqjequack" ,"qje")
