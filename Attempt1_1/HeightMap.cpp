#include "HeightMap.h"



HeightMap::HeightMap(int vectors, int gridSize, float min, float max, Smoother smooth, int smoothCount, RandomNumber random, float randomA, float randomB, float offset)
{
	generator = std::mt19937(rd());
	uniformReal = std::uniform_real_distribution<double>(randomA, randomB);			// -10.0, 300.0 
	normal = std::normal_distribution<double>(randomA, randomB);					// 1.0, 1000.0
	logNormal = std::lognormal_distribution<double>(randomA, randomB);				// 1.0, 5.5
	min_height = min;
	max_height = max;
	vectorCount = vectors;
	gridWidth = gridSize;
	smoother = smooth;
	smoothIterations = smoothCount;
	randomNumber = random;
	displacementOffset = offset;
}


HeightMap::~HeightMap()
{
}


double HeightMap::GetRandomNumber()
{
	if (randomNumber == RandomNumber::normalDistribution)
		return normal(generator);
	else if (randomNumber == RandomNumber::logNormalDistribution)
		return logNormal(generator);
	else if (randomNumber == RandomNumber::uniformDistribution)
		return uniformReal(generator);
}


double HeightMap::heightBoundCheck(double initialHeight)
{
	if (initialHeight > max_height)
		return max_height;
	else if (initialHeight < min_height)
		return min_height;
	return initialHeight;
}


void HeightMap::square(int x, int y, int size, float* heightmap[], float displacement, int sides)
{
	int half_size = size / 2;

	float top_center = 0;
	if (y - half_size >= 0)
	{
		top_center = heightmap[x + half_size][y - half_size];
	}
	float top_left = heightmap[x][y];
	float top_right = heightmap[x][y + size - 1];
	float center = heightmap[x + half_size][y + half_size];
	float bottom_left = heightmap[x + size - 1][y];
	float bottom_right = heightmap[x + size - 1][y + size - 1];

	float a = (top_left + center + top_right) / 3.0f;
	float b = (top_left + center + bottom_left) / 3.0f;
	float c = (top_right + center + bottom_right) / 3.0f;
	float d = (bottom_left + center + bottom_right) / 3.0f;

	//  a
	//b   c
	//  d

	//check if on the edge of the grid. If above water level (0), set to 0 to avoid gaping edges
	if (x == 0)	heightmap[x][y + half_size] = 0;
	else heightmap[x][y + half_size] = heightBoundCheck(a + GetRandomNumber()*displacement); // a

	if (y == 0) heightmap[x + half_size][y] = 0;
	else heightmap[x + half_size][y] = heightBoundCheck(b + GetRandomNumber()*displacement); // b

	if (x > 0) heightmap[x + half_size][y + size - 1] = 0;
	else heightmap[x + half_size][y + size - 1] = heightBoundCheck(c + GetRandomNumber()*displacement); // c

	if (y > 0) heightmap[x + size - 1][y + half_size] = 0;
	else heightmap[x + size - 1][y + half_size] = heightBoundCheck(d + GetRandomNumber()*displacement); // d
}


void HeightMap::diamond(int x, int y, int size, float* heightmap[], float displacement, int sides)
{
	int half_size = size / 2;

	float top_left = heightmap[x][y];
	float top_right = heightmap[x + size - 1][y];
	float bottom_left = heightmap[x][y + size - 1];
	float bottom_right = heightmap[x + size - 1][y + size - 1];

	float average = (top_left + bottom_left + top_right + bottom_right) / 4.0f;

	heightmap[x + half_size][y + half_size] = heightBoundCheck(average + GetRandomNumber()*displacement);

	square(x, y, size, heightmap, displacement, sides);

	if (half_size > 1)
	{
		diamond(x, y, half_size + 1, heightmap, displacement / displacementOffset/*1.2f*/, sides);
		diamond(x + half_size, y, half_size + 1, heightmap, displacement / displacementOffset/*1.2f*/, sides);
		diamond(x, y + half_size, half_size + 1, heightmap, displacement / displacementOffset/*1.2f*/, sides);
		diamond(x + half_size, y + half_size, half_size + 1, heightmap, displacement / displacementOffset/*1.2f*/, sides);
	}
}


