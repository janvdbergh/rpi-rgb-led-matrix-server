#include "LayerMap.h"
#include "DisplayError.h"

void LayerMap::add_layer(uint16_t index, const LayerPtr &layer) {
	if (_layers.find(index) != _layers.end()) {
		throw DisplayError(LAYER_ALREADY_EXISTS, "Layer already exists");
	}

	_layers.insert(std::pair<uint16_t, const LayerPtr &>(index, layer));
}

void LayerMap::select_layer(uint16_t index) {
	auto it = _layers.find(index);
	if (it == _layers.end()) {
		throw DisplayError(LAYER_NOT_FOUND, "Layer not found");
	}

	_selected_layer = it->second;
}

inline uint8_t calculate_using_alpha(uint8_t current, uint8_t to_update, uint8_t alpha) {
	return static_cast<uint8_t>(((uint16_t)current * (255 - alpha) + (uint16_t) to_update * alpha) >> 8);
}

inline void update(Pixel current, const Pixel newValue, uint8_t layer_alpha) {
	auto total_alpha = static_cast<uint8_t>((uint16_t)layer_alpha * newValue.get_alpha() >> 8);

	current.set_r(calculate_using_alpha(current.get_r(), newValue.get_r(), total_alpha));
	current.set_g(calculate_using_alpha(current.get_g(), newValue.get_g(), total_alpha));
	current.set_b(calculate_using_alpha(current.get_b(), newValue.get_b(), total_alpha));
}

Pixel LayerMap::get_pixel(Point location) const {
	Pixel current;
	for (const auto &it : _layers) {
		update(current, it.second->get_pixel(location), it.second->get_alpha());
	}
	return current;
}
