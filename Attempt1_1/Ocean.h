#pragma once
#include "SFML\OpenGL.hpp"
class Ocean
{
public:
	Ocean();
	~Ocean();
	void Setup(float oceanSize, float y);
	void Ocean::Render(float * matrix);

private:
	float size;
	float height;

};