void HeightMap::Smooth(float * heightmap[], int sides)
{
	for (int i = 1; i < sides; ++i)
	{
		for (int j = 1; j < sides; ++j)
		{

			float aa = heightmap[(i - 1 + sides) % sides][(j - 1 + sides) % sides];
			float ab = heightmap[(i + sides) % sides][(j - 1 + sides) % sides];
			float ac = heightmap[(i + 1 + sides) % sides][(j - 1 + sides) % sides];
			float ba = heightmap[(i - 1 + sides) % sides][(j + sides) % sides];
			float bb = heightmap[(i + sides) % sides][(j + sides) % sides];
			float bc = heightmap[(i + 1 + sides) % sides][(j + sides) % sides];
			float ca = heightmap[(i - 1 + sides) % sides][(j + 1 + sides) % sides];
			float cb = heightmap[(i + sides) % sides][(j + 1 + sides) % sides];
			float cc = heightmap[(i + 1 + sides) % sides][(j + 1 + sides) % sides];

			float average = (aa + ab + ac + ba + bb + bc + ca + cb + cc) / 9;
			if (i == sides - 1 || j == sides - 1) heightmap[i][j] = 0;
			else
			{
				if (smoother == Smoother::normalSmoothing)
					heightmap[i][j] = (heightmap[i][j] + average) / 2;
				else if (smoother == Smoother::aggressiveSmoothing)
					heightmap[i][j] = average;
			}
		}
	}
}


std::pair<int, int> HeightMap::convert1Dto2D(int id, int sides)
{
	int row = id / sides;
	int col = id % sides;

	return std::pair<int, int>(row, col);
}

int HeightMap::convert2Dto1D(int row, int col, int sides)
{
	return (row * sides) + col;
}


void HeightMap::Render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe on, must be set back before UI gets drawn

	glColor3f(0, 1, 0);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vectorBuffer[0]);

	//glColorPointer();TODO

	glDrawElements(GL_TRIANGLE_STRIP, facesIndex.size(), GL_UNSIGNED_INT, &facesIndex[0]);

	glDisableClientState(GL_VERTEX_ARRAY);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // wireframe off
}


void HeightMap::GenerateHeightMap()
{
	heightmap = new float*[vectorCount];
	for (int i = 0; i < vectorCount; ++i)
	{
		heightmap[i] = new float[vectorCount];
	}

	heightmap[0][0] = double(0.0000);
	heightmap[vectorCount - 1][0] = double(0.0000);
	heightmap[0][vectorCount - 1] = double(0.0000);
	heightmap[vectorCount - 1][vectorCount - 1] = double(0.0000);


	diamond(0, 0, vectorCount, heightmap, 2.0f, vectorCount);

	/// run the smooth function x number of times
	for (int i = 0; i < smoothIterations; i++)
	{
		Smooth(heightmap, vectorCount);
	}

	int v = 0;
	float width = 200.0f;

	float maxWidth = width * vectorCount;

	for (int i = 0; i <vectorCount; i++)
	{
		for (int j = 0; j < vectorCount; j++)
		{
			float x = i*width - (maxWidth / 2);
			float y = heightmap[i][j];
			float z = j*width;

			vectorBuffer.push_back(x);
			vectorBuffer.push_back(y);
			vectorBuffer.push_back(z);

			v++;
		}
	}

	facesIndex.push_back(0);

	int limit = vectorCount * vectorCount - 1;

	for (int i = 0; i < vectorCount; i++)
	{
		for (int j = 0; j < vectorCount; j++)
		{
			int vertex = convert2Dto1D(i, j, vectorCount); // little method to calculate the vertex identifier
			int adjacent = vertex + 1;
			int below = vertex + vectorCount;

			if (below <= limit)
			{
				facesIndex.push_back(below);
			}
			if (j == vectorCount - 1) // is this the end of the row?
			{
				facesIndex.push_back(0xff);
			}
			else
			{
				if (adjacent <= limit)
				{
					facesIndex.push_back(adjacent);
				}

			}
		}
	}
}
