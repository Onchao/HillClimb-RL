#pragma once
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>


class TextureLoader
{
private:
	GLuint terrain;
	GLuint car;
	GLuint green;
	GLuint yellow;
public:
	TextureLoader();

	void loadTextureFromFile(GLuint& mem, const char* path);
	GLuint getTerrain();
	GLuint getCar();
	GLuint getGreen();
	GLuint getYellow();
};
