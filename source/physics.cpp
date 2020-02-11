#include "physics.h"

using std::cout;
using std::endl;

Physics::Physics(GameMap* gm, Controls* co) :
	gameMap(gm), controls(co)
{
}

void Physics::simulate(Car* car, float time, int iterations)
{
	for (int i = 0; i < iterations; i++)
	{
		gForce(car, time);
		collisions(car);
		engine(car, time);
		resistance(car, time);
		position(car, time);
	}
}

void Physics::gForce(Car* car, float time)
{
	car->setVel({ car->getVel().x, car->getVel().y - (float)time * g });
}

void Physics::collisions(Car* car)
{	
	int start, end;
	int last = -1;
	float dRotateStart = 0.01;
	float dRotate = dRotateStart;
	
	while(true)
	{
		bool tireTouchGround[2] = { false };
		point tireCenter[2] = { car->getTireCenter(0), car->getTireCenter(1) };
		getTireStatus(car, tireCenter, tireTouchGround);
		
		if (tireTouchGround[0] && tireTouchGround[1]) //both touch
		{
			car->reduceNormalToGroundVel(0.0f);
			car->moveY(0.1f);
			dRotate = dRotateStart;
			last = -1;
		}
		if ((!tireTouchGround[0]) && (!tireTouchGround[1])) // no collision
		{
			break;
		}
		//cout << tireTouchGround[0] << tireTouchGround[1] << endl;
		if (tireTouchGround[0])
		{
			if (last == 2)
			{
				dRotate = dRotate / 2;
				car->moveY(0.025f);
			}
			car->rotateCar(-dRotate);
			last = 1;
		}
		if (tireTouchGround[1])
		{
			if (last == 1)
			{
				dRotate = dRotate / 2;
				car->moveY(0.01f);
			}
			car->rotateCar(dRotate);
			last = 2;
		}			
		//cout << dRotate << endl;

	}
}

void Physics::getTireStatus(Car* car, point tireCenter[2], bool tireTouchGround[2])
{
	int start, end;
	for (int t = 0; t < 2; t++)
	{
		start = (int)tireCenter[t].x / gameMap->getIntervalSize() * gameMap->getSubdiv() - 2 * gameMap->getSubdiv();
		start = std::max(0, start);
		end = start + 4 * gameMap->getSubdiv();
		end = std::min(end, (int)gameMap->getColPoints().size() - 1);

		for (int i = start; i <= end; i++)
		{
			if (dist(tireCenter[t], gameMap->getColPoints()[i]) < car->getTireRadius())
			{
				tireTouchGround[t] = true;
				break;
			}
		}
	}
}

bool Physics::isFlying(Car* car)
{
	bool tireTouchGround[2] = { false };
	point tireCenter[2] = { car->getTireCenter(0), car->getTireCenter(1) };
	tireCenter[0].y -= 0.2f;
	tireCenter[1].y -= 0.2f;
	getTireStatus(car, tireCenter, tireTouchGround);	
	return (!tireTouchGround[0]) && (!tireTouchGround[1]);
}


void Physics::engine(Car* car, float time)
{
	int dir = 0;
	if (car->getAccelerateLeft() && !car->getAccelerateRight())
		dir = -1;
	if (!car->getAccelerateLeft() && car->getAccelerateRight())
		dir = 1;
	if (dir == 0)
		return;
	//car->setVel({ 10.0f * dir,0 });
	if (!isFlying(car))
	{
		car->setVel({
			car->getVel().x + cos(car->getRotation()) * car->getEngineAcc() * time * dir,
			car->getVel().y + sin(car->getRotation()) * car->getEngineAcc() * time * dir }
		);
	}	
}

void Physics::resistance(Car* car, float time)
{
	if (!isFlying(car))
	{
		point vel = car->getVel();
		point delta;
		delta.x = (-1 * sgn(vel.x)) * cos(car->getRotation()) * car->getEngineAcc() * time * car->getRollResistance();
		delta.y = (-1 * sgn(vel.x)) * sin(car->getRotation()) * car->getEngineAcc() * time * car->getRollResistance();
		point vel2 = { vel.x + delta.x, vel.y + delta.y };
		
		if (sgn(vel.x) != sgn(vel2.x)) vel.x = 0;
		else vel.x = vel2.x;

		if (sgn(vel.y) != sgn(vel2.y)) vel.y = 0;
		else vel.y = vel2.y;

		car->setVel({ vel.x, vel.y });
	}
}

void Physics::position(Car* car, float time)
{
	car->setPos({ car->getPos().x + car->getVel().x * time * 20, 
		car->getPos().y + car->getVel().y * time * 20 });
}
