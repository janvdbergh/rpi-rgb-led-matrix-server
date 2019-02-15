#ifndef DISPLAYSERVER_EXECUTION_CONTEXT_H
#define DISPLAYSERVER_EXECUTION_CONTEXT_H


#include <server/LayerMap.h>
#include <server/AbstractDisplay.h>
#include <server/ImageMap.h>

class ExecutionContext {
public:
	ExecutionContext(AbstractDisplay &_display);

	void show();

	void reset();

	void set_brighness(uint8_t brightness);

	void add_layer(uint8_t index, LayerPtr layer);

	void select_layer(uint8_t index);

	void define_image(const std::string &name, const ImagePtr &image);

	void draw_image(Point location, const std::string& name);

	LayerPtr selected_layer();

private:
	LayerMap _layer_map;
	ImageMap _image_map;
	AbstractDisplay &_display;
};

typedef std::shared_ptr<ExecutionContext> ContextPtr;

#endif //DISPLAYSERVER_EXECUTION_CONTEXT_H
