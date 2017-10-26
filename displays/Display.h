#ifndef DISPLAYSERVER_DISPLAY_H
#define DISPLAYSERVER_DISPLAY_H

#include <string>

namespace displays {

    class Display {
    public:
        virtual ~Display() = default;

        virtual bool Initialize(int argc, char **argv) =0;

        virtual void SetColor(uint8_t r, uint8_t g, uint8_t b) =0;
        virtual void Clear() =0;

        virtual void DrawPixel(int x, int y) =0;
        virtual void DrawRectangle(int x, int y, int width, int height) =0;
        virtual void DrawDigit(int position, int digit) =0;
        virtual void DrawSmallText(int x, int y, std::string text) =0;
        virtual void DrawLargeText(int x, int y, std::string text) =0;

        virtual void Show() =0;
    };

    std::unique_ptr<Display> createDisplay();
}

#endif //DISPLAYSERVER_DISPLAY_H
