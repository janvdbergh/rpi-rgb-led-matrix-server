#ifndef DISPLAYSERVER_LEDMATRIXDISPLAY_H
#define DISPLAYSERVER_LEDMATRIXDISPLAY_H

#include <led-matrix.h>
#include <graphics.h>
#include "Display.h"

namespace displays {

    class LedMatrixDisplay : public Display {
    public:
        LedMatrixDisplay() : _frameCanvas(nullptr), _color(255, 255, 255) {}

        bool Initialize(int argc, char **argv) override;

        void SetColor(uint8_t r, uint8_t g, uint8_t b) override;
        void Clear() override;

        void DrawPixel(int x, int y) override;
        void DrawRectangle(int x, int y, int width, int height) override;
        void DrawDigit(int position, int digit) override;
        void DrawSmallText(int x, int y, std::string text) override;
        void DrawLargeText(int x, int y, std::string text) override;

        void Show() override;

    private:
        std::unique_ptr<rgb_matrix::RGBMatrix> _rgbMatrix;
        rgb_matrix::FrameCanvas *_frameCanvas;

        rgb_matrix::Color _color;
        std::unique_ptr<rgb_matrix::Font> _smallFont;
        std::unique_ptr<rgb_matrix::Font> _largeFont;
    };
}

#endif //DISPLAYSERVER_LEDMATRIXDISPLAY_H
