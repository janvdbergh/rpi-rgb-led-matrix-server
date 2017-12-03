//
// Created by Jan Van den Bergh on 03/12/2017.
//

#include "Image.h"

Image::Image(uint16_t width, uint16_t height) : _width(width), _height(height) {
    _red.resize(width * height);
    _green.resize(width * height);
    _blue.resize(width * height);
}

void Image::SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
    size_t index = this->Index(x, y);

    _red[index] = red;
    _green[index] = green;
    _blue[index] = blue;
}