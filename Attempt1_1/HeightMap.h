#pragma once
#include <vector>
#include "SFML/OpenGL.hpp"
#include <random>


class HeightMap
{
public:
	enum Smoother {
		normalSmoothing,
		aggressiveSmoothing
	};
	enum RandomNumber {
		normalDistribution,
		logNormalDistribution
	};

	HeightMap::HeightMap();
	HeightMap(int vectorCount, int gridSize, float min, float max, Smoother smooth, 
		int smoothCount, RandomNumber random, float randomMin, float randomMax, float offset);
	~HeightMap();

	

	std::pair <int, int> HeightMap::convert1Dto2D(int id, int sides);
	int HeightMap::convert2Dto1D(int row, int col, int sides);
	void HeightMap::Render();
	void HeightMap::GenerateHeightMap();

	void HeightOffset();

private:
	///Vectors for holding the vectors and faces
	std::vector<GLfloat> vectorBuffer;
	std::vector<GLint> facesIndex;

	std::vector<GLfloat> colourBuffer;

	///random number related vars
	std::random_device rd;
	std::mt19937 generator;// (rd());
	//std::uniform_real_distribution<double> uniformReal;//(-10.0f, 300.0f);
	std::normal_distribution<double> normal;//(1.0, 1000.0);
	std::lognormal_distribution<double> logNormal;// (1.0, 5.5);
	float min_height;
	float max_height;

	///
	int vectorCount;
	float gridWidth;
	Smoother smoother;
	int smoothIterations;
	RandomNumber randomNumber;
	float** heightmap;
	float displacementOffset;


	double HeightMap::heightBoundCheck(double initialHeight);
	double HeightMap::GetRandomNumber();
	void HeightMap::square(int x, int y, int size, float* heightmap[], float displacement, int sides);
	void HeightMap::diamond(int x, int y, int size, float* heightmap[], float displacement, int sides);
	void HeightMap::Smooth(float* heightmap[], int sides);

};

