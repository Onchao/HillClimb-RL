#include "point.h"

float dist(point a, point b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;

	return std::sqrt(dx * dx + dy * dy);
}

float length(point a)
{
	return std::sqrt(a.x * a.x + a.y * a.y);
}
