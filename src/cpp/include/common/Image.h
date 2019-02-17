#ifndef DISPLAYSERVER_IMAGE_H
#define DISPLAYSERVER_IMAGE_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "Pixel.h"

class Image {
public:
	Image(uint16_t width, uint16_t height);

	uint16_t get_height() const { return _height; }

	uint16_t get_width() const { return _width; }

	Pixel get_pixel(uint16_t x, uint16_t y) const { return _pixels[index(x, y)]; }

	void set_pixel(uint16_t x, uint16_t y, Pixel pixel);

private:
	uint16_t _width, _height;

	std::vector<Pixel> _pixels;

	size_t index(uint16_t x, uint16_t y) const {
		return y * _width + x;
	}

	friend class ImageBuilder;
};

typedef boost::shared_ptr<Image> ImagePtr;

class ImageBuilder {
public:
	ImageBuilder(uint16_t width, uint16_t height) : _image(new Image(width, height)) {}

	ImageBuilder &set_pixel(uint16_t x, uint16_t y, Pixel pixel);

	ImagePtr build() const {
		return _image;
	}

private:
	boost::shared_ptr<Image> _image;
};


#endif //DISPLAYSERVER_IMAGE_H
