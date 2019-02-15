#include <iostream>
#include <server/LedMatrixDisplay.h>

using namespace rgb_matrix;

const uint16_t ROWS = 32;
const uint16_t CHAIN_LENGTH = 2;
const uint16_t COLUMNS = CHAIN_LENGTH * 32;

bool LedMatrixDisplay::initialize(int argc, char *argv[]) {
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

	return true;
}

void LedMatrixDisplay::show(const LayerMap &layer_set) {
	_frameCanvas->Clear();

	for (int16_t x = 0; x < ROWS; x++) {
		for (int16_t y = 0; y < COLUMNS; y++) {
			Point location(x, y);
			Pixel value = layer_set.get_pixel(location);
			_frameCanvas->SetPixel(x, y, value.get_r(), value.get_g(), value.get_b());
		}
	}

	_frameCanvas = _rgbMatrix->SwapOnVSync(_frameCanvas);
}

void LedMatrixDisplay::set_brightness(uint8_t brightness) {
	_rgbMatrix->SetBrightness(brightness);

}
