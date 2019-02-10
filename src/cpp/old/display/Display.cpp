#include "Display.h"
#include "DisplayError.h"

void Display::Reset() {
	_images.clear();
	_animations.clear();
	Clear();
	Show();
}

void Display::DefineImage(const std::string &name, const ImagePtr &image) {
	_images[name] = image;
}

const ImagePtr Display::GetImage(const std::string &name) const {
	auto iterator = _images.find(name);
	if (iterator == _images.end()) {
		boost::throw_exception(DisplayError(UNKNOWN_IMAGE, "Unknown image"));
	}

	return iterator->second;
}

void Display::DefineAnimation(const std::string &name, const OldCommandPtr &command) {
	_animations[name] = command;
}

const OldCommandPtr Display::GetAnimation(const std::string &name) const {
	auto iterator = _animations.find(name);
	if (iterator == _animations.end()) {
		boost::throw_exception(DisplayError(UNKNOWN_ANIMATION, "Unknown animation"));
	}

	return iterator->second;
}

void Display::Sleep(uint16_t millis) {
	usleep((uint32_t) 1000 * millis);
}

void Display::ShowAnimation(const std::string &name) {
	OldCommandPtr command = GetAnimation(name);
	this->ExecuteCommand(command);
}
