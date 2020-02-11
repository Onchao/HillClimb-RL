#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <cstdio>
#include <ft2build.h>
#include <string>
#include FT_FREETYPE_H

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include "view.h"
#include "controls.h"
#include "commandLine.h"
#include "shader.h";
#include "textureLoader.h"
#include "car.h"
#include "physics.h"
#include "verticalLine.h"

using namespace std;

Player* player;
View* view;
GameMap* gameMap;
TextureLoader* tl;
Controls* controls;
CommandLine* cLine;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	controls->c_mouse_button_callback(window, button, action, mods);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	controls->c_key_callback(window, key, scancode, action, mods);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	controls->c_framebuffer_size_callback(window, width, height);
}

unsigned int VAO;
unsigned int VAOtext;
unsigned int VBOtext;
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;

void RenderText(int shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAOtext);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBOtext);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6)* scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int threadOpenGL();
std::mutex mtx;	
char windowType = 'w';
int main()
{
	std::thread th(threadOpenGL);
	cLine->printHelp();
	while (true)
	{
		std::string tmp;
		std::getline(std::cin, tmp);
		mtx.lock();
		cLine->sendCommand(tmp);		
		mtx.unlock();
	}
	return 0;
}



int threadOpenGL()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Hill Climb Reinforcement Learning", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(1920, 1080, "WOW WOW WOW", glfwGetPrimaryMonitor(), NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glGenVertexArrays(1, &VAOtext);
	glBindVertexArray(VAOtext);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	tl = new TextureLoader();
	view = new View({ 0,0 }, window);
	
	std::vector<std::vector<std::vector<float>>> Qtable(50000, std::vector<std::vector<float>>(200, std::vector<float>(3, 0))); // [pos.x][vel.x][L O R]

	gameMap = new GameMap(view, tl, VAO);
	Car car(view, VAO, tl, gameMap->getStartPos());
	
	VerticalLine green(view, VAO, tl->getGreen());

	Shader basic_shader("../source/basic_shader.vs","../source/basic_shader.fs");

	glBindVertexArray(VAOtext);
	glGenBuffers(1, &VBOtext);
	glBindBuffer(GL_ARRAY_BUFFER, VBOtext);

	////////////////////////////////////////////////////////////////////			
	/*Shader text_shader("../source/text_shader.vs", "../source/text_shader.fs");
	text_shader.use();
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(view->getW()), 0.0f, static_cast<GLfloat>(view->getH()));
	glUniformMatrix4fv(glGetUniformLocation(text_shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "../fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 120);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);	// Configure VAO/VBO for texture quads
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
	///////////////////////////////////////////////////////////////
	
	Physics physics(gameMap, controls);
	controls = new Controls(window, view, gameMap, player, &car, tl, VAO);
	cLine = new CommandLine(gameMap, &car, controls, &physics, Qtable);


	int actions = 0;
	int perFrame = 8;
	physics.simulate(&car, 1.0f / 60 / 8, perFrame);

	while (!glfwWindowShouldClose(window))
	{
		cLine->countFps();

		mtx.lock();
		cLine->receiveCommand();
		mtx.unlock();

		controls->processInput(window);
		glfwPollEvents();
		view->updateWH();

		if (cLine->getMax() < car.getPos().x)
		{
			cLine->setMax(car.getPos().x);
		}

		if (cLine->getState() == 2)
		{
			for (int u = 0; u < controls->getAnimationSpeed(); u++)
			{				
				int simulateAction;
				int AIactionTime = cLine->getAIactionTime();
				actions++;
				if (actions >= AIactionTime)
				{
					actions = 0;
					Car carLeft(car);
					Car carOff(car);
					Car carRight(car);

					carLeft.setAccelerateLeft(1);
					physics.simulate(&carLeft, 1.0f / 60 / perFrame, perFrame * AIactionTime);
					carLeft.setAccelerateLeft(0);

					physics.simulate(&carOff, 1.0f / 60 / perFrame, perFrame * AIactionTime);

					carRight.setAccelerateRight(1);
					physics.simulate(&carRight, 1.0f / 60 / perFrame, perFrame * AIactionTime);
					carRight.setAccelerateRight(0);

					int a = (int)car.getPos().x / 40 + 100;
					int b = (int)(car.getVel().x + 100);
								

					float carLeftReward = carLeft.getPos().x;
					float carOffReward = carOff.getPos().x;
					float carRightReward = carRight.getPos().x;
					/*
					Qtable[a][b][0] = 0.0 * Qtable[a][b][0] + 1.0 *
						(carLeftReward + 0.99 * std::max(std::max(
							Qtable[(int)carLeft.getPos().x / 40 + 100][(int)(sgn(carLeft.getVel().x) + 100)][0],
							Qtable[(int)carLeft.getPos().x / 40 + 100][(int)(sgn(carLeft.getVel().x) + 100)][1]),
							Qtable[(int)carLeft.getPos().x / 40 + 100][(int)(sgn(carLeft.getVel().x) + 100)][2]));
					Qtable[a][b][1] = 0.0 * Qtable[a][b][1] + 1.0 *
						(carOffReward + 0.99 * std::max(std::max(
							Qtable[(int)carOff.getPos().x / 40 + 100][(int)(sgn(carOff.getVel().x) + 100)][0],
							Qtable[(int)carOff.getPos().x / 40 + 100][(int)(sgn(carOff.getVel().x) + 100)][1]),
							Qtable[(int)carOff.getPos().x / 40 + 100][(int)(sgn(carOff.getVel().x) + 100)][2]));
					Qtable[a][b][2] = 0.0 * Qtable[a][b][2] + 1.0 *
						(carRightReward + 0.99 * std::max(std::max(
							Qtable[(int)carRight.getPos().x / 40 + 100][(int)(sgn(carRight.getVel().x) + 100)][0],
							Qtable[(int)carRight.getPos().x / 40 + 100][(int)(sgn(carRight.getVel().x) + 100)][1]),
							Qtable[(int)carRight.getPos().x / 40 + 100][(int)(sgn(carRight.getVel().x) + 100)][2]));
					*/
					Qtable[a][b][0] = 0.0 * Qtable[a][b][0] + 1.0 *
						(carLeftReward + 0.99 * std::max(std::max(
							Qtable[(int)carLeft.getPos().x / 40 + 100][(int)(sgn(carLeft.getVel().x) * length(carLeft.getVel()) + 100)][0],
							Qtable[(int)carLeft.getPos().x / 40 + 100][(int)(sgn(carLeft.getVel().x) * length(carLeft.getVel()) + 100)][1]),
							Qtable[(int)carLeft.getPos().x / 40 + 100][(int)(sgn(carLeft.getVel().x) * length(carLeft.getVel()) + 100)][2]));
					Qtable[a][b][1] = 0.0 * Qtable[a][b][1] + 1.0 *
						(carOffReward + 0.99 * std::max(std::max(
							Qtable[(int)carOff.getPos().x / 40 + 100][(int)(sgn(carOff.getVel().x) * length(carOff.getVel()) + 100)][0],
							Qtable[(int)carOff.getPos().x / 40 + 100][(int)(sgn(carOff.getVel().x) * length(carOff.getVel()) + 100)][1]),
							Qtable[(int)carOff.getPos().x / 40 + 100][(int)(sgn(carOff.getVel().x) * length(carOff.getVel()) + 100)][2]));
					Qtable[a][b][2] = 0.0 * Qtable[a][b][2] + 1.0 *
						(carRightReward + 0.99 * std::max(std::max(
							Qtable[(int)carRight.getPos().x / 40 + 100][(int)(sgn(carRight.getVel().x) * length(carRight.getVel()) + 100)][0],
							Qtable[(int)carRight.getPos().x / 40 + 100][(int)(sgn(carRight.getVel().x) * length(carRight.getVel()) + 100)][1]),
							Qtable[(int)carRight.getPos().x / 40 + 100][(int)(sgn(carRight.getVel().x) * length(carRight.getVel()) + 100)][2]));
					
					//cout << "q "<< Qtable[a][b][2]<<endl;
					
					if (rand() % 100 < cLine->getRandom())
					{
						int t = rand() % 3;
						if (t == 0) simulateAction = -1;
						else if (t == 1) simulateAction = 0;
						else if (t == 2) simulateAction = 1;
					}
					else
					{
						if (Qtable[a][b][0] > Qtable[a][b][1] && Qtable[a][b][0] > Qtable[a][b][2]) simulateAction = -1;
						else if (Qtable[a][b][1] > Qtable[a][b][0] && Qtable[a][b][1] > Qtable[a][b][2]) simulateAction = 0;
						else if (Qtable[a][b][2] > Qtable[a][b][0] && Qtable[a][b][2] > Qtable[a][b][1]) simulateAction = 1;
					}
				}
				if (simulateAction == -1)
				{
					car.setAccelerateLeft(1);
					physics.simulate(&car, 1.0f / 60 / perFrame, perFrame);
					car.setAccelerateLeft(0);
				}
				else if(simulateAction == 0)
				{
					physics.simulate(&car, 1.0f / 60 / perFrame, perFrame);
				}
				else// 1
				{
					car.setAccelerateRight(1);
					physics.simulate(&car, 1.0f / 60 / perFrame, perFrame);
					car.setAccelerateRight(0);
				}				
			}			
		}
		else if(cLine->getState() == 1)
		{
			physics.simulate(&car, 1.0f / 60 / perFrame, perFrame);
		}
	
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.2f, 0.6f, 0.4f, 1.0f);
		glClearColor(0.2f, 0.6f, 1.f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindVertexArray(VAO);

		basic_shader.use();
		basic_shader.setMat4("transform", view->getTransform());
		gameMap->drawFragments();
				
		basic_shader.setMat4("transform", green.getTransform(cLine->getMax()));
		green.drawImage();

		basic_shader.setMat4("transform", car.getTransform());
		car.drawImage();

		//test.prepareImage();
		//test.drawImage();
			
		//static_shader.use();
		//intheirface->draw();
		

		//glBindVertexArray(VAOtext);
		//text_shader.use();
		//for(int xddd = 0; xddd < 10000; xddd++)
		//RenderText(text_shader.getID(), "xdxdxd", 200.0f, 100.0f, 1.0f, glm::vec3(0.2f, 0.12f, 0.0f));
		//RenderText(shader, "pee", 400.0f, 700.0f, 0.6f, glm::vec3(0.9f, 0.9f, 0.0f));

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	delete player;
	delete view;
	delete gameMap;
	delete tl;
	delete cLine;
	return 0;
}