#pragma once
#include "SFML\Graphics.hpp"
#include "HeightMap.h"
#include <sstream>

class MenuData
{
public:
	enum DataType {
		GridSize,
		GridWidth,
		MinHeight,
		MaxHeight,
		NumberGen,
		MinGen,
		MaxGen,
		Mean,
		Range,
		Smooth,
		SmoothCount,
		Offset
	};
	
	MenuData();
	~MenuData();

	MenuData(DataType type, int arrayIndex);

	sf::String GetNextValue();

	sf::String GetPrevValue();

	int GetIndex();

	sf::String GetCurrentValue();

	MenuData::DataType GetDataType();

	int GetSelection(int identifier);
	float GetSelection(float identifier);
	HeightMap::RandomNumber GetSelection(HeightMap::RandomNumber identifier);
	HeightMap::Smoother GetSelection(HeightMap::Smoother identifier);

private:
	DataType dataType;
	sf::String data_text;

	int data_GridSize[5] = { 32, 64, 128, 256, 512 };
	int data_GridWidth[10] = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500 };
	float data_MinHeight[13] = { 0.0f, -50.0f, -100.0f, -200.0f, -300.0f, -400.0f, -500.0f, -750.0f, -1000.0f, -2000.0f, -3000.0f, -5000.0f, -10000.0f };
	float data_MaxHeight[12] = { 50.0f, 100.0f, 200.0f, 300.0f, 400.0f, 500.0f, 750.0f, 1000.0f, 2000.0f, 3000.0f, 5000.0f, 10000.0f };
	HeightMap::RandomNumber data_NumberGen[2] = { HeightMap::RandomNumber::normalDistribution, HeightMap::RandomNumber::logNormalDistribution};
	float data_MinGen[9] = { 0.0f, -25.0f, -50.0f, -100.0f, -200.0f, -300.0f, -500.0f, -750.0f, -1000.0f };
	float data_MaxGen[14] = { 0.0f, 25.0f, 50.0f, 100.0f, 200.0f, 300.0f, 500.0f, 750.0f, 1000.0f, 2000.0f, 5000.0f, 10000.0f, 15000.0f, 20000.0f };
	float data_Mean[9] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 10.0f, 15.0f, 20.0f, 50.0f };
	float data_Range[18] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 10.0f, 15.0f, 20.0f, 50.0f, 75.0f, 100.0f, 200.0f, 300.0f, 500.0f, 750.0f, 1000.0f, 2000.0f, 5000.0f };
	HeightMap::Smoother data_Smoother[2] = { HeightMap::Smoother::aggressiveSmoothing, HeightMap::Smoother::normalSmoothing };
	int data_SmoothCount[11] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	float data_Offset[11] = { 0.1f, 0.5f, 1.0f, 1.1f, 1.2f, 1.5f, 2.0f, 3.0f, 4.0f, 5.0f, 10.0f };

	int index;
	int arrayLength;
	
	sf::String ValueToString(int num);
	sf::String ValueToString(float num);
	sf::String ValueToString(HeightMap::RandomNumber random);
	sf::String ValueToString(HeightMap::Smoother smooth);

	
};

