#ifndef DISPLAYSERVER_IMAGESET_H
#define DISPLAYSERVER_IMAGESET_H

#include <map>
#include "../datatype/Image.h"

class ImageMap {
public:
	void clear() { _images.clear(); }

	void add_image(const std::string &name, ImagePtr image);

	ImagePtr get_image(const std::string& name);

private:
	std::map<std::string, const ImagePtr> _images;
};


#endif //DISPLAYSERVER_IMAGESET_H
