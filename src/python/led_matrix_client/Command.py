from enum import Enum

from . import Util


class CommandCode(Enum):
	CLEAR = 0
	SHOW = 1
	COLOR = 2
	PIXEL = 3
	RECTANGLE = 4
	DIGIT = 5
	SMALL_TEXT = 6
	LARGE_TEXT = 7
	IMAGE = 8
	ANIMATION = 9
	SLEEP = 10
	COMPOSITE = 11
	DEFINE_IMAGE = 12
	DEFINE_ANIMATION = 13


class Command:
	@staticmethod
	def clear():
		return Command(Util.to16BitUnsigned(CommandCode.CLEAR.value))

	@staticmethod
	def show():
		return Command(Util.to16BitUnsigned(CommandCode.SHOW.value))

	@staticmethod
	def color(r, g, b):
		return Command(Util.to16BitUnsigned(CommandCode.COLOR.value) + Util.to8BitUnsigned(r) + Util.to8BitUnsigned(g) + Util.to8BitUnsigned(b))

	@staticmethod
	def pixel(x, y):
		return Command(Util.to16BitUnsigned(CommandCode.PIXEL.value) + Util.to16BitSigned(x) + Util.to16BitSigned(y))

	@staticmethod
	def rectangle(x, y, width, height):
		return Command(Util.to16BitUnsigned(CommandCode.RECTANGLE.value) + Util.to16BitSigned(x) + Util.to16BitSigned(y) + Util.to16BitUnsigned(width) + Util.to16BitUnsigned(height))

	@staticmethod
	def digit(position, digit):
		return Command(
			Util.to16BitUnsigned(CommandCode.DIGIT.value) + Util.to8BitUnsigned(position) + Util.to8BitUnsigned(digit))

	@staticmethod
	def smallText(x, y, text):
		return Command(Util.to16BitUnsigned(CommandCode.SMALL_TEXT.value) + Util.to16BitSigned(x) + Util.to16BitSigned(y) + Util.toString(text))

	@staticmethod
	def largeText(x, y, text):
		return Command(Util.to16BitUnsigned(CommandCode.LARGE_TEXT.value) + Util.to16BitSigned(x) + Util.to16BitSigned(y) + Util.toString(text))

	@staticmethod
	def image(x, y, name):
		return Command(
			Util.to16BitUnsigned(CommandCode.IMAGE.value) + Util.to16BitSigned(x) + Util.to16BitSigned(y) + Util.toString(name))

	@staticmethod
	def animation(name):
		return Command(
			Util.to16BitUnsigned(CommandCode.ANIMATION.value) + Util.toString(name))

	@staticmethod
	def sleep(duration):
		return Command(Util.to16BitUnsigned(CommandCode.SLEEP.value) + Util.to16BitSigned(duration))

	@staticmethod
	def composite(commands):
		data = Util.to16BitUnsigned(CommandCode.COMPOSITE.value) + Util.to16BitUnsigned(len(commands))
		for command in commands:
			data = data + command.data
		return Command(data)

	@staticmethod
	def defineImage(name, image):
		width = image.size[0]
		height = image.size[1]
		data = Util.to16BitUnsigned(CommandCode.DEFINE_IMAGE.value) + Util.toString(name) + Util.to16BitUnsigned(width) + Util.to16BitUnsigned(height)
		for x in range(0, width):
			for y in range(0, height):
				pixel = image.getpixel((x, y))
				data = data + Util.to8BitUnsigned(pixel[0]) + Util.to8BitUnsigned(pixel[1]) + Util.to8BitUnsigned(pixel[2])
		return Command(data)

	@staticmethod
	def defineAnimation(name, command):
		return Command(Util.to16BitUnsigned(CommandCode.DEFINE_ANIMATION.value) + Util.toString(name) + command.data)

	def __init__(self, data):
		self.data = data
