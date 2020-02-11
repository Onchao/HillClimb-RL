#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

#include "textureLoader.h"
#include "view.h"
#include "mapFragment.h"
#include "point.h"

class GameMap
{
	View* view;
	TextureLoader* tl;
	std::vector<std::vector<MapFragment>> gameMap;
	unsigned int VAO;

	std::vector<point> points;
	std::vector<point> gridPoints;
	std::vector<point> collisionPoints;
	int subdiv = 5;
	int intervalSize = 10;
	int numOfIntervals;
	std::vector<MapFragment> fragments;

	point startPos;
	double kFriction; // kinetic friction

public:
	GameMap(View* v, TextureLoader* t, unsigned int vao);
	void avarage(int dist, int iterations);
	void drawFragments();
	std::vector<point>& getColPoints();
	int getSubdiv();
	int getIntervalSize();
	void generateMap(int type);
	point getStartPos();
};