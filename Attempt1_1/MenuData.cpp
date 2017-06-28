#include "MenuData.h"

MenuData::MenuData()
{
}

MenuData::MenuData(DataType type, int arrayIndex)
{
	dataType = type;
	index = arrayIndex;
	// set the array values for the type of data to represented in the class instance
	// and set the arrayLength var accordingly
	switch (type)
	{
	case DataType::GridSize: 
	{
		arrayLength = 5;
		break;
	}
		
	case DataType::GridWidth: 
	{
		arrayLength = 10;
		break;
	}

	case DataType::MinHeight: 
	{
		arrayLength = 13;
		break;
	}

	case DataType::MaxHeight: 
	{
		arrayLength = 12;
		break;
	}

	case DataType::NumberGen: 
	{
		arrayLength = 2;
		break;
	}

	case DataType::MinGen: 
	{
		arrayLength = 9;
		break;
	}

	case DataType::MaxGen: 
	{
		arrayLength = 14;
		break;
	}

	case DataType::Mean: 
	{
		arrayLength = 9;
		break;
	}

	case DataType::Range: 
	{
		arrayLength = 18;
		break;
	}

	case DataType::Smooth: 
	{
		arrayLength = 2;
		break;
	}

	case DataType::SmoothCount:
	{
		arrayLength = 11;
		break;
	}

	case DataType::Offset: 
	{
		arrayLength = 11;
		break;
	}

	}
}

sf::String MenuData::GetNextValue()
{
	index = (index + 1);
	if (index % arrayLength == 0)
		index = 0;
	return GetCurrentValue();

}

sf::String MenuData::GetPrevValue()
{
	index = (index - 1);
	if (index < 0)
		index = arrayLength - 1;
	return GetCurrentValue();
}

int MenuData::GetIndex()
{
	return index;
}

sf::String MenuData::GetCurrentValue()
{
	switch (dataType)
	{
	case (DataType::GridSize):
		return ValueToString(data_GridSize[index]);
	case (DataType::GridWidth):
		return ValueToString(data_GridWidth[index]);
	case DataType::MinHeight:
		return ValueToString(data_MinHeight[index]);
	case DataType::MaxHeight:
		return ValueToString(data_MaxHeight[index]);
	case DataType::NumberGen:
		return ValueToString(data_NumberGen[index]);
	case DataType::MinGen:
		return ValueToString(data_MinGen[index]);
	case DataType::MaxGen:
		return ValueToString(data_MaxGen[index]);
	case DataType::Mean:
		return ValueToString(data_Mean[index]);
	case DataType::Range:
		return ValueToString(data_Range[index]);
	case DataType::Smooth:
		return ValueToString(data_Smoother[index]);
	case DataType::SmoothCount:
		return ValueToString(data_SmoothCount[index]);
	case DataType::Offset:
		return ValueToString(data_Offset[index]);
	}
}

sf::String MenuData::ValueToString(int num)
{
	std::ostringstream stream;
	stream << num;
	return sf::String(stream.str());
}

sf::String MenuData::ValueToString(float num)
{
	std::ostringstream stream;
	stream << num;
	return sf::String(stream.str());
}

sf::String MenuData::ValueToString(HeightMap::RandomNumber random)
{
	switch (random)
	{
	case HeightMap::RandomNumber::normalDistribution:
		return sf::String("Normal Distribution");
	case HeightMap::RandomNumber::logNormalDistribution:
		return sf::String("Lognormal Distribution");
	default:
		return sf::String("Error");
	}
}

sf::String MenuData::ValueToString(HeightMap::Smoother smooth)
{
	switch (smooth)
	{
	case HeightMap::Smoother::normalSmoothing:
		return sf::String("Normal Smoothing");
	case HeightMap::Smoother::aggressiveSmoothing:
		return sf::String("Aggressive Smoothing");
	default:
		return sf::String("Error");
	}
}

MenuData::DataType MenuData::GetDataType()
{
	return dataType;
}

int MenuData::GetSelection(int identifier)
{
	switch (dataType)
	{
	case DataType::GridSize:
		return data_GridSize[index];
	case DataType::GridWidth:
		return data_GridWidth[index];
	case DataType::SmoothCount:
		return data_SmoothCount[index];
	default:
		return -1;
	}
}

float MenuData::GetSelection(float identifier)
{
	switch (dataType)
	{
	case DataType::MinHeight:
		return data_MinHeight[index];
	case DataType::MaxHeight:
		return data_MaxHeight[index];
	case DataType::MinGen:
		return data_MinGen[index];
	case DataType::MaxGen:
		return data_MaxGen[index];
	case DataType::Mean:
		return data_Mean[index];
	case DataType::Range:
		return data_Range[index];
	case DataType::Offset:
		return data_Offset[index];
	default:
		return -1;
	}
}

HeightMap::RandomNumber MenuData::GetSelection(HeightMap::RandomNumber identifier)
{
	if (dataType == DataType::NumberGen)
		return data_NumberGen[index];
}

HeightMap::Smoother MenuData::GetSelection(HeightMap::Smoother identifier)
{
	if (dataType == DataType::Smooth)
		return data_Smoother[index];
}


MenuData::~MenuData()
{
}
