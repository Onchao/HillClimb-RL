#include "view.h"

#include <iostream>

View::View(point p, GLFWwindow* platform) :
	pos(p), scale(1), window(platform) 
{
	updateWH();
	transform = glm::mat4(1.0f);
};

int View::getScale() { return scale; }
GLFWwindow* View::getWidow() { return window; }

void View::updateWH() 
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	W = width + (width % 2);
	H = height + (height % 2);
	glViewport(0, 0, W, H);
	//glViewport(0, 0, width, height); // ?
}
int View::getW() { return W; }
int View::getH() { return H; }

void View::setX(float x) { pos.x = x; }
void View::setY(float y) { pos.y = y; }
float View::getX() { return pos.x; }
float View::getY() { return pos.y; }

void View::setWidow(GLFWwindow* w) 
{ 
	window = w; 
}

void View::setFollowXY(point p) 
{
	if (cameraFollow)
	{
		p.x -= 400;
		p.y -= H / 2;
		pos = p;
		transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(-pos.x * 2 / W, -pos.y * 2 / H, 0.0f));
	}	
}

void View::movedXdY(float dx, float dy) 
{
	pos.x += dx;
	pos.y += dy;
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-pos.x * 2 / W, -pos.y * 2 / H, 0.0f));
}

glm::mat4& View::getTransform()
{ 
	return transform; 
}

void View::setCameraFollow(bool f)
{
	cameraFollow = f;
}

void View::switchCameraFollow()
{
	cameraFollow = 1 - cameraFollow;
}
