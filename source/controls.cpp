#include "controls.h"


Controls::Controls(GLFWwindow* window, View* v, GameMap* m, Player* p, Car* c, TextureLoader* t, unsigned int vao) :
	view(v), gameMap(m), player(p), car(c), tl(t), VAO(vao)
{
	glfwGetCursorPos(window, &prev_xpos, &prev_ypos);
	mode = gameMode::play;
}

void Controls::c_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos;
		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);		
	}
}

void Controls::c_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (mode == gameMode::play)
	{
		if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS)
		{
			car->setAccelerateLeft(true);
		}
		if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_RELEASE)
		{
			car->setAccelerateLeft(false);
		}

		if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS)
		{
			car->setAccelerateRight(true);
		}
		if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_RELEASE)
		{
			car->setAccelerateRight(false);
		}				

		if ((key == GLFW_KEY_KP_ADD) && action == GLFW_PRESS)
		{
			if (animationSpeed < 65536)
				animationSpeed *= 2;
			std::cout<<"animation speed: " << animationSpeed << "\n > ";
		}
		if ((key == GLFW_KEY_KP_SUBTRACT) && action == GLFW_PRESS)
		{
			if (animationSpeed > 1)
				animationSpeed /= 2;
			std::cout << "animation speed: " << animationSpeed << "\n > ";
		}

		if (key == GLFW_KEY_C && action == GLFW_PRESS)
		{
			view->switchCameraFollow();
		}
	}
}

int Controls::getAnimationSpeed()
{
	return animationSpeed;
}

void Controls::processInput(GLFWwindow* window)
{	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	double xpos;
	double ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS)
	{
		view->setCameraFollow(false);
		double xDif = prev_xpos - xpos;
		double yDif = prev_ypos - ypos;
		double dx = xDif;
		double dy = -yDif;

		view->movedXdY(dx, dy);		
	}
	prev_xpos = xpos;
	prev_ypos = ypos;
}

void Controls::c_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	view->updateWH();
}

bool Controls::inEditor() 
{
	return gameMode::map_editor == mode; 
}

bool Controls::inPlayMode()
{
	return mode == gameMode::play;
}

