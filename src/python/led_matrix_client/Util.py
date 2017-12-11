def to8BitUnsigned(value):
	return value.to_bytes(length=1, byteorder='little', signed=False)


def to16BitUnsigned(value):
	return value.to_bytes(length=2, byteorder='little', signed=False)


def to16BitSigned(value):
	return value.to_bytes(length=2, byteorder='little', signed=True)


def to32BitUnsigned(value):
	return value.to_bytes(length=4, byteorder='little', signed=False)


def toString(text):
	return to16BitUnsigned(len(text)) + bytearray(text, encoding='ascii')


def from16BitUnsigned(buf, offset):
	bytes = buf[offset:offset + 2]
	return int.from_bytes(bytes, byteorder='little', signed=False)


def from16BitSigned(buf, offset):
	bytes = buf[offset:offset + 2]
	return int.from_bytes(bytes, byteorder='little', signed=True)


def from32BitUnsigned(buf, offset):
	bytes = buf[offset:offset + 4]
	return int.from_bytes(bytes, byteorder='little', signed=False)


def fromString(buf, offset):
	length = from16BitUnsigned(buf, offset)
	bytes = buf[offset + 2:offset + 2 + length]
	return bytes.decode('ascii')
