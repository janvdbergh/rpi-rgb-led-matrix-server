#include "Display.h"

void Display::DefineImage(const std::string &name, boost::shared_ptr<const Image> image) {
    _images[name] = image;
}
