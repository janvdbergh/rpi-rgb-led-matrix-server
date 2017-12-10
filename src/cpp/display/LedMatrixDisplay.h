#ifndef DISPLAYSERVER_LEDMATRIXDISPLAY_H
#define DISPLAYSERVER_LEDMATRIXDISPLAY_H

#include <led-matrix.h>
#include <graphics.h>
#include "Display.h"

class LedMatrixDisplay : public Display {
public:
	LedMatrixDisplay() : _frameCanvas(nullptr), _color(255, 255, 255) {}

	bool Initialize(int argc, char **argv) override;

	void SetColor(uint8_t r, uint8_t g, uint8_t b) override;

	void Clear() override;

	void DrawPixel(int16_t x, int16_t y) override;

	void DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height) override;

	void DrawDigit(uint8_t position, uint8_t digit) override;

	void DrawSmallText(int16_t x, int16_t y, std::string text) override;

	void DrawLargeText(int16_t x, int16_t y, std::string text) override;

	virtual void DrawImage(int16_t x, int16_t y, const std::string &imageName) override;

	void Show() override;

private:
	std::unique_ptr<rgb_matrix::RGBMatrix> _rgbMatrix;
	rgb_matrix::FrameCanvas *_frameCanvas;

	rgb_matrix::Color _color;
	std::unique_ptr<rgb_matrix::Font> _smallFont;
	std::unique_ptr<rgb_matrix::Font> _largeFont;
};

#endif //DISPLAYSERVER_LEDMATRIXDISPLAY_H
