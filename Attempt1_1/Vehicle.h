#pragma once
#include "glew.h"
#include "SFML/OpenGL.hpp"
//#include <stdlib.h>
//#include <vector>
//#include "glm\common.hpp"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

class Vehicle
{
public:
	Vehicle();
	void LoadObjectFile(std::string location);
	void Render();
	~Vehicle();

private:
	std::string path;

	float *vertexArray;
	float *normalArray;
	float *uvArray;

	int numVerts;
};

