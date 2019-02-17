#include <utility>

#include <boost/throw_exception.hpp>
#include <common/Command.h>
#include <common/NetworkPacketBuilder.h>
#include <common/NetworkPacketParser.h>

NetworkPacketPtr Command::packet() const {
	return NetworkPacketBuilder()
			.add_uint8(get_command_code())
			.build();
}

NetworkPacketPtr SetBrightnessCommand::packet() const {
	return NetworkPacketBuilder()
			.add_uint8(get_command_code())
			.add_uint8(_brightness)
			.build();
}

NetworkPacketPtr CreateLayerCommand::packet() const {
	return NetworkPacketBuilder()
			.add_uint8(get_command_code())
			.add_uint8(_index)
			.add_point(_offset)
			.add_uint16(_width)
			.add_uint16(_height)
			.build();
}

NetworkPacketPtr SelectLayerCommand::packet() const {
	return NetworkPacketBuilder()
			.add_uint8(get_command_code())
			.add_uint8(_index)
			.build();
}

NetworkPacketPtr DrawPixelCommand::packet() const {
	return NetworkPacketBuilder()
			.add_uint8(get_command_code())
			.add_point(_location)
			.add_pixel(_value)
			.build();
}

NetworkPacketPtr DrawRectangleCommand::packet() const {
	return NetworkPacketBuilder()
			.add_uint8(get_command_code())
			.add_point(_location)
			.add_uint16(_width)
			.add_uint16(_height)
			.add_pixel(_value)
			.build();
}

NetworkPacketPtr DefineImageCommand::packet() const {
	return NetworkPacketBuilder()
			.add_uint8(get_command_code())
			.add_string(_name)
			.add_image(_image)
			.build();
}

NetworkPacketPtr DrawImageCommand::packet() const {
	return NetworkPacketBuilder()
			.add_uint8(get_command_code())
			.add_point(_location)
			.add_string(_name)
			.build();
}

template<class CommandPtr>
CommandPtr CommandReader<CommandPtr>::read_command(NetworkPacketPtr packet) {
	NetworkPacketParser parser(std::move(packet));

	uint8_t command_code = parser.read_uint8();
	CommandPtr result;
	switch (command_code) {
		case RESET:
			result = _commandFactory.create_reset_command();
			break;
		case SHOW:
			result = _commandFactory.create_show_command();
			break;
		case SET_BRIGHTNESS:
			result = _commandFactory.create_set_brightness_command(parser.read_uint8());
			break;
		case CREATE_LAYER:
			result = _commandFactory.create_create_layer_command(parser.read_uint8(), parser.read_point(),
																 parser.read_uint16(), parser.read_uint16());
			break;
		case SELECT_LAYER:
			result = _commandFactory.create_select_layer_command(parser.read_uint8());
			break;
		case DRAW_PIXEL:
			return _commandFactory.create_draw_pixel_command(parser.read_int16(), parser.read_int16(),
															 parser.read_pixel(),
															 parser.read_uint8(), parser.read_uint8());
			break;
		case DRAW_RECTANGLE:
			return _commandFactory.create_draw_rectangle_command(parser.read_point(),
																 parser.read_uint16(), parser.read_uint16(),
																 parser.read_pixel());
			break;
		case DEFINE_IMAGE:
			result = _commandFactory.create_define_image_command(parser.read_string(), parser.read_image());
			break;
		case DRAW_IMAGE:
			result = _commandFactory.create_draw_image_command(parser.read_point(), parser.read_string());
		default:
			boost::throw_exception(CommandError("Unknown command type"));
	}
	parser.ensure_at_end();

	return result;
}
