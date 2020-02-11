#include "car.h"


Car::Car(View* v, unsigned int vao, TextureLoader* t, point p) : 
	view(v), VAO(vao), tl(t), pos(p)
{
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	setCorners();
}

Car::Car(Car* c)
{
	for (int i = 0; i < 36; i++)
		vertices[i] = c->vertices[i];
	VAO = c->VAO;
	VBO = c->VAO;
	EBO = c->EBO;
	view = c->view;
	tl = c->tl;
	rotation = c->rotation;
	pos = c->pos;
	vel = c->vel;
}

void Car::setCorners()
{
	//float W = view->getW();
	//float H = view->getH();
	//
	//float px = (pos.x - 50) / (W / 2) - 1;//-50
	//float py = (pos.y - 25) / (H / 2) - 1;//-25
	//float w = 2 * width / W;
	//float h = 2 * height / H;
	//vertices[0] = px + w;
	//vertices[1] = py + h;

	//vertices[9] = px + w;
	//vertices[10] = py;

	//vertices[18] = px;
	//vertices[19] = py;

	//vertices[27] = px;
	//vertices[28] = py + h;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);
}

void Car::drawImage()
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tl->getCar());
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

glm::mat4 Car::getTransform()
{
	//glm::radians(rotation)
	glm::mat4 trans = view->getTransform();
	trans = glm::translate(trans, glm::vec3(-1.0f + 2*pos.x/view->getW(), -1.0f + 2*pos.y/view->getH(), 0.0f));
	trans = glm::scale(trans, glm::vec3(1.0 / view->getW(), 1.0 / view->getH(), 0.0));
	trans = glm::rotate(trans, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(view->getW() / 1.0, view->getH() / 1.0, 0.0));
	trans = glm::scale(trans, glm::vec3((float)width / view->getW(), (float)height / view->getH(), 0.0));
	return trans;
}

void Car::reduceNormalToGroundVel(float percent)
{
	float dx = getTireCenter(1).x - getTireCenter(0).x;
	float dy = getTireCenter(1).y - getTireCenter(0).y;

	float alpha = atan(dy / dx);
	float hor = vel.y * sin(alpha) + vel.x * cos(alpha);
	float ver = vel.y * cos(alpha) + vel.x * sin(alpha);
	//std::cout << alpha << " "<< hor<< " "<<ver<< std::endl;
	ver *= percent;

	vel.x = hor * cos(alpha) + ver * sin(alpha);
	vel.y = hor * sin(alpha) + ver * cos(alpha);
}

point Car::getTireCenter(int i)
{
	float x = cos(rotation) * tireCenter[i].x - sin(rotation) * tireCenter[i].y;
	float y = sin(rotation) * tireCenter[i].x + cos(rotation) * tireCenter[i].y;
	return { pos.x + x, pos.y + y };
}

point Car::getPos()
{
	return pos;
}

void Car::setPos(point t)
{
	view->setFollowXY(t);

	pos.x = t.x;
	pos.y = t.y;
}

point Car::getVel()
{
	return vel;
}

void Car::setVel(point t)
{
	vel.x = t.x;
	vel.y = t.y;
}

float Car::getTireRadius()
{
	return tireRadius;
}

void Car::rotateCar(float angle)
{
	rotation += angle;
}

void Car::moveY(float t)
{
	pos.y += t;
}

float Car::getEngineAcc()
{
	return engineAcc;
}

float Car::getRotation()
{
	return rotation;
}

float Car::getRollResistance()
{
	return rollResistance;
}

void Car::setAccelerateLeft(bool t)
{
	accelerateLeft = t;
}

void Car::setAccelerateRight(bool t)
{
	accelerateRight = t;
}

bool Car::getAccelerateLeft()
{
	return accelerateLeft;
}

bool Car::getAccelerateRight()
{
	return accelerateRight;
}

void Car::setRotate(int a)
{
	rotation = 0;
}

void Car::reset()
{
	vel = { 0,0 };
	rotation = 0;
}
