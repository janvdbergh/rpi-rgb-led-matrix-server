#include <iostream>
#include <stdio.h>
#include "LedMatrixDisplay.h"

const int DIGIT_START_X = 7;
const int DIGIT_START_Y = 12;

using namespace rgb_matrix;

LedMatrixDisplay::LedMatrixDisplay() : _rgbMatrix(0), _frameCanvas(0), _color(255, 255, 255) {}

LedMatrixDisplay::~LedMatrixDisplay() {
    delete _rgbMatrix;
}

bool LedMatrixDisplay::Initialize(int argc, char *argv[]) {
    RGBMatrix::Options options;
    options.hardware_mapping = "adafruit-hat-pwm";
    options.rows = 32;
    options.chain_length = 2;
    options.parallel = 1;

    RuntimeOptions runtimeOptions;
    runtimeOptions.gpio_slowdown = 2;
    _rgbMatrix = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &options, &runtimeOptions);

    if (_rgbMatrix == 0) {
        std::cerr << "Could not initialize matrix" << std::endl;
        rgb_matrix::PrintMatrixFlags(stderr);
        return false;
    }

    _frameCanvas = _rgbMatrix->CreateFrameCanvas();

    return true;
}

void LedMatrixDisplay::SetColor(uint8_t r, uint8_t g, uint8_t b) {
    _color = Color(r, g, b);
}

void LedMatrixDisplay::Clear() {
    _frameCanvas->Clear();
}

void LedMatrixDisplay::DrawPixel(int x, int y) {
    _frameCanvas->SetPixel(x, y, _color.r, _color.g, _color.b);
}

void LedMatrixDisplay::DrawRectangle(int x, int y, int width, int height) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            _frameCanvas->SetPixel(x + i, y + j, _color.r, _color.g, _color.b);
        }
    }
}

void LedMatrixDisplay::DrawDigit(int position, int digit) {
    int startX = DIGIT_START_X + position * 12;
    if (position >= 2) {
        startX += 4;
    }

    // top
    if (digit==0 || digit==2 || digit==3 || digit==5 || digit==6 || digit==7 || digit==8 || digit==9) {
        DrawRectangle(startX, DIGIT_START_Y, 10, 2);
    }

    // middle
    if (digit==2 || digit==3 || digit==4 || digit==5 || digit==6 || digit==8 || digit==9) {
        DrawRectangle(startX, DIGIT_START_Y + 8, 10, 2);
    }

    // bottom
    if (digit==0 || digit==2 || digit==3 || digit==5 || digit==6 || digit==8 || digit==9) {
        DrawRectangle(startX, DIGIT_START_Y + 16, 10, 2);
    }

    // left top
    if (digit==0 || digit==4 || digit==5 || digit==6 || digit==8 || digit==9) {
        DrawRectangle(startX, DIGIT_START_Y, 2, 10);
    }

    // right top
    if (digit==0 || digit==1 || digit==2 || digit==3 || digit==4 || digit==7 || digit==8 || digit==9) {
        DrawRectangle(startX + 8, DIGIT_START_Y, 2, 10);
    }

    // left bottom
    if (digit==0 || digit==2 || digit==6 || digit==8) {
        DrawRectangle(startX, DIGIT_START_Y + 8, 2, 10);
    }

    // right bottom
    if (digit==0 || digit==1 || digit==3 || digit==4 || digit==5 || digit==6 || digit==7 || digit==8 || digit==9) {
        DrawRectangle(startX + 8, DIGIT_START_Y + 8, 2, 10);
    }
}

void LedMatrixDisplay::Show() {
    _rgbMatrix->SwapOnVSync(_frameCanvas);
}
