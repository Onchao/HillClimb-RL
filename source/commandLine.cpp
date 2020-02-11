#include "commandLine.h"

CommandLine::CommandLine(GameMap* m, Car* ca, Controls* co, Physics* p, std::vector<std::vector<std::vector<float>>>& q) :
	newCommand(false), printFps(false), fpsCount(0), Qtable(q), physics(p),
	gameMap(m), car(ca), controls(co)
{	
	start = std::clock();

	prompt();
}

void CommandLine::countFps()
{
	fpsCount++;
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	if (duration > 1)
	{		
		if(printFps)
			std::cout << "FPS: " << fpsCount << "\n > ";
		start = std::clock();
		fpsCount = 0;
	}
}

void CommandLine::sendCommand(std::string c)
{
	newCommand = true;
	command = c;
	//std::cout << "main: " << command << std::endl;
}

int CommandLine::getRandom()
{
	return random;
}

float CommandLine::getMax()
{
	return max;
}
void CommandLine::setMax(float m)
{
	max = m;
}

int CommandLine::getAIactionTime()
{
	return AIactionTime;
}

int CommandLine::getState()
{
	return state;
}

void CommandLine::receiveCommand()
{
	if (newCommand)
	{
		newCommand = false;
		//std::cout << "opengl: " << command << std::endl;
		
		std::string buf;
		std::stringstream ss(command);
		std::vector<std::string> tokens;

		while (ss >> buf)
			tokens.push_back(buf);

		if (tokens[0] == "clear")
		{
			system("cls");
		}
		else if(tokens[0] == "fps")
		{
			printFps = 1 - printFps;
		}
		else if(tokens[0] == "help")
		{ 
			printHelp();
		}
		else if (tokens[0] == "set" && tokens[1] == "random")
		{
			random = stoi(tokens[2]);
		}
		else if (tokens[0] == "set" && tokens[1] == "aiactiontime")
		{
			AIactionTime = stoi(tokens[2]);
		}
		else if (tokens[0] == "reset" && tokens[1] == "max")
		{
			max = car->getPos().x;
		}
		else if (tokens[0] == "stop")
		{
			state = 0;
		}		
		else if (tokens[0] == "start" && tokens[1] == "player")
		{
			state = 1;
		}
		else if (tokens[0] == "start" && tokens[1] == "ai")
		{
			state = 2;
		}
		else if (tokens[0] == "generate")
		{
			if(tokens[1] == "random")
				gameMap->generateMap(0);
			else if(tokens[1] == "gauss")
				gameMap->generateMap(1);
			else
				std::cout << "invalid command\n > " << std::endl;

			car->setPos(gameMap->getStartPos());
			car->reset();
			max = car->getPos().x;
			std::vector<std::vector<std::vector<float>>> tmp(50000, std::vector<std::vector<float>>(200, std::vector<float>(3, 0)));
			Qtable = tmp;
			physics->simulate(car, 1.0f / 60 / 8, 8);
		}
		else if (tokens[0] == "reset" && tokens[1] == "car")
		{
			car->setPos(gameMap->getStartPos());
			car->reset();
			max = car->getPos().x;
			physics->simulate(car, 1.0f / 60 / 8, 8);
		}
		else
		{
			std::cout << "invalid command\n > " << std::endl;
		}
		prompt();
	}
}

void CommandLine::prompt()
{
	std::cout << " > ";
}

void CommandLine::printHelp()
{
}

