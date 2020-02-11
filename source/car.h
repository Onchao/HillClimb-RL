#pragma once
#include "point.h"
#include "view.h"
#include "textureLoader.h"

class Car
{
private:
	// image
	float vertices[36] = {
		// positions         // colors                 // texture coords
		1.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // right top
		1.0f, -1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,	  1.0f, 0.0f, // right bottom
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // left bottom
		-1.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,	  0.0f, 1.0f  // left top
	};                                             // l/r  top/bot
	unsigned int squareIndices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	int height = 50;
	int width = 100;
	unsigned int VAO, VBO, EBO;
	View* view;
	TextureLoader* tl;	

	//physics
	double mass = 1000; // kg
	point tireCenter[2] = { {-30, -10},{30,-10} }; // px
	float tireRadius = 15; // px
	float rotation = 0; // radian
	float engineAcc = 4; // m/s^2 

	point pos; //px   1m = 20px
	double rollResistance = 0.1; // tire on asphalt 0.01
	double friction = 0.8;
	point vel = { 0, 0 }; // m/s

	bool accelerateLeft = false;
	bool accelerateRight = false;

public:
	Car(View* v, unsigned int vao, TextureLoader* t, point p);
	Car(Car* c);
	void setCorners();
	void drawImage();
	glm::mat4 getTransform();

	void reduceNormalToGroundVel(float percent);

	point getTireCenter(int i);
	point getPos();
	void setPos(point t);
	point getVel();
	void setVel(point t);
	float getTireRadius();
	void rotateCar(float angle);
	void moveY(float t);
	float getEngineAcc();
	float getRotation();
	float getRollResistance();
	void setAccelerateLeft(bool t);
	void setAccelerateRight(bool t);
	bool getAccelerateLeft();
	bool getAccelerateRight();
	void setRotate(int a);
	void reset();
};