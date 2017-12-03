#ifndef DISPLAYSERVER_IMAGE_H
#define DISPLAYSERVER_IMAGE_H

#include <vector>

class Image {
public:
    uint8_t red(uint16_t x, uint16_t y) const { return _red[index(x, y)]; }

    uint8_t green(uint16_t x, uint16_t y) const { return _green[index(x, y)]; }

    uint8_t blue(uint16_t x, uint16_t y) const { return _blue[index(x, y)]; }

    uint8_t alpha(uint16_t x, uint16_t y) const { return _alpha[index(x, y)]; }

private:
    uint16_t _width, _height;
    std::vector<uint8_t> _red, _green, _blue, _alpha;

    Image(uint16_t width, uint16_t height);

    void setPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    size_t index(uint16_t x, uint16_t y) const {
        return y * _width + x;
    }

    friend class ImageBuilder;
};

class ImageBuilder {
public:
    ImageBuilder(uint16_t width, uint16_t height) : _image(width, height) {}

    void setPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
        _image.setPixel(x, y, red, green, blue, alpha);
    }

    Image build() const {
        return _image;
    }

private:
    Image _image;
};


#endif //DISPLAYSERVER_IMAGE_H
