#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include "gameMap.h"
#include "controls.h"
#include "physics.h"

class CommandLine
{
	bool newCommand;
	std::string command;

	int random = 15;
	int AIactionTime = 20;
	float max;

	bool printFps;
	int fpsCount;
	std::clock_t start;
	int state = 0;

	GameMap* gameMap;
	Controls* controls;
	Car* car;
	Physics* physics;
	void prompt();
	std::vector<std::vector<std::vector<float>>>& Qtable;
public:
	CommandLine(GameMap* m, Car* c, Controls* controls, Physics* p, std::vector<std::vector<std::vector<float>>>& q);
	void sendCommand(std::string c);
	void receiveCommand();
	
	void printHelp();
	void countFps();
	int getRandom();
	int getAIactionTime();
	float getMax();
	void setMax(float m);
	int getState();
};