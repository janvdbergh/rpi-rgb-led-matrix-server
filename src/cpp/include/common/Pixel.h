#ifndef DISPLAYSERVER_PIXEL_H
#define DISPLAYSERVER_PIXEL_H

class Pixel {
public:
private:
public:
	Pixel() {}

	Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) :
			_r(r), _g(g), _b(b), _alpha(alpha) {}

	uint8_t get_r() const { return _r; }

	void set_r(uint8_t r) { _r = r; }

	uint8_t get_g() const { return _g; }

	void set_g(uint8_t g) { _g = g; }

	uint8_t get_b() const { return _b; }

	void set_b(uint8_t b) { _b = b; }

	uint8_t get_alpha() const { return _alpha; }

private:
	uint8_t _r, _g, _b, _alpha;
};

#endif //DISPLAYSERVER_PIXEL_H
