#include <utility>

#include <utility>

#include <utility>

#ifndef DISPLAYSERVER_COMMAND_H
#define DISPLAYSERVER_COMMAND_H

#include "../datatype/Point.h"
#include "../datatype/Pixel.h"
#include "../network/NetworkPacket.h"
#include "../datatype/Image.h"

const uint8_t COMMAND_PREFIX = 0xC0;

enum CommandCode {
	UNKNOWN = 0xff,

	RESET = 0,
	SHOW = 1,
	SET_BRIGHTNESS = 10,

	CREATE_LAYER = 50,
	SELECT_LAYER = 51,

	DRAW_PIXEL = 100,
	DRAW_RECTANGLE = 101,

	DEFINE_IMAGE = 150,
	DRAW_IMAGE = 151
};


class Command {
public:
	CommandCode get_commandCode() const { return _commandCode; }

	virtual NetworkPacketPtr packet() const;

protected:
	Command() : _commandCode(UNKNOWN) {};

	explicit Command(CommandCode commandCode) : _commandCode(commandCode) {}

	std::vector<uint8_t> get_command_code_data() const;

private:
	CommandCode _commandCode;
};

typedef std::shared_ptr<Command> CommandPtr;


class ResetCommand : public Command {
public:
	ResetCommand() : Command(RESET) {}
};


class ShowCommand : public Command {
public:
	ShowCommand() : Command(SHOW) {}
};

class SetBrightnessCommand : public Command {
public:
	explicit SetBrightnessCommand(uint8_t brightness) : Command(SET_BRIGHTNESS), _brightness(brightness) {}

	uint8_t get_brightness() const { return _brightness; }

private:
	uint8_t _brightness;
};

class CreateLayerCommand : public Command {
public:
	CreateLayerCommand(uint8_t index, Point offset, uint16_t width, uint16_t height) :
			Command(CREATE_LAYER),
			_index(index), _offset(offset), _width(width), _height(height) {};

	uint8_t get_index() const { return _index; }

	Point get_offset() const { return _offset; }

	uint16_t get_width() const { return _width; }

	uint16_t get_height() const { return _height; }

private:
	uint8_t _index;
	Point _offset;
	uint16_t _width, _height;
};

class SelectLayerCommand : public Command {
public:
	explicit SelectLayerCommand(uint8_t index) : Command(SELECT_LAYER), _index(index) {}

	uint8_t get_index() const { return _index; }

private:
	uint8_t _index;
};

class DrawPixelCommand : public Command {
public:
	DrawPixelCommand(Point location, Pixel value) :
			Command(DRAW_PIXEL), _location(location), _value(value) {}

	Point get_location() const { return _location; }

	Pixel get_value() const { return _value; }

private:
	Point _location;
	Pixel _value;
};

class DrawRectangleCommand : public Command {
public:
	DrawRectangleCommand(Point location, uint16_t width, uint16_t height, Pixel value) :
			Command(DRAW_RECTANGLE), _location(location), _width(width), _height(height), _value(value) {}

	Point get_location() const { return _location; }

	uint16_t get_width() const { return _width; }

	uint16_t get_height() const { return _height; }

	Pixel get_value() const { return _value; }

private:
	Point _location;
	uint16_t _width, _height;
	Pixel _value;
};

class DefineImageCommand : public Command {
public:
	DefineImageCommand(std::string name, ImagePtr image) : _name(std::move(name)), _image(std::move(image)) {}

	const std::string &get_name() const { return _name; }

	ImagePtr get_image() const { return _image; }

private:
	std::string _name;
	ImagePtr _image;
};

class DrawImageCommand: public Command {
public:
	DrawImageCommand(Point location, std::string name): Command(DRAW_IMAGE), _location(location), _name(std::move(name)) {}

	const Point &get_location() const { return _location; }

	const std::string &get_name() const { return _name; }

private:
	Point _location;
	std::string _name;
};

template<class CommandPtr>
class CommandFactory {
public:
	virtual CommandPtr create_reset_command() const = 0;

	virtual CommandPtr create_show_command() const = 0;

	virtual CommandPtr create_set_brightness_command(uint8_t brightness) const = 0;

	virtual CommandPtr create_create_layer_command(uint8_t index, Point offset, uint16_t width,
												   uint16_t height) const = 0;

	virtual CommandPtr create_select_layer_command(uint8_t index) const = 0;

	virtual CommandPtr create_draw_pixel_command(Point location, Pixel value) const = 0;

	virtual CommandPtr create_draw_rectangle_command(Point location, uint16_t width, uint16_t height, Pixel value) const = 0;

	virtual CommandPtr create_define_image_command(const std::string &name, ImagePtr image) const = 0;

	virtual CommandPtr create_draw_image_command(Point location, const std::string &name) const = 0;
};


template<class CommandPtr>
class CommandReader {
public:
	explicit CommandReader(const CommandFactory<CommandPtr> &commandFactory) : _commandFactory(commandFactory) {}

	CommandPtr read_command(NetworkPacketPtr packet);

private:
	CommandFactory<CommandPtr> _commandFactory;
};

class CommandError : public std::exception {
public:
	explicit CommandError(std::string message) : _message(std::move(message)) {}

	const char *what() const noexcept override { return _message.c_str(); }

private:
	std::string _message;
};

#endif //DISPLAYSERVER_COMMAND_H
