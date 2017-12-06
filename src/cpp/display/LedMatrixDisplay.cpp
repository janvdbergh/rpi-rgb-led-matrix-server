#include <iostream>
#include "LedMatrixDisplay.h"

const int16_t DIGIT_START_X = 7;
const int16_t DIGIT_START_Y = 12;
const char *BDF_SMALL_FONT_FILE = "fonts/6x9.bdf";
const char *BDF_LARGE_FONT_FILE = "fonts/9x18.bdf";

using namespace rgb_matrix;

bool LedMatrixDisplay::Initialize(int argc, char *argv[]) {
    RGBMatrix::Options options;
    options.hardware_mapping = "adafruit-hat-pwm";
    options.rows = 32;
    options.chain_length = 2;
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

void LedMatrixDisplay::SetColor(uint8_t r, uint8_t g, uint8_t b) {
    _color = Color(r, g, b);
}

void LedMatrixDisplay::Clear() {
    _frameCanvas->Clear();
}

void LedMatrixDisplay::DrawPixel(int16_t x, int16_t y) {
    _frameCanvas->SetPixel(x, y, _color.r, _color.g, _color.b);
}

void LedMatrixDisplay::DrawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            _frameCanvas->SetPixel(x + i, y + j, _color.r, _color.g, _color.b);
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
    DrawText(_frameCanvas, *_smallFont, x, y, _color, text.c_str());
}

void LedMatrixDisplay::DrawLargeText(int16_t x, int16_t y, std::string text) {
    DrawText(_frameCanvas, *_largeFont, x, y, _color, text.c_str());
}

void LedMatrixDisplay::DrawImage(int16_t x, int16_t y, const std::string &imageName) {
    boost::shared_ptr<const Image> image = GetImage(imageName);

    if (image) {
        for (uint16_t i = 0; i < image->GetWidth(); i++) {
            for (uint16_t j = 0; j < image->GetHeight(); j++) {
                _frameCanvas->SetPixel(x + i, y + j, image->GetRed(i, j), image->GetGreen(i, j), image->GetBlue(i, j));
            }
        }
    }
}

void LedMatrixDisplay::Show() {
    _frameCanvas = _rgbMatrix->SwapOnVSync(_frameCanvas);
}

boost::shared_ptr<Display> CreateDisplay() {
    return boost::shared_ptr<Display>(new LedMatrixDisplay());
}