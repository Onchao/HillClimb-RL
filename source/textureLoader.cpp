#include "textureLoader.h"


TextureLoader::TextureLoader()
{
	loadTextureFromFile(terrain, "../graphics/terrain4.png");
	loadTextureFromFile(car, "../graphics/car4.png");
	loadTextureFromFile(green, "../graphics/green.png");
	loadTextureFromFile(yellow, "../graphics/yellow.png");

}

void TextureLoader::loadTextureFromFile(GLuint& mem, const char* path)
{
	int width, height, nrChannels;
	unsigned char* data;
	
	glGenTextures(1, &mem);

	glBindTexture(GL_TEXTURE_2D, mem);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set texture filtering parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture:  " << path << std::endl;
	}
	stbi_image_free(data);
}

GLuint TextureLoader::getTerrain()
{
	return terrain;
}

GLuint TextureLoader::getCar()
{
	return car;
}

GLuint TextureLoader::getGreen()
{
	return green;
}

GLuint TextureLoader::getYellow()
{
	return yellow;
}
