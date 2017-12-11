from enum import Enum

from . import Util


class ResponseCode(Enum):
	NETWORK_ERROR = -99
	CRC_ERROR = -98
	MESSAGE_ERROR = -97
	UNKNOWN_COMMAND = -10
	UNKNOWN_IMAGE = -2
	UNKNOWN_ANIMATION = -1
	OK = 0


class Response:
	def __init__(self, responseData):
		if (Util.from16BitUnsigned(responseData, 0) != 0xE0):
			raise Exception("Invalid packet type")
		self.responseCode = Util.from16BitSigned(responseData, 2)
		self.message = Util.fromString(responseData, 4)

	def isError(self):
		return self.responseCode != ResponseCode.OK.value

	def __str__(self):
		return "Status %d (%s)" % (self.responseCode, self.message)
