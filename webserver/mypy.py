f = open("web/static/files/largefile.txt","r")
b = 0
while True:
    q = f.readline()
    if not q:
        break
    b += len(q)
    if "gypsy" in q:
        break
print b

