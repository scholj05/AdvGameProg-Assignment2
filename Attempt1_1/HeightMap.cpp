/*
*	This file and its contents were written by the author, unless stated otherwise.
*	The code is in no way guaranteed to be in working condition,
*	nor does the author claim any responsibility for negative outcomes of running
*	the contents of this file.
*  Any use of this file must be done with the permission of the author.
*
*	@author		Jesse Schollitt
*/

#include "HeightMap.h"

HeightMap::HeightMap()
{

}

HeightMap::HeightMap(int vectors, int gridSize, float min, float max, Smoother smooth, int smoothCount, RandomNumber random, float randomA, float randomB, float offset)
{
	generator = std::mt19937(rd());
	if (random == RandomNumber::normalDistribution)
		normal = std::normal_distribution<double>(double(randomA), double(randomB));
	else
		logNormal = std::lognormal_distribution<double>(randomA, randomB);
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

	heightmap[x][y + half_size] = heightBoundCheck(a + GetRandomNumber()*displacement);
	heightmap[x + half_size][y] = heightBoundCheck(b + GetRandomNumber()*displacement);
	heightmap[x + half_size][y + size - 1] = heightBoundCheck(c + GetRandomNumber()*displacement);
	heightmap[x + size - 1][y + half_size] = heightBoundCheck(d + GetRandomNumber()*displacement);
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
		diamond(x, y, half_size + 1, heightmap, displacement / displacementOffset, sides);
		diamond(x + half_size, y, half_size + 1, heightmap, displacement / displacementOffset, sides);
		diamond(x, y + half_size, half_size + 1, heightmap, displacement / displacementOffset, sides);
		diamond(x + half_size, y + half_size, half_size + 1, heightmap, displacement / displacementOffset, sides);
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
			
			if (smoother == Smoother::normalSmoothing)
				heightmap[i][j] = (heightmap[i][j] + average) / 2;
			else if (smoother == Smoother::aggressiveSmoothing)
				heightmap[i][j] = average;

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
	glEnable(GL_DEPTH_TEST); // ensure only things that should be visible are actually seen
	
	glEnable(GL_BLEND); 

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // set the sides of polygon to be drawn and the type of colouring (LINE for wireframe, FILL for colour-fill)

	glEnableClientState(GL_COLOR_ARRAY);// enable the use of colour arrays

	glEnableClientState(GL_VERTEX_ARRAY);// enable the use of vertex arrays

	glVertexPointer(3, GL_FLOAT, 0, &vectorBuffer[0]);// set parameters of vertex array (values per point, datatype, stripe, start)

	glColorPointer(4, GL_FLOAT, 0, &colourBuffer[0]);// set parameters of colour array (values per point, datatype, stripe, start)

	glDrawElements(GL_TRIANGLE_STRIP, facesIndex.size(), GL_UNSIGNED_INT, &facesIndex[0]);// draw the stuff (draw type, number of poly's, datatype, start)

	glDisableClientState(GL_VERTEX_ARRAY);// disable after use

	glDisableClientState(GL_COLOR_ARRAY);// disable after use

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // set mode for post-render drawing (other objects like UI);
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

	// run the smooth function x number of times
	for (int i = 0; i < smoothIterations; i++)
	{
		Smooth(heightmap, vectorCount);
	}

	HeightOffset();

	int v = 0;
	float width = 200.0f;

	float maxWidth = width * vectorCount;
	float colourOffset = 0.0f;
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

			float r, g, b, a;
			float segment = max_height / 100;

			oceanPoint = segment * 5;

			if (y < oceanPoint)// ocean (transparent)
			{
				r = 0;
				g = 0;
				b = 0;
				a = 0;
			}
			if (y >= segment * 2.5 && y < segment * 5)// waves
			{
				r = 0.5;
				g = 0.5;
				b = 1;
				a = 0.1;
			}
			if (y >= segment * 5 && y < segment * 12)// sand (yellow)
			{
				r = (1-(1.0f / max_height) * y*2);
				g = r-0.1;
				b = g/2;
				a = 1;
			}
			if (y >= segment * 12 && y < segment * 15)// grass (green)
			{
				g = (1.0f / max_height) * y *3;
				r = g/2;
				b = r/2;
				a = 1;
			}
			if (y >= segment * 15 && y < segment * 22)// grass (upper)
			{
				g = (1.0f / max_height) * y * 2.5;
				r = g / 2;
				b = r / 2;
				a = 1;
			}
			if (y >= segment * 22 && y < segment * 30)// mountain (brown)
			{
				r = (1.0f / max_height) * y;
				g = (0.75f / max_height) * y *1.5;
				b = (0.5f / max_height) * y;
				a = 1;
			}
			if (y >= segment * 30 && y < segment * 80)// mountain (dark brown)
			{
				r = (1.0f / max_height) * y /2;
				g = (0.75f / max_height) * y /2;
				b = (0.5f / max_height) * y /2;
				a = 1;
			}
			if (y >= segment * 80 && y < segment * 100)// mountain top (white)
			{
				r = g = b = a = 1.0f;
			}

			colourBuffer.push_back(r);
			colourBuffer.push_back(g);
			colourBuffer.push_back(b);
			colourBuffer.push_back(a);

			v++;
		}
	}

	facesIndex.push_back(0);

	int limit = vectorCount * vectorCount -1;

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

void HeightMap::HeightOffset()
{
	heightOffsetMap = new float*[vectorCount];

	for (int x = 0; x < vectorCount; x++)
	{
		heightOffsetMap[x] = new float[vectorCount];
		for (int y = 0; y < vectorCount; y++)
		{
			int offsetXRight = vectorCount - (x + 1);
			float offsetX = (x < offsetXRight ? x : offsetXRight);

			int offsetYRight = vectorCount - (y + 1);
			float offsetY = (y < offsetYRight ? y : offsetYRight);

			float offset = 1 - ((vectorCount - (offsetX < offsetY ? offsetX : offsetY)) / vectorCount);

			if (heightmap[x][y] > 0) 
				heightmap[x][y] *= (2 * offset);
		}
	}
}

float HeightMap::GetOceanPoint()
{
	return oceanPoint;
}