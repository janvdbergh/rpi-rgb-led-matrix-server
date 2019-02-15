#ifndef DISPLAYSERVER_ABSTRACTDISPLAY_H
#define DISPLAYSERVER_ABSTRACTDISPLAY_H


#include <server/LayerMap.h>

class AbstractDisplay {
public:
	virtual void set_brightness(uint8_t brightness) = 0;

	virtual void show(const LayerMap &layer_set) = 0;
};


#endif //DISPLAYSERVER_ABSTRACTDISPLAY_H
