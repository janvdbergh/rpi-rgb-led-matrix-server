#include <iostream>
#include "LedMatrixDisplay.h"

const int16_t DIGIT_START_X = 7;
const int16_t DIGIT_START_Y = 12;
const char *BDF_SMALL_FONT_FILE = "fonts/6x9.bdf";
const char *BDF_LARGE_FONT_FILE = "fonts/9x18.bdf";

const uint16_t ROWS = 32;
const uint16_t CHAIN_LENGTH = 2;
const uint16_t COLUMNS = CHAIN_LENGTH * 32;

using namespace rgb_matrix;

bool LedMatrixDisplay::Initialize(int argc, char *argv[]) {
	RGBMatrix::Options options;
	options.hardware_mapping = "adafruit-hat-pwm";
	options.rows = ROWS;
	options.chain_length = CHAIN_LENGTH;
	options.parallel = 1;

	RuntimeOptions runtimeOptions;
	runtimeOptions.gpio_slowdown = 2;
	_rgbMatrix = std::unique_ptr<rgb_matrix::RGBMatrix>(
			rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &options, &runtimeOptions));

	if (_rgbMatrix == nullptr) {
		std::cerr << "Could not initialize matrix" << std::endl;
		rgb_matrix::PrintMatrixFlags(stderr);
		return false;
	}

	_frameCanvas = _rgbMatrix->CreateFrameCanvas();

	_smallFont = std::make_unique<Font>();
	if (!_smallFont->LoadFont(BDF_SMALL_FONT_FILE)) {
		std::cerr << "Could not load font " << BDF_SMALL_FONT_FILE << std::endl;
		return false;
	}

	_largeFont = std::make_unique<Font>();
	if (!_largeFont->LoadFont(BDF_LARGE_FONT_FILE)) {
		std::cerr << "Could not load font " << BDF_LARGE_FONT_FILE << std::endl;
		return false;
	}

	return true;
}

void LedMatrixDisplay::SetBrightness(uint8_t brightness) {
	_rgbMatrix->SetBrightness(brightness);
}

void LedMatrixDisplay::SetColor(uint8_t r, uint8_t g, uint8_t b) {
	_color = Color(r, g, b);
}

void LedMatrixDisplay::Clear() {
	_layers.clear();

	SetLayer(0);
}

void LedMatrixDisplay::DrawPixel(int16_t x, int16_t y) {
	getCurrentLayer()->DrawPixel(x, y, _color);
}

void LedMatrixDisplay::DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height) {
	for (int16_t i = 0; i < width; i++) {
		for (int16_t j = 0; j < height; j++) {
			DrawPixel(x + i, y + j);
		}
	}
}

void LedMatrixDisplay::DrawDigit(uint8_t position, uint8_t digit) {
	int16_t startX = DIGIT_START_X + position * (int16_t) 12;
	if (position >= 2) {
		startX += 4;
	}

	// top
	if (digit == 0 || digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9) {
		DrawRectangle(startX, DIGIT_START_Y, 10, 2);
	}

	// middle
	if (digit == 2 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9) {
		DrawRectangle(startX, DIGIT_START_Y + 8, 10, 2);
	}

	// bottom
	if (digit == 0 || digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 8 || digit == 9) {
		DrawRectangle(startX, DIGIT_START_Y + 16, 10, 2);
	}

	// left top
	if (digit == 0 || digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9) {
		DrawRectangle(startX, DIGIT_START_Y, 2, 10);
	}

	// right top
	if (digit == 0 || digit == 1 || digit == 2 || digit == 3 || digit == 4 || digit == 7 || digit == 8 || digit == 9) {
		DrawRectangle(startX + (int16_t) 8, DIGIT_START_Y, 2, 10);
	}

	// left bottom
	if (digit == 0 || digit == 2 || digit == 6 || digit == 8) {
		DrawRectangle(startX, DIGIT_START_Y + 8, 2, 10);
	}

	// right bottom
	if (digit == 0 || digit == 1 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 7 || digit == 8 ||
		digit == 9) {
		DrawRectangle(startX + (int16_t) 8, DIGIT_START_Y + 8, 2, 10);
	}
}

void LedMatrixDisplay::DrawSmallText(int16_t x, int16_t y, std::string text) {
	Canvas &canvas = *this->getCurrentLayer();
	DrawText(&canvas, *_smallFont, x, y, _color, text.c_str());
}

void LedMatrixDisplay::DrawLargeText(int16_t x, int16_t y, std::string text) {
	Canvas &canvas = *this->getCurrentLayer();
	DrawText(&canvas, *_largeFont, x, y, _color, text.c_str());
}

void LedMatrixDisplay::DrawImage(int16_t x, int16_t y, const std::string &imageName) {
	ImagePtr image = GetImage(imageName);

	for (uint16_t i = 0; i < image->GetWidth(); i++) {
		for (uint16_t j = 0; j < image->GetHeight(); j++) {
			DrawPixel(x + i, y + j);
		}
	}
}

uint8_t updateUsingAlpha(uint8_t currentValue, uint8_t newValue, uint16_t alpha) {
	int value = ((255 - alpha) * currentValue + alpha * newValue) / 255;
	return static_cast<uint8_t>(value);
}

void LedMatrixDisplay::Show() {
	for (int16_t x = 0; x < COLUMNS; x++) {
		for (int16_t y = 0; y < ROWS; y++) {
			uint8_t r = 0, g = 0, b = 0;

			for (auto &_layer : _layers) {
				Layer *layer = _layer.get();

				uint8_t alpha = layer->GetAlpha();

				r = updateUsingAlpha(r, layer->GetRed(x, y), alpha);
				g = updateUsingAlpha(g, layer->GetGreen(x, y), alpha);
				b = updateUsingAlpha(b, layer->GetBlue(x, y), alpha);
			}

			_frameCanvas->SetPixel(x, y, r, g, b);
		}
	}

	_frameCanvas = _rgbMatrix->SwapOnVSync(_frameCanvas);
}

void LedMatrixDisplay::ClearLayer() {
	getCurrentLayer()->Clear();
}

void LedMatrixDisplay::SetLayerAlpha(uint8_t alpha) {
	getCurrentLayer()->SetAlpha(alpha);
}

void LedMatrixDisplay::SetLayer(uint8_t layer) {
	_currentLayer = layer;

	if (_layers.size() <= _currentLayer) {
		_layers.resize(_currentLayer + 1);
	}

	if (!_layers[_currentLayer]) {
		_layers[_currentLayer] = std::shared_ptr<Layer>(new Layer(COLUMNS, ROWS));
	}
}

boost::shared_ptr<Display> CreateDisplay() {
	return boost::shared_ptr<Display>(new LedMatrixDisplay());
}