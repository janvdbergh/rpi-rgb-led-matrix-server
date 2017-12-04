#ifndef DISPLAYSERVER_IMAGE_H
#define DISPLAYSERVER_IMAGE_H

#include <vector>
#include <boost/shared_ptr.hpp>

class Image {
public:
    uint16_t GetHeight() const { return _height; }
    uint16_t GetWidth() const { return _width; }

    uint8_t GetRed(uint16_t x, uint16_t y) const { return _red[Index(x, y)]; }

    uint8_t GetGreen(uint16_t x, uint16_t y) const { return _green[Index(x, y)]; }

    uint8_t GetBlue(uint16_t x, uint16_t y) const { return _blue[Index(x, y)]; }

private:
    uint16_t _width, _height;
    std::vector<uint8_t> _red, _green, _blue;

    Image(uint16_t width, uint16_t height);

    void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);

    size_t Index(uint16_t x, uint16_t y) const {
        return y * _width + x;
    }

    friend class ImageBuilder;
};

class ImageBuilder {
public:
    ImageBuilder(uint16_t width, uint16_t height) : _image(new Image(width, height)) {}

    void SetPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);

    boost::shared_ptr<const Image> Build() const {
        return _image;
    }

private:
    boost::shared_ptr<Image> _image;
};


#endif //DISPLAYSERVER_IMAGE_H
