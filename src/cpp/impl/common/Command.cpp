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
