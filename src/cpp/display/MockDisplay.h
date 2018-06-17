#ifndef DISPLAYSERVER_MOCKDISPLAY_H
#define DISPLAYSERVER_MOCKDISPLAY_H

#include <led-matrix.h>
#include <graphics.h>
#include "Display.h"

class MockDisplay : public Display {
public:
	MockDisplay();

	~MockDisplay() override;

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

	void SetLayerAlpha(uint8_t layer, uint8_t alpha) override;

	void Show() override;
};

#endif //DISPLAYSERVER_MOCKDISPLAY_H
