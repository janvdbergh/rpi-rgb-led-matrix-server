#include <server/ImageMap.h>
#include <server/DisplayError.h>

void ImageMap::add_image(const std::string &name, ImagePtr image) {
	if (_images.find(name) != _images.end()) {
		throw DisplayError(IMAGE_ALREADY_EXISTS, "Image already exists");
	}

	_images.insert(std::pair<std::string, const ImagePtr &>(name, image));
}

ImagePtr ImageMap::get_image(const std::string &name) {
	auto it = _images.find(name);
	if (it == _images.end()) {
		throw DisplayError(LAYER_NOT_FOUND, "Layer not found");
	}

	return it->second;
}
