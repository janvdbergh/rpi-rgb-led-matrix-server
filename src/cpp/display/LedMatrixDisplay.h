#ifndef DISPLAYSERVER_LEDMATRIXDISPLAY_H
#define DISPLAYSERVER_LEDMATRIXDISPLAY_H

#include <led-matrix.h>
#include <graphics.h>
#include "Display.h"
#include "Layer.h"

class LedMatrixDisplay : public Display {
public:
	LedMatrixDisplay() : _frameCanvas(nullptr), _color(255, 255, 255), _currentLayer(0) { this->SetLayer(0); }

	bool Initialize(int argc, char **argv) override;

	void SetBrightness(uint8_t brightness) override;

	void SetColor(uint8_t r, uint8_t g, uint8_t b) override;

	void Clear() override;

	void DrawPixel(int16_t x, int16_t y) override;

	void DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height) override;

	void DrawDigit(uint8_t position, uint8_t digit) override;

	void DrawSmallText(int16_t x, int16_t y, std::string text) override;

	void DrawLargeText(int16_t x, int16_t y, std::string text) override;

	void DrawImage(int16_t x, int16_t y, const std::string &imageName) override;

	void SetLayer(uint8_t layer) override;

	void ClearLayer() override;

	void Show() override;

private:
	const std::shared_ptr<Layer> getCurrentLayer() { return _layers[_currentLayer]; };

	std::unique_ptr<rgb_matrix::RGBMatrix> _rgbMatrix;
	rgb_matrix::FrameCanvas *_frameCanvas;

	rgb_matrix::Color _color;
	std::unique_ptr<rgb_matrix::Font> _smallFont;
	std::unique_ptr<rgb_matrix::Font> _largeFont;

	uint8_t _currentLayer;
	std::vector<std::shared_ptr<Layer>> _layers;
};

#endif //DISPLAYSERVER_LEDMATRIXDISPLAY_H
