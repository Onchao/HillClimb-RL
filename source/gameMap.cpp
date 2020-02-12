#include "gameMap.h"
#include "utility.h"

using std::cout;
using std::endl;
using std::string;



GameMap::GameMap(View* v, TextureLoader* t, unsigned int vao) :
	view(v), tl(t),	VAO(vao)
{	
	generateMap(1);
}

void GameMap::generateMap(int type)
{
	gameMap.clear();
	points.clear();
	gridPoints.clear();
	collisionPoints.clear();
	fragments.clear();


	if (type == 1)
	{
		numOfIntervals = 600;

		srand(time(NULL));
		for (int i = 0; i < numOfIntervals; i++)
			points.push_back({ (float)intervalSize * i, 0 });
		for (int i = 50 + 1; i < numOfIntervals - 20 + 1; i++)
			points[i].y = 0.90 * points[i - 1].y + (rand() % 151 - 75);
		for (int i = numOfIntervals - 25 + 1; i < numOfIntervals; i++)
			points[i].y = points[numOfIntervals - 25].y;

		startPos = { 140, 2000 };
		avarage(5, 30);
	}
	else
	{
		numOfIntervals = 140;

		// a*e^(-(x-b)^2/(2c^2))
		float a = -36;
		float e = 2.71828;
		float b = numOfIntervals / 2;
		float c = 16;
		for (int i = 0; i < numOfIntervals; i++)
		{
			float y = a * pow(e, -(i - b) * (i - b) / (2 * c * c));
			points.push_back({ (float)intervalSize * i, (float)intervalSize * y });
		}
		startPos = { (float)intervalSize * numOfIntervals / 2, 2000 + 100 + (float)intervalSize * a };
	}
	for (int i = 10; i >= 0; i--)
	{
		points[10 - i].y += pow(i, 2.7);
	}
	for (int i = 0; i <= 10; i++)
	{
		points[numOfIntervals - 11 + i].y += pow(i, 2.7);
	}

	for (int i = 0; i < numOfIntervals; i++)
		points[i].y += 2000;


	point p0 = points[0];
	gridPoints.push_back(p0);
	for (int i = 1; i < points.size(); i++)
	{
		while (1)
		{
			float next100;
			float nextX;
			if (p0.y < points[i].y)
			{
				next100 = ((int)p0.y / 100 + 1) * 100;
				if (next100 < points[i].y && next100 > p0.y)
				{
					nextX = ((next100 - p0.y) / (points[i].y - p0.y)) * (points[i].x - p0.x) + p0.x;
					p0 = { nextX, next100 };
					gridPoints.push_back(p0);
				}
				else
				{
					p0 = { points[i].x, points[i].y };
					gridPoints.push_back(p0);
					break;
				}
			}
			else if (p0.y > points[i].y)
			{
				next100 = (((int)p0.y - 1) / 100) * 100;
				if (next100 > points[i].y&& next100 < p0.y)
				{
					nextX = ((next100 - p0.y) / (points[i].y - p0.y)) * (points[i].x - p0.x) + p0.x;
					p0 = { nextX, next100 };
					gridPoints.push_back(p0);
				}
				else
				{
					p0 = { points[i].x, points[i].y };
					gridPoints.push_back(p0);
					break;
				}
			}
			else
			{
				p0 = { points[i].x, points[i].y };
				gridPoints.push_back(p0);
				break;
			}
		}
	}

	for (int i = 0; i < gridPoints.size(); i++)
	{
		//cout << "(" << gridPoints[i].x << "," << gridPoints[i].y << ")" << endl;
	}

	for (int i = 0; i < gridPoints.size() - 1; i++)
	{
		float X = (int)gridPoints[i].x / 100 * 100;
		float x1 = gridPoints[i].x - X;
		float x2 = gridPoints[i + 1].x - X;
		float Y = (int)std::min(gridPoints[i].y, gridPoints[i + 1].y) / 100 * 100;
		float y1 = gridPoints[i].y - Y;
		float y2 = gridPoints[i + 1].y - Y;

		fragments.push_back(MapFragment(view, VAO, tl, X, Y, x1, x2, y1, y2));
		for (int k = 0; k < 8; k++)
		{
			fragments.push_back(MapFragment(view, VAO, tl, X, Y - 100, x1, x2, 100, 100));
			Y -= 100;
		}
	}

	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = 0; j < subdiv; j++)
		{
			point tmp;
			tmp.x = points[i].x * (subdiv - j) / subdiv + points[i + 1].x * j / subdiv;
			tmp.y = points[i].y * (subdiv - j) / subdiv + points[i + 1].y * j / subdiv;
			collisionPoints.push_back(tmp);
		}
	}
	for (int i = 0; i < collisionPoints.size(); i++)
	{
		//std::cout << collisionPoints[i].x << " --- " << collisionPoints[i].y << std::endl;
	}
}

void GameMap::avarage(int dist, int iterations)
{
	//std::vector<float> points2(points.size());
	//for (int z = 0; z < iterations; z++)
	//{
	//	for (int i = dist/2 + 1; i < numOfIntervals - dist/2; i++)
	//	{
	//		float avg = 0;
	//		for (int j = -dist / 2; j < dist / 2; j++)
	//		{
	//			avg += points[i + j].y;
	//		}
	//		avg /= dist;

	//		points2[i] = 0.8 * points[i].y + 0.2 * avg;			
	//	}
	//	for (int i = 0; i < dist / 2 + 1; i++)
	//		points2[i] = points2[dist / 2 + 1];
	//	for (int i = numOfIntervals - dist / 2; i < numOfIntervals; i++)
	//		points2[i] = points2[numOfIntervals - dist / 2 - 1];

	//	for (int i = 0; i < numOfIntervals; i++)
	//		points[i].y = points2[i];				
	//}

	std::vector<float> points2(points.size());
	for(int z=0; z<iterations; z++)
	{ 
		for (int i = 0; i < numOfIntervals - dist; i++)
		{
			float avg = 0;
			for (int j = 0; j < dist; j++)
			{
				avg += points[i + j].y;
			}
			avg /= dist;

			points2[i] = 0.8 * points[i].y + 0.2 * avg;	
		}
		for (int i = numOfIntervals - dist; i < numOfIntervals; i++)
			points2[i] = points2[numOfIntervals - dist - 1];


		for (int i = 0; i < numOfIntervals; i++)
			points[i].y = points2[i];
	}	
}

void GameMap::drawFragments()
{
	for (int i = 0; i < fragments.size(); i++)
	{
		fragments[i].drawImage();
	}
}

std::vector<point>& GameMap::getColPoints()
{
	return collisionPoints;
}

int GameMap::getSubdiv()
{
	return subdiv;
}

int GameMap::getIntervalSize()
{
	return intervalSize;
}

point GameMap::getStartPos()
{
	return startPos;
}
