#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "view.h"
#include "gameMap.h"
#include "player.h"
#include "car.h"

class Controls
{
	enum class gameMode {
		play,
		map_editor
	};
	gameMode mode;

	double prev_xpos;
	double prev_ypos;

	Player* player;
	View* view;
	GameMap* gameMap;
	Car* car;

	unsigned int VAO;
	TextureLoader* tl;

	int animationSpeed = 1;
	
public:	
	Controls(GLFWwindow* window, View* view, GameMap* gameMap, Player* player, Car* c, TextureLoader* t, unsigned int vao);
	void c_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void c_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void c_framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
	bool inEditor();
	bool inPlayMode();	
	int getAnimationSpeed();
};