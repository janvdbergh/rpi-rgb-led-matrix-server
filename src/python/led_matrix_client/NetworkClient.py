# Network client in Python

import socket

from . import Util
from crcmod.predefined import mkPredefinedCrcFun



class NetworkClient:
	def __init__(self, hostName, port):
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect((hostName, port))

	def close(self):
		self.sock.shutdown(1)
		self.sock.close()

	def send(self, command):
		data = command.data
		self.sock.sendall(Util.to32BitUnsigned(len(data)))
		self.sock.sendall(data)

		crc = mkPredefinedCrcFun('crc-32')
		self.sock.sendall(Util.to32BitUnsigned(crc(data)))
