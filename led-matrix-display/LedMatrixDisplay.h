#ifndef DISPLAYSERVER_LEDMATRIXDISPLAY_H
#define DISPLAYSERVER_LEDMATRIXDISPLAY_H

#include <led-matrix.h>
#include <graphics.h>

class LedMatrixDisplay {
public:
    LedMatrixDisplay();
    virtual ~LedMatrixDisplay();

    bool Initialize(int argc, char **argv);

    void SetColor(uint8_t r, uint8_t g, uint8_t b);

    void Clear();
    void DrawPixel(int x, int y);
    void DrawRectangle(int x, int y, int width, int height);
    void DrawDigit(int position, int digit);
    void DrawSmallText(int x, int y, std::string text);
    void DrawLargeText(int x, int y, std::string text);

    void Show();
private:
    rgb_matrix::RGBMatrix* _rgbMatrix;
    rgb_matrix::FrameCanvas *_frameCanvas;
    rgb_matrix::Color _color;
    rgb_matrix::Font* _smallFont;
    rgb_matrix::Font* _largeFont;
};

#endif //DISPLAYSERVER_LEDMATRIXDISPLAY_H
