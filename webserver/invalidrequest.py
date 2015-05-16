from response import Response
class InvalidRequestError(Exception):
    def __init__(self, rsp):
        type(rsp) is Response
        self.response = rsp
    def getResponse(self):
        return self.response
