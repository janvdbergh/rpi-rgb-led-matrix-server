#include <common/NetworkPacketBuilder.h>

NetworkPacketPtr NetworkPacketBuilder::build() const {
	return NetworkPacketPtr(new NetworkPacket(_data));
}

NetworkPacketBuilder& NetworkPacketBuilder::add_uint8(uint8_t value) {
	_data.push_back(value);
	return *this;
}

NetworkPacketBuilder &NetworkPacketBuilder::add_int16(int16_t value) {
	_data.push_back(static_cast<uint8_t>(value % 256));
	_data.push_back(static_cast<uint8_t>(value / 256));
	return *this;
}

NetworkPacketBuilder &NetworkPacketBuilder::add_uint16(uint16_t value) {
	_data.push_back(static_cast<uint8_t>(value % 256));
	_data.push_back(static_cast<uint8_t>(value / 256));
	return *this;
}

NetworkPacketBuilder &NetworkPacketBuilder::add_point(Point point) {
	add_int16(point.get_x());
	add_int16(point.get_y());
	return *this;
}

NetworkPacketBuilder &NetworkPacketBuilder::add_pixel(Pixel pixel) {
	add_uint8(pixel.get_r());
	add_uint8(pixel.get_g());
	add_uint8(pixel.get_g());
	add_uint8(pixel.get_alpha());
	return *this;
}

NetworkPacketBuilder &NetworkPacketBuilder::add_string(const std::string &str) {
	auto size = static_cast<uint16_t>(str.length());
	add_uint16(size);

	for (char ch : str) {
		_data.push_back(static_cast<unsigned char &&>(ch));
	}

	return *this;
}

NetworkPacketBuilder &NetworkPacketBuilder::add_image(ImagePtr image) {
	add_uint16(image->get_width());
	add_uint16(image->get_height());

	for (uint16_t y = 0; y < image->get_height(); y++) {
		for (uint16_t x = 0; x < image->get_width(); x++) {
			add_pixel(image->get_pixel(x, y));
		}
	}

	return *this;
}
