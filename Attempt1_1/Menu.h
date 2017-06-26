#pragma once
#include "SFML\Graphics.hpp"
#include "HeightMap.h"
#include "MenuData.h"
#include <list>
#include <iostream>



class Menu
{
public:
	Menu(sf::RenderWindow &window);
	
	~Menu();

	void Update();

	bool Menu::Run();

	void Menu::HighlightSelection();

	void Menu::Draw();

	HeightMap & Menu::GetHeightMap();

	int finalGridSize;
	int finalGridWidth;
	float finalMinHeight;
	float finalMaxHeight;
	HeightMap::RandomNumber finalNumberGen;
	float finalNumberGenFirst;
	float finalNumberGenSecond;
	HeightMap::Smoother finalSmoother;
	int finalSmoothCount;
	float finalOffset;

private:
	struct Data 
	{
		MenuData menudata;
		sf::Text text;
		bool isHighlighted;
		int index;
	};
	
	HeightMap heightmap;

	sf::RenderWindow * m_window;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	sf::Font textFont;

	//text objects for the titles of data
	sf::Text txt_title, txt_gridSize, txt_gridWidth, txt_heightMin, txt_heightMax, txt_numberGen, txt_numberGenFirst, txt_numberGenSecond, txt_smoothing, txt_smoothingX;
	
	//text objects for the data
	Data data_gridSize, data_gridWidth, data_heightMin, data_heightMax, data_numberGen, data_numberGenFirst, data_numberGenSecond, data_smoothingCount, data_smoothingType;

	std::list<sf::Text> textList;
	std::list<Data> dataList;

	Data * highlightedData;
	sf::RectangleShape highlight;
	sf::Color highlightColour;



	

	
};

