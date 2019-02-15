#include <server/ExecutionContext.h>

ExecutionContext::ExecutionContext(AbstractDisplay &_display) : _display(_display) {}

void ExecutionContext::reset() {
	_layer_map.clear();
}

void ExecutionContext::show() {
	_display.show(_layer_map);
}

void ExecutionContext::set_brighness(uint8_t brightness) {
	_display.set_brightness(brightness);
}

void ExecutionContext::add_layer(uint8_t index, const LayerPtr layer) {
	_layer_map.add_layer(index, layer);
}

void ExecutionContext::select_layer(uint8_t index) {
	return _layer_map.select_layer(index);
}

LayerPtr ExecutionContext::selected_layer() {
	return _layer_map.get_selected_layer();
}

void ExecutionContext::define_image(const std::string &name, const ImagePtr &image) {
	_image_map.add_image(name, image);
}

void ExecutionContext::draw_image(Point location, const std::string &name) {
	const ImagePtr &image = _image_map.get_image(name);
	_layer_map.get_selected_layer()->draw_image(location, image);
}
