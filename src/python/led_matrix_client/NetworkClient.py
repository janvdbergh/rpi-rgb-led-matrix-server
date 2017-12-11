# Network client in Python

import socket

from crcmod.predefined import mkPredefinedCrcFun

from . import Response
from . import Util


def sendData(sock, data):
	sock.sendall(data)


def receiveData(sock, size):
	buf = bytearray(size)
	sock.recv_into(buf, size, socket.MSG_WAITALL)
	return buf


def calculateCrc(data):
	return mkPredefinedCrcFun('crc-32')(data)


def sendPacket(socket, data):
	sendData(socket, Util.to32BitUnsigned(len(data)))
	sendData(socket, data)
	sendData(socket, Util.to32BitUnsigned(calculateCrc(data)))


def receivePacket(socket):
	buf = receiveData(socket, 4)
	size = Util.from32BitUnsigned(buf, 0)

	data = receiveData(socket, size)

	buf = receiveData(socket, 4)
	crc = Util.from32BitUnsigned(buf, 0)
	if (calculateCrc(data) != crc):
		raise Exception("Invalid CRC")

	return data


class NetworkClient:
	def __init__(self, hostName, port):
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect((hostName, port))

	def close(self):
		self.sock.shutdown(1)
		self.sock.close()

	def sendCommand(self, command):
		sendPacket(self.sock, command.data)

		responseData = receivePacket(self.sock)
		response = Response.Response(responseData)

		if response.isError():
			raise Exception(response)
