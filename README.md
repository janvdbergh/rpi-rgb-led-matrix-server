# DisplayServer

DisplayServer is a TCP server accepting commands to control a LED matrix display connected to the Raspberry Pi.

## Building

To build DisplayServer, the LED Matrix library has to be installed in `$HOME/libs/rpi-rgb-led-matrix`. 
It can be cloned and built from `https://github.com/hzeller/rpi-rgb-led-matrix`. 

# Server

Build the files. 
Then run either DisplayServerLed or DisplayServerMock. 
It will start listening on port 1246.

## Client

The client library (both C++ and Python) support the following operations:
* clear()
* clor(r, g, b)
* pixel(x, y)
* rectangle(x, y, width, height)
* digit(position, digit) - to show a clock with up to 4 large digits
* smallText(x, y, text)
* largeText(x, y, text)
* defineImage(name, image) - in Python this expects an 8 bit PIL.Image (see Pillow)
* image(x, y, name) 
* show()
