#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "point.h"

class View
{	
	point pos;
	int scale;		
	GLFWwindow* window;
	int W;
	int H;	
	glm::mat4 transform;	
	bool cameraFollow = true;
public:		
	View(point p, GLFWwindow* platform);
	void updateWH();	
	float getX();
	float getY();
	int getScale();
	GLFWwindow* getWidow();
	int getW();
	int getH();
	void setX(float x);
	void setY(float y);
	void setFollowXY(point p);
	void setWidow(GLFWwindow* platform);
	void movedXdY(float dx, float dy);	
	glm::mat4& getTransform();
	void setCameraFollow(bool f);
	void switchCameraFollow();
};