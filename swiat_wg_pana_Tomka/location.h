#ifndef LOCATION_H
#define LOCATION_H
#include <cstdlib>
class Location {
public:
	bool operator==(const Location& location) {
		return location.x == this->x && location.y == this->y;
	}
	bool operator!=(const Location& location) {
		return location.x != this->x || location.y != this->y;
	}
	Location operator+(const Location& location) {
		return { location.x + this->x,location.y + this->y };
	}
	Location(int x, int y) : x(x), y(y) {
	};
	int x, y;
	static Location getRandomLocation(int width, int height) {
		return Location(rand() % width, rand() % height);
	};
};

#endif
