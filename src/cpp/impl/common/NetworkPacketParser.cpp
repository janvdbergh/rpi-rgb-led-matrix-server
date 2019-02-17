
#include <common/NetworkPacketParser.h>
#include <common/NetworkError.h>

uint8_t NetworkPacketParser::read_uint8() {
	if (_position >= _networkPacket->get_data().size()) {
		boost::throw_exception(NetworkError(MESSAGE_ERROR, "Packet too small"));
	}
	return _networkPacket->get_data()[_position++];
}

int16_t NetworkPacketParser::read_int16() {
	int8_t byte1 = read_uint8();
	int8_t byte2 = read_uint8();

	return (byte2 << 8) + byte1;
}

uint16_t NetworkPacketParser::read_uint16() {
	int8_t byte1 = read_uint8();
	int8_t byte2 = read_uint8();

	return static_cast<uint16_t>((byte2 << 8) + byte1);
}

Point NetworkPacketParser::read_point() {
	return {read_int16(), read_int16()};
}

Pixel NetworkPacketParser::read_pixel() {
	return {read_uint8(), read_uint8(), read_uint8(), read_uint8()};
}

std::string NetworkPacketParser::read_string() {
	uint16_t size = read_uint16();

	std::string result;
	for (uint16_t i = 0; i < size; i++) {
		result.push_back(static_cast<char>(read_uint8()));
	}

	return result;
}

ImagePtr NetworkPacketParser::read_image() {
	uint16_t width = read_uint16();
	uint16_t height = read_uint16();

	ImagePtr image = ImagePtr(new Image(width, height));
	for (uint16_t y = 0; y < image->get_height(); y++) {
		for (uint16_t x = 0; x < image->get_width(); x++) {
			image->set_pixel(x, y, read_pixel());
		}
	}

	return image;
}

void NetworkPacketParser::ensure_at_end() {
	if (_position + 1 != _networkPacket->get_data().size()) {
		boost::throw_exception(NetworkError(MESSAGE_ERROR, "Packet too large"));
	}
}