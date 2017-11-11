#ifndef DISPLAYSERVER_DISPLAY_H
#define DISPLAYSERVER_DISPLAY_H

#include <string>
#include <boost/shared_ptr.hpp>

class Display {
public:
    virtual ~Display() = default;

    virtual bool Initialize(int argc, char **argv) =0;

    virtual void SetColor(uint8_t r, uint8_t g, uint8_t b) =0;

    virtual void Clear() =0;

    virtual void DrawPixel(int16_t x, int16_t y) =0;

    virtual void DrawRectangle(int16_t x, int16_t y, int16_t width, int16_t height) =0;

    virtual void DrawDigit(uint8_t position, uint8_t digit) =0;

    virtual void DrawSmallText(int16_t x, int16_t y, std::string text) =0;

    virtual void DrawLargeText(int16_t x, int16_t y, std::string text) =0;

    virtual void Show() =0;
};

boost::shared_ptr<Display> createDisplay();

#endif //DISPLAYSERVER_DISPLAY_H
