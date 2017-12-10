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
