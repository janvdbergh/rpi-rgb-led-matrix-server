#ifndef DISPLAYSERVER_LAYER_H
#define DISPLAYSERVER_LAYER_H

#include <cstdint>
#include <vector>
#include <graphics.h>

class Layer: public rgb_matrix::Canvas {
public:
	Layer(uint16_t width, uint16_t height);

	void DrawPixel(int16_t x, int16_t y, rgb_matrix::Color color) {
		this->SetPixel(x, y, color.r, color.g, color.b);
	}
	void SetPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue) override;

	void Clear() override { Fill(0, 0, 0); }
	void Fill(uint8_t red, uint8_t green, uint8_t blue) override;

	int width() const override { return _width; };
	int height() const override { return _height; };

	uint8_t GetAlpha() const { return _alpha; }
	uint8_t GetRed(int16_t x, int16_t y) const;
	uint8_t GetGreen(int16_t x, int16_t y) const;
	uint8_t GetBlue(int16_t x, int16_t y) const;

private:
	std::vector<uint8_t> _r, _g, _b;
	uint8_t _alpha;
	uint16_t _width, _height;

	uint16_t _index(int16_t x, int16_t y) const;
};


#endif //DISPLAYSERVER_LAYER_H
