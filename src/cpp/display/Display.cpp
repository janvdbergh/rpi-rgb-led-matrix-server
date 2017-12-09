#include "Display.h"

void Display::DefineImage(const std::string &name, const ImagePtr &image) {
    _images[name] = image;
}

void Display::DefineAnimation(const std::string &name, const CommandPtr &command) {
    _animations[name] = command;
}

void Display::Sleep(uint16_t millis) {
    usleep((uint32_t)1000 * millis);
}

void Display::ShowAnimation(const std::string & name) {
    CommandPtr command = GetAnimation(name);
    if (command) {
        this->ExecuteCommand(command);
    }
}
