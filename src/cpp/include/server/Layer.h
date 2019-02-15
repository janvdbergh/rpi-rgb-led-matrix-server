#ifndef DISPLAYSERVER_LAYER_H
#define DISPLAYSERVER_LAYER_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <common/Point.h>
#include <common/Command.h>

class Layer {
public:
	Layer(Point offset, uint16_t width, uint16_t height) :
			_width(width), _height(height), _offset(offset), _alpha(255), _pixels(width * height) {}

	const Pixel get_pixel(Point location) const;

	void set_pixel(Point location, Pixel value);

	void clear() { fill(Pixel()); }

	void fill(Pixel value);

	void rectangle(Point location, uint16_t width, uint16_t height, Pixel value);

	void draw_image(Point location, const ImagePtr& image);

	int get_width() const { return _width; };

	int get_height() const { return _height; };

	Point get_offset() const { return _offset; }

	uint8_t get_alpha() const { return _alpha; }

	void set_alpha(uint8_t alpha) { _alpha = alpha; }

private:
	std::vector<Pixel> _pixels;
	uint8_t _alpha;
	Point _offset;
	uint16_t _width, _height;

	int16_t get_relative_x(Point location) const {
		return location.get_x() - _offset.get_x();
	}

	int16_t get_relative_y(Point location) const {
		return location.get_y() - _offset.get_y();
	}

	bool is_valid(Point location) const {
		int16_t x0 = get_relative_x(location);
		int16_t y0 = get_relative_y(location);
		return x0 >= 0 && x0 < _width && y0 >= 0 && y0 < _height;
	}
};

typedef boost::shared_ptr<Layer> LayerPtr;


#endif //DISPLAYSERVER_LAYER_H
