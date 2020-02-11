#pragma once
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "view.h"
#include "textureLoader.h"

class MapFragment {
private:
	float vertices[36] = {
		// positions         // colors                 // texture coords
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // right top
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,	  1.0f, 0.0f, // right bottom
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // left bottom
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 0.0f,	  0.0f, 1.0f  // left top
	};                                             // l/r  top/bot
	unsigned int squareIndices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	int height = 100;
	int width = 100;
	unsigned int VAO, VBO, EBO;
	View* view;
	TextureLoader* tl;
	glm::mat4 transform;

public:
	MapFragment(View* view, unsigned int vao, TextureLoader* tl, float X, float Y, float x1, float x2, float y1, float y2);
	void drawImage();

	void setCorners(float X, float Y, float x1, float x2, float y1, float y2);
};