from enum import Enum

from . import Util


class CommandCode(Enum):
	BRIGHTNESS = 0
	CLEAR = 1
	SHOW = 2
	COLOR = 3
	PIXEL = 4
	RECTANGLE = 5
	DIGIT = 6
	SMALL_TEXT = 7
	LARGE_TEXT = 8
	IMAGE = 9
	ANIMATION = 10
	SLEEP = 11
	COMPOSITE = 12
	DEFINE_IMAGE = 13
	DEFINE_ANIMATION = 14
	SET_LAYER = 15
	CLEAR_LAYER = 16


class Command:
	@staticmethod
	def brightness(brightness):
		return Command(Util.to16BitUnsigned(CommandCode.BRIGHTNESS.value) + Util.to8BitUnsigned(brightness))

	@staticmethod
	def clear():
		return Command(Util.to16BitUnsigned(CommandCode.CLEAR.value))

	@staticmethod
	def show():
		return Command(Util.to16BitUnsigned(CommandCode.SHOW.value))

	@staticmethod
	def color(r, g, b):
		return Command(Util.to16BitUnsigned(CommandCode.COLOR.value) + Util.to8BitUnsigned(r) + Util.to8BitUnsigned(
			g) + Util.to8BitUnsigned(b))

	@staticmethod
	def pixel(x, y):
		return Command(Util.to16BitUnsigned(CommandCode.PIXEL.value) + Util.to16BitSigned(x) + Util.to16BitSigned(y))

	@staticmethod
	def rectangle(x, y, width, height):
		return Command(
			Util.to16BitUnsigned(CommandCode.RECTANGLE.value) + Util.to16BitSigned(x) + Util.to16BitSigned(
				y) + Util.to16BitUnsigned(width) + Util.to16BitUnsigned(height))

	@staticmethod
	def digit(position, digit):
		return Command(
			Util.to16BitUnsigned(CommandCode.DIGIT.value) + Util.to8BitUnsigned(position) + Util.to8BitUnsigned(digit))

	@staticmethod
	def smallText(x, y, text):
		return Command(Util.to16BitUnsigned(CommandCode.SMALL_TEXT.value) + Util.to16BitSigned(x) + Util.to16BitSigned(
			y) + Util.toString(text))

	@staticmethod
	def largeText(x, y, text):
		return Command(Util.to16BitUnsigned(CommandCode.LARGE_TEXT.value) + Util.to16BitSigned(x) + Util.to16BitSigned(
			y) + Util.toString(text))

	@staticmethod
	def image(x, y, name):
		return Command(
			Util.to16BitUnsigned(CommandCode.IMAGE.value) + Util.to16BitSigned(x) + Util.to16BitSigned(
				y) + Util.toString(name))

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
		data = Util.to16BitUnsigned(CommandCode.DEFINE_IMAGE.value) + Util.toString(name) + Util.to16BitUnsigned(
			width) + Util.to16BitUnsigned(height)
		for x in range(0, width):
			for y in range(0, height):
				pixel = image.getpixel((x, y))
				data = data + Util.to8BitUnsigned(pixel[0]) + Util.to8BitUnsigned(pixel[1]) + Util.to8BitUnsigned(
					pixel[2])
		return Command(data)

	@staticmethod
	def defineAnimation(name, command):
		return Command(Util.to16BitUnsigned(CommandCode.DEFINE_ANIMATION.value) + Util.toString(name) + command.data)

	@staticmethod
	def setLayer(layer):
		return Command(Util.to16BitUnsigned(CommandCode.SET_LAYER.value) + Util.to8BitUnsigned(layer))

	@staticmethod
	def clearLayer():
		return Command(Util.to16BitUnsigned(CommandCode.CLEAR_LAYER.value))

	def __init__(self, data):
		self.data = Util.to16BitUnsigned(0xC0) + data
