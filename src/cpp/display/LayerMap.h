#ifndef DISPLAYSERVER_LAYERSET_H
#define DISPLAYSERVER_LAYERSET_H

#include <map>
#include "Layer.h"

class LayerMap {
public:
	void clear() { _layers.clear(); }

	void add_layer(uint16_t index, const LayerPtr &layer);

	void select_layer(uint16_t index);

	LayerPtr get_selected_layer() const {
		return _selected_layer;
	}

	Pixel get_pixel(Point location) const;

private:
	std::map<uint16_t, const LayerPtr> _layers;
	LayerPtr _selected_layer;
};


#endif //DISPLAYSERVER_LAYERSET_H
