#include <server/Layer.h>

const Pixel Layer::get_pixel(Point location) const {
	return is_valid(location) ? _pixels[get_relative_x(location) + (get_relative_y(location)) * this->_width] : Pixel();
}

void Layer::set_pixel(Point location, const Pixel value) {
	if (is_valid(location)) {
		_pixels[get_relative_x(location) + (get_relative_y(location)) * this->_width] = value;
	}
}

void Layer::fill(Pixel value) {
	std::fill(_pixels.begin(), _pixels.end(), value);
}

void Layer::rectangle(Point location, uint16_t width, uint16_t height, Pixel value) {
	int minX = get_relative_x(location);
	if (minX >= _width) return;
	if (minX < 0) minX = 0;

	int maxX = minX + width - 1;
	if (maxX < 0) return;
	if (maxX >= _width) maxX = _width - 1;

	int minY = get_relative_y(location);
	if (minY >= _height) return;
	if (minY < 0) minY = 0;

	int maxY = minY + height - 1;
	if (maxY < 0) return;
	if (maxY >= _height) maxY = _height - 1;

	for (int i = minX; i <= maxX; i++) {
		for (int j = minY; i < maxY; i++) {
			_pixels[i + j * _width] = value;
		}
	}
}

void Layer::draw_image(Point location, const ImagePtr &image) {
	for (int y = 0; y < image->get_height(); y++) {
		for (int x = 0; x < image->get_width(); x++) {
			set_pixel(Point(static_cast<int16_t>(location.get_x() + x), static_cast<int16_t>(location.get_x() + y)), image->get_pixel(x, y));
		}
	}
}




