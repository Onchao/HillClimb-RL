#pragma once
#include "car.h"
#include "gameMap.h"
#include "controls.h"
#include "utility.h"
#include <algorithm>

class Physics
{
private:
	GameMap* gameMap;
	Controls* controls;
	const float g = 9.81;

	void gForce(Car* car, float time);
	void collisions(Car* car);
	void engine(Car* car, float time);
	void resistance(Car* car, float time);
	void position(Car* car, float time);
	bool isFlying(Car* car);
	void getTireStatus(Car* car, point tireCenter[2], bool tireTouchGround[2]);

public:
	Physics(GameMap* g, Controls* controls);
	void simulate(Car* car, float time, int iterations);
};