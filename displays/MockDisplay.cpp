#include <iostream>
#include "MockDisplay.h"

const int DIGIT_START_X = 7;
const int DIGIT_START_Y = 12;
const char *BDF_SMALL_FONT_FILE = "fonts/6x9.bdf";
const char *BDF_LARGE_FONT_FILE = "fonts/9x18.bdf";

using namespace displays;

MockDisplay::MockDisplay() {
    std::cout << "MockDisplay()" << std::endl;
}

MockDisplay::~MockDisplay() {
    std::cout << "~MockDisplay()" << std::endl;
}

bool MockDisplay::Initialize(int argc, char *argv[]) {
    std::cout << "Initialize() with ";
    for (int i = 1; i < argc; i++) {
        if (i != 1) std::cout << " ";
        std::cout << argv[i];
    }
    std::cout << std::endl;
    return true;
}

void MockDisplay::SetColor(uint8_t r, uint8_t g, uint8_t b) {
    std::cout << "SetColor(" << r << ", " << g << ", " << b << ")" << std::endl;
}

void MockDisplay::Clear() {
    std::cout << "Clear()" << std::endl;
}

void MockDisplay::DrawPixel(int x, int y) {
    std::cout << "DrawPixel(" << x << ", " << y << ")" << std::endl;
}

void MockDisplay::DrawRectangle(int x, int y, int width, int height) {
    std::cout << "DrawRectangle(" << x << ", " << y << ", " << width << ", " << height << ")" << std::endl;
}

void MockDisplay::DrawDigit(int position, int digit) {
    std::cout << "DrawDigit(" << position << ", " << digit << ")" << std::endl;
}

void MockDisplay::DrawSmallText(int x, int y, std::string text) {
    std::cout << "DrawSmallText(" << x << ", " << y << ", \"" << text << "\")" << std::endl;
}

void MockDisplay::DrawLargeText(int x, int y, std::string text) {
    std::cout << "DrawLargeText(" << x << ", " << y << ", \"" << text << "\")" << std::endl;
}

void MockDisplay::Show() {
    std::cout << "Show()" << std::endl << std::endl;
}

boost::shared_ptr<Display> displays::createDisplay() {
    return boost::shared_ptr<MockDisplay>(new MockDisplay());
}