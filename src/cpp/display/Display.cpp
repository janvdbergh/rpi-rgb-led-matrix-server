#include "Display.h"

void Display::DefineImage(std::string name, boost::shared_ptr<Image> image) {
    _images[name] = image;
}