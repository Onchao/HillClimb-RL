#include "verticalLine.h"

VerticalLine::VerticalLine(View* v, unsigned int vao, GLuint i) :
	view(v), VAO(vao), image(i)
{
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	setCorners();
}

void VerticalLine::setCorners()
{
	float W = view->getW();	
	float H = view->getH();
	vertices[0] = (1) / (W / 2);
	vertices[9] = (1) / (W / 2);
	vertices[18] = (-1) / (W / 2);
	vertices[27] = (-1) / (W / 2);

	vertices[1] += (2000) / (H / 2);
	vertices[10] += (2000) / (H / 2);
	vertices[19] += (2000) / (H / 2);
	vertices[28] += (2000) / (H / 2);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);
}

void VerticalLine::drawImage()
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image);
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

glm::mat4 VerticalLine::getTransform(float x)
{
	//glm::radians(rotation)
	glm::mat4 trans = view->getTransform();
	trans = glm::translate(trans, glm::vec3(-1.0f + 2 * x / view->getW(), 0.0f, 0.0f));
	
	//trans = glm::scale(trans, glm::vec3(1.0 / view->getW(), 1.0 / view->getH(), 0.0));
	//trans = glm::rotate(trans, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::scale(trans, glm::vec3(view->getW() / 1.0, view->getH() / 1.0, 0.0));
	//trans = glm::scale(trans, glm::vec3((float)width / view->getW(), (float)height / view->getH(), 0.0));
	return trans;
}