#ifndef DISPLAYSERVER_COMMAND_H
#define DISPLAYSERVER_COMMAND_H

#include <common/Point.h>
#include <common/Pixel.h>
#include <common/Image.h>
#include <common/NetworkPacket.h>
#include "NetworkPacketParser.h"

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
	CommandCode get_command_code() const { return _command_code; }

	virtual NetworkPacketPtr packet() const;

protected:
	Command() : _command_code(UNKNOWN) {};

	explicit Command(CommandCode commandCode) : _command_code(commandCode) {}

private:
	CommandCode _command_code;
};

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

	NetworkPacketPtr packet() const override;

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

	NetworkPacketPtr packet() const override;

private:
	uint8_t _index;
	Point _offset;
	uint16_t _width, _height;
};

class SelectLayerCommand : public Command {
public:
	explicit SelectLayerCommand(uint8_t index) : Command(SELECT_LAYER), _index(index) {}

	uint8_t get_index() const { return _index; }

	NetworkPacketPtr packet() const override;

private:
	uint8_t _index;
};

class DrawPixelCommand : public Command {
public:
	DrawPixelCommand(Point location, Pixel value) :
			Command(DRAW_PIXEL), _location(location), _value(value) {}

	Point get_location() const { return _location; }

	Pixel get_value() const { return _value; }

	NetworkPacketPtr packet() const override;

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

	NetworkPacketPtr packet() const override;

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

	NetworkPacketPtr packet() const override;
private:
	std::string _name;
	ImagePtr _image;
};

class DrawImageCommand : public Command {
public:
	DrawImageCommand(Point location, std::string name) : Command(DRAW_IMAGE), _location(location),
														 _name(std::move(name)) {}

	const Point &get_location() const { return _location; }

	const std::string &get_name() const { return _name; }

	NetworkPacketPtr packet() const override;

private:
	Point _location;
	std::string _name;
};

template<class CommandType>
class CommandFactory {
public:
	virtual std::shared_ptr<const CommandType> create_reset_command() const = 0;

	virtual std::shared_ptr<const CommandType> create_show_command() const = 0;

	virtual std::shared_ptr<const CommandType> create_set_brightness_command(uint8_t brightness) const = 0;

	virtual std::shared_ptr<const CommandType> create_create_layer_command(uint8_t index, Point offset, uint16_t width,
												   uint16_t height) const = 0;

	virtual std::shared_ptr<const CommandType> create_select_layer_command(uint8_t index) const = 0;

	virtual std::shared_ptr<const CommandType> create_draw_pixel_command(Point location, Pixel value) const = 0;

	virtual std::shared_ptr<const CommandType> create_draw_rectangle_command(Point location, uint16_t width, uint16_t height, Pixel value) const = 0;

	virtual std::shared_ptr<const CommandType> create_define_image_command(const std::string &name, ImagePtr image) const = 0;

	virtual std::shared_ptr<const CommandType> create_draw_image_command(Point location, const std::string &name) const = 0;
};

template<class CommandType>
class CommandReader {
public:
	explicit CommandReader(const std::shared_ptr<CommandFactory<CommandType>> commandFactory) : _commandFactory(commandFactory) {}

	std::shared_ptr<const CommandType> read_command(NetworkPacketPtr packet);

private:
	std::shared_ptr<CommandFactory<CommandType>> _commandFactory;
};

class CommandError : public std::exception {
public:
	explicit CommandError(std::string message) : _message(std::move(message)) {}

	const char *what() const noexcept override { return _message.c_str(); }

private:
	std::string _message;
};

template<class CommandType>
std::shared_ptr<const CommandType> CommandReader<CommandType>::read_command(NetworkPacketPtr packet) {
	NetworkPacketParser parser(std::move(packet));

	uint8_t command_code = parser.read_uint8();
	std::shared_ptr<const CommandType> result;
	switch (command_code) {
		case RESET:
			result = _commandFactory->create_reset_command();
			break;
		case SHOW:
			result = _commandFactory->create_show_command();
			break;
		case SET_BRIGHTNESS:
			result = _commandFactory->create_set_brightness_command(parser.read_uint8());
			break;
		case CREATE_LAYER:
			result = _commandFactory->create_create_layer_command(parser.read_uint8(), parser.read_point(),
																  parser.read_uint16(), parser.read_uint16());
			break;
		case SELECT_LAYER:
			result = _commandFactory->create_select_layer_command(parser.read_uint8());
			break;
		case DRAW_PIXEL:
			result = _commandFactory->create_draw_pixel_command(parser.read_point(), parser.read_pixel());
			break;
		case DRAW_RECTANGLE:
			result = _commandFactory->create_draw_rectangle_command(parser.read_point(),
																	parser.read_uint16(), parser.read_uint16(),
																	parser.read_pixel());
			break;
		case DEFINE_IMAGE:
			result = _commandFactory->create_define_image_command(parser.read_string(), parser.read_image());
			break;
		case DRAW_IMAGE:
			result = _commandFactory->create_draw_image_command(parser.read_point(), parser.read_string());
			break;
		default:
			boost::throw_exception(CommandError("Unknown command type"));
	}
	parser.ensure_at_end();

	return result;
}

#endif //DISPLAYSERVER_COMMAND_H
