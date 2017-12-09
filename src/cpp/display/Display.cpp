#include "Display.h"

void Display::DefineImage(const std::string &name, const boost::shared_ptr<const Image> &image) {
    _images[name] = image;
}

void Display::DefineAnimation(const std::string &name, const boost::shared_ptr<const Command> &command) {
    _command[name] = command;
}
