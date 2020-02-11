#pragma once
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "view.h"
#include "textureLoader.h"

class VerticalLine {
private:
	float vertices[36] = {
		// positions         // colors                 // texture coords
		0.0f, 4.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // right top
		0.0f, -4.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,	  1.0f, 0.0f, // right bottom
		0.0f, -4.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // left bottom
		0.0f, 4.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,	  0.0f, 1.0f  // left top
	};                                             // l/r  top/bot
	unsigned int squareIndices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	int height = 100;
	int width = 100;
	unsigned int VAO, VBO, EBO;
	View* view;
	GLuint image;
	//glm::mat4 transform;

public:
	VerticalLine(View* view, unsigned int vao, GLuint image);
	void drawImage();
	void setCorners();
	glm::mat4 getTransform(float x);	
};