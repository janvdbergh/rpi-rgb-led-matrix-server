# Network client in Python

import socket
from enum import Enum

from crcmod.predefined import mkPredefinedCrcFun


class Command(Enum):
    CLEAR = 0
    SHOW = 1
    COLOR = 2
    PIXEL = 3
    RECTANGLE = 4
    DIGIT = 5
    SMALL_TEXT = 6
    LARGE_TEXT = 7


def to8BitUnsigned(value):
    return value.to_bytes(length=1, byteorder='little', signed=False)


def to16BitUnsigned(value):
    return value.to_bytes(length=2, byteorder='little', signed=False)


def to16BitSigned(value):
    return value.to_bytes(length=2, byteorder='little', signed=True)


def to32BitUnsigned(value):
    return value.to_bytes(length=4, byteorder='little', signed=False)


class NetworkClient:
    def __init__(self, hostName, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((hostName, port))

    def close(self):
        self.sock.shutdown(1)
        self.sock.close()

    def sendPacket(self, data):
        self.sock.sendall(to32BitUnsigned(len(data)))
        self.sock.sendall(data)

        crc = mkPredefinedCrcFun('crc-32')
        self.sock.sendall(to32BitUnsigned(crc(data)))

    def clear(self):
        self.sendPacket(to16BitUnsigned(Command.CLEAR.value))

    def show(self):
        self.sendPacket(to16BitUnsigned(Command.SHOW.value))

    def color(self, r, g, b):
        self.sendPacket(
            to16BitUnsigned(Command.COLOR.value) + to8BitUnsigned(r) + to8BitUnsigned(g) + to8BitUnsigned(b)
        )

    def pixel(self, x, y):
        self.sendPacket(
            to16BitUnsigned(Command.PIXEL.value) + to16BitSigned(x) + to16BitSigned(y)
        )

    def rectangle(self, x, y, width, height):
        self.sendPacket(
            to16BitUnsigned(Command.RECTANGLE.value) + to16BitSigned(x) + to16BitSigned(y) + to16BitSigned(
                width) + to16BitSigned(height)
        )

    def digit(self, position, digit):
        self.sendPacket(
            to16BitUnsigned(Command.DIGIT.value) + to8BitUnsigned(position) + to8BitUnsigned(digit)
        )

    def smallText(self, x, y, text):
        self.sendPacket(
            to16BitUnsigned(Command.SMALL_TEXT.value) + to16BitUnsigned(x) + to16BitUnsigned(y) + to16BitUnsigned(
                len(text)) + bytearray(text, encoding='ascii')
        )

    def largeText(self, x, y, text):
        self.sendPacket(
            to16BitUnsigned(Command.LARGE_TEXT.value) + to16BitUnsigned(x) + to16BitUnsigned(y) + to16BitUnsigned(
                len(text)) + bytearray(text, encoding='ascii')
        )
