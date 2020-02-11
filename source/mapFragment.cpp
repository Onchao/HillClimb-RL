#include "mapFragment.h"

MapFragment::MapFragment(View* v, unsigned int vao, TextureLoader* t, float X, float Y, float x1, float x2, float y1, float y2) :
	view(v), VAO(vao), tl(t)
{
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	setCorners(X, Y, x1, x2, y1, y2);
}

/*
            y2
         /| 
        / |
       /  | y1
      |   |
      |   |
X Y   x1 x2

*/

void MapFragment::setCorners(float X, float Y, float x1, float x2, float y1, float y2)
{
	float W = view->getW();
	float H = view->getH();

	X = X / (W / 2) - 1;
	Y = Y / (H / 2) - 1;

	float x1p = x1 / (W / 2);
	float x2p = x2 / (W / 2);
	float y1p = y1 / (H / 2);
	float y2p = y2 / (H / 2);

	vertices[0] = X + x2p;
	vertices[1] = Y + y2p;

	vertices[9] = X + x2p;
	vertices[10] = Y;

	vertices[18] = X + x1p;
	vertices[19] = Y;

	vertices[27] = X + x1p;
	vertices[28] = Y + y1p;

	vertices[7] = x2 / 100;
	vertices[8] = y2 / 100;
	vertices[16] = x2 / 100;
	vertices[17] = 0.0;
	vertices[25] = x1 / 100;
	vertices[26] = 0.0;
	vertices[34] = x1 / 100;
	vertices[35] = y1 / 100;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);
}

void MapFragment::drawImage()
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tl->getTerrain());
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