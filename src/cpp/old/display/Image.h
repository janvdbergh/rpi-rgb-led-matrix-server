#ifndef DISPLAYSERVER_IMAGE_H
#define DISPLAYSERVER_IMAGE_H

#include <vector>
#include <boost/shared_ptr.hpp>

class Image {
public:
	uint16_t get_height() const { return _height; }

	uint16_t get_width() const { return _width; }

	uint8_t get_red(uint16_t x, uint16_t y) const { return _red[Index(x, y)]; }

	uint8_t get_green(uint16_t x, uint16_t y) const { return _green[Index(x, y)]; }

	uint8_t get_blue(uint16_t x, uint16_t y) const { return _blue[Index(x, y)]; }

private:
	uint16_t _width, _height;
	std::vector<uint8_t> _red, _green, _blue;

	Image(uint16_t width, uint16_t height);

	void set_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);

	size_t index(uint16_t x, uint16_t y) const {
		return y * _width + x;
	}

	friend class ImageBuilder;
};

typedef boost::shared_ptr<Image> ImagePtr;

class ImageBuilder {
public:
	ImageBuilder(uint16_t width, uint16_t height) : _image(new Image(width, height)) {}

	ImageBuilder& set_pixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);

	ImagePtr build() const {
		return _image;
	}

private:
	ImagePtr _image;
};


#endif //DISPLAYSERVER_IMAGE_H
