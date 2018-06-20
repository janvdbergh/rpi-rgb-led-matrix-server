#include "Layer.h"

Layer::Layer(uint16_t width, uint16_t height) :
		_width(width), _height(height), _r(width * height), _g(width * height), _b(width * height), _alpha(255) {
}

void Layer::SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		return;
	}

	uint16_t index = _index(static_cast<int16_t>(x), static_cast<int16_t>(y));
	_r[index] = red;
	_g[index] = green;
	_b[index] = blue;
}

void Layer::Fill(uint8_t red, uint8_t green, uint8_t blue) {
	std::fill(_r.begin(), _r.end(), red);
	std::fill(_g.begin(), _g.end(), green);
	std::fill(_b.begin(), _b.end(), blue);
}

uint8_t Layer::GetRed(int16_t x, int16_t y) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		return 0;
	}
	return _r[_index(x, y)];
}

uint8_t Layer::GetGreen(int16_t x, int16_t y) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		return 0;
	}\

	return _g[_index(x, y)];
}

uint8_t Layer::GetBlue(int16_t x, int16_t y) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		return 0;
	}
	return _b[_index(x, y)];
}

uint16_t Layer::_index(int16_t x, int16_t y) const {
	return x + y * this->_width;
}
