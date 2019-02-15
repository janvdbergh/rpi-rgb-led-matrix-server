
#ifndef DISPLAYSERVER_LEDMATRIXDISPLAY_H
#define DISPLAYSERVER_LEDMATRIXDISPLAY_H


#include <led-matrix.h>
#include <server/AbstractDisplay.h>

class LedMatrixDisplay : public AbstractDisplay {
public:
	bool initialize(int argc, char **argv);

	void set_brightness(uint8_t brightness) override;

	void show(const LayerMap &layer_set) override;

private:
	std::unique_ptr<rgb_matrix::RGBMatrix> _rgbMatrix;
	rgb_matrix::FrameCanvas *_frameCanvas;
};


#endif //DISPLAYSERVER_LEDMATRIXDISPLAY_H
