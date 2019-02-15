#include <boost/throw_exception.hpp>
#include <common/Command.h>

std::vector<uint8_t> Command::get_command_code_data() const {
	std::vector<uint8_t> data;
	data.insert(data.end(), COMMAND_PREFIX);
	data.insert(data.end(), get_commandCode());
	return data;
}

NetworkPacketPtr Command::packet() const {
	return NetworkPacketPtr(new NetworkPacket(get_command_code_data()));
}

inline void assert_minimum_size(const std::vector<uint8_t> &data, unsigned long minimum_size) {
	unsigned long size = data.size();
	if (size < minimum_size) {
		boost::throw_exception(CommandError("Invalid packet size"));
	}
}

inline void assert_size(const std::vector<uint8_t> &data, unsigned long required_size) {
	unsigned long size = data.size();
	if (size != required_size) {
		boost::throw_exception(CommandError("Invalid packet size"));
	}
}

inline uint8_t read_uint8(const std::vector<uint8_t> &data, unsigned long index) {
	return (uint8_t) data[index];
}

inline int16_t read_int16(const std::vector<uint8_t> &data, unsigned long index) {
	uint8_t byte1 = read_uint8(data, index);
	uint8_t byte2 = read_uint8(data, index + 1);
	return (int16_t) ((byte2 << 8) + byte1);
}

inline uint16_t read_uint16(const std::vector<uint8_t> &data, unsigned long index) {
	uint8_t byte1 = read_uint8(data, index);
	uint8_t byte2 = read_uint8(data, index + 1);
	return (uint16_t) ((byte2 << 8) + byte1);
}

template<class CommandPtr>
CommandPtr CommandReader<CommandPtr>::read_command(NetworkPacketPtr packet) {
	const std::vector<uint8_t> &data = packet->get_data();
	assert_minimum_size(data, 1);

	switch (data[0]) {
		case RESET:
			assert_size(data, 1);
			return _commandFactory.create_reset_command();
		case SHOW:
			assert_size(data, 1);
			return _commandFactory.create_show_command();
		case SET_BRIGHTNESS:
			assert_size(data, 2);
			return _commandFactory.create_set_brightness_command(read_uint8(data, 1));
		case CREATE_LAYER:
			assert_size(data, 10);
			return _commandFactory.create_create_layer_command(read_uint8(data, 1),
															   read_int16(data, 2), read_int16(data, 4),
															   read_uint16(data, 6), read_uint16(data, 8));
		case SELECT_LAYER:
			assert_size(data, 2);
			return _commandFactory.create_select_layer_command(read_uint8(data, 1));
		case DRAW_PIXEL:
			assert_size(data, 9);
			return _commandFactory.create_draw_pixel_command(read_int16(data, 1), read_int16(data, 3),
															 Pixel(read_uint8(data, 5), read_uint8(data, 6),
																   read_uint8(data, 7), read_uint8(data, 8)));
		case DRAW_RECTANGLE:
			assert_size(data, 13);
			return _commandFactory.create_draw_rectangle_command(read_int16(data, 1), read_int16(data, 3),
															 read_int16(data, 5), read_int16(data, 7),
															 Pixel(read_uint8(data, 9), read_uint8(data, 10),
																   read_uint8(data, 11), read_uint8(data, 12)));

		case DEFINE_IMAGE:
			// TODO read string and image
			return nullptr;
		case DRAW_IMAGE:
			// TODO read string
			return nullptr;
		default:
			boost::throw_exception(CommandError("Unknown command type"));
	}
}

