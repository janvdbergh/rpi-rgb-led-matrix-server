#include "Display.h"

void Display::DefineImage(const std::string &name, const ImagePtr &image) {
	_images[name] = image;
}

const ImagePtr Display::GetImage(const std::string &name) const {
	auto iterator = _images.find(name);
	if (iterator != _images.end()) {
		return iterator->second;
	}

	return nullptr;
}

void Display::DefineAnimation(const std::string &name, const CommandPtr &command) {
	_animations[name] = command;
}

const CommandPtr Display::GetAnimation(const std::string &name) const {
	auto iterator = _animations.find(name);
	if (iterator != _animations.end()) {
		return iterator->second;
	}

	return nullptr;
}

void Display::Sleep(uint16_t millis) {
	usleep((uint32_t) 1000 * millis);
}

void Display::ShowAnimation(const std::string &name) {
	CommandPtr command = GetAnimation(name);
	if (command) {
		this->ExecuteCommand(command);
	}
}
