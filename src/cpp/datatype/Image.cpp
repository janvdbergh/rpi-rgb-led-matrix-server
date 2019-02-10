#include "Image.h"

Image::Image(uint16_t width, uint16_t height) : _width(width), _height(height) {
	_pixels.resize(width * height);
}

void Image::set_pixel(uint16_t x, uint16_t y, Pixel pixel) {
	size_t index = this->index(x, y);
	_pixels[index] = pixel;
}

ImageBuilder& ImageBuilder::set_pixel(uint16_t x, uint16_t y, Pixel pixel) {
	_image->set_pixel(x, y, pixel);
	return *this;
}
