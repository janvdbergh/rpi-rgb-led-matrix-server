#ifndef DISPLAYSERVER_POINT_H
#define DISPLAYSERVER_POINT_H

class Point {
public:
	Point(int16_t _x, int16_t _y) : _x(_x), _y(_y) {}

	int16_t get_x() const {
		return _x;
	}

	int16_t get_y() const {
		return _y;
	}

private:
	int16_t _x, _y;
};

#endif //DISPLAYSERVER_POINT_H
