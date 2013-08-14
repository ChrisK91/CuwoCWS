from cuwo.script import ServerScript, ConnectionScript
import ctypes

libCuwoCWS = ctypes.cdll.LoadLibrary("CuwoCWS.dll")
CuwoCWSObj = None

class CuwoCWSConnection(ConnectionScript):
    def on_join(self, event):
    	libCuwoCWS.CuwoComm_playerJoined(CuwoCWSObj, ctypes.c_char_p(self.connection.name), ctypes.c_char_p(self.connection.address.host))
    	#print dir(self.connection);

    def on_unload(self):
        libCuwoCWS.CuwoComm_playerLeft(CuwoCWSObj, ctypes.c_char_p(self.connection.address.host))

class CuwoCWS(ServerScript):
    connection_class = CuwoCWSConnection

    def on_load(self):
        global CuwoCWSObj
    	CuwoCWSObj = libCuwoCWS.CuwoComm_new()
    	libCuwoCWS.CuwoComm_initialize(CuwoCWSObj)
    
    def on_unload(self):
    	libCuwoCWS.CuwoComm_destruct(CuwoCWSObj)
		

def get_class():
    return CuwoCWS