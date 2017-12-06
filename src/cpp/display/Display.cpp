#include "Display.h"

void Display::DefineImage(const std::string &name, const boost::shared_ptr<const Image> &image) {
    _images[name] = image;
}

void Display::DefineAnimation(const std::string &name, const std::vector<boost::shared_ptr<const Command>> &commands) {
    _commands[name] = commands;
}
