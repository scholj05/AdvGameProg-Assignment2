/*
*	This file and its contents were written by the author, unless stated otherwise.
*	The code is in no way guaranteed to be in working condition,
*	nor does the author claim any responsibility for negative outcomes of running
*	the contents of this file.
*  Any use of this file must be done with the permission of the author.
*
*	@author		Jesse Schollitt
*/

#include "Menu.h"


Menu::Menu(sf::RenderWindow &window)
{
	m_window = &window;
	
	if (!textFont.loadFromFile("../resources/TCM_____.TTF"))
		printf("error loading font 'TCM_____.TTF'");

	if (!backgroundTexture.loadFromFile("../resources/backgroundTexture.png"))
		printf("error loading texture 'backgroundTexture.png'");
	else 
	{
		backgroundSprite.setTexture(backgroundTexture);
		backgroundSprite.setScale
		(
			m_window->getSize().x / backgroundSprite.getLocalBounds().width,
			m_window->getSize().y / backgroundSprite.getLocalBounds().height
		);
	}

	highlightColour = sf::Color(255, 255, 255, 100);
	highlight.setFillColor(sf::Color(0, 0, 0, 0));
	highlightedData = NULL;

	int windowX = m_window->getSize().x;
	int windowY = m_window->getSize().y;

	// setup title texts
	txt_title.setFont(textFont);
	txt_title.setCharacterSize(windowX / 25);
	txt_title.setString("World Generation Settings");
	txt_title.setOrigin(txt_title.getLocalBounds().width / 2, txt_title.getLocalBounds().height / 2);
	txt_title.setPosition(windowX / 2, windowY / 10);
	textList.push_back(txt_title);

	txt_gridSize.setFont(textFont);
	txt_gridSize.setCharacterSize(windowX / 50);
	txt_gridSize.setString("Grid Size");
	txt_gridSize.setOrigin(txt_gridSize.getGlobalBounds().width / 2, txt_gridSize.getGlobalBounds().height / 2);
	txt_gridSize.setPosition(windowX / 4, windowY / 5);
	textList.push_back(txt_gridSize);

	txt_gridWidth.setFont(textFont);
	txt_gridWidth.setCharacterSize(windowX / 50);
	txt_gridWidth.setString("Grid Width");
	txt_gridWidth.setOrigin(txt_gridWidth.getGlobalBounds().width / 2, txt_gridWidth.getGlobalBounds().height / 2);
	txt_gridWidth.setPosition(windowX / 4, windowY / 5 * 2);
	textList.push_back(txt_gridWidth);

	txt_heightMin.setFont(textFont);
	txt_heightMin.setCharacterSize(windowX / 50);
	txt_heightMin.setString("Min Height");
	txt_heightMin.setOrigin(txt_heightMin.getGlobalBounds().width / 2, txt_heightMin.getGlobalBounds().height / 2);
	txt_heightMin.setPosition(windowX / 4 - txt_heightMin.getGlobalBounds().width, windowY / 5 * 3);
	textList.push_back(txt_heightMin);

	txt_heightMax.setFont(textFont);
	txt_heightMax.setCharacterSize(windowX / 50);
	txt_heightMax.setString("Max Height");
	txt_heightMax.setOrigin(txt_heightMax.getGlobalBounds().width / 2, txt_heightMax.getGlobalBounds().height / 2);
	txt_heightMax.setPosition(windowX / 4 + txt_heightMax.getGlobalBounds().width, windowY / 5 * 3);
	textList.push_back(txt_heightMax);

	txt_numberGen.setFont(textFont);
	txt_numberGen.setCharacterSize(windowX / 50);
	txt_numberGen.setString("Number Generation");
	txt_numberGen.setOrigin(txt_numberGen.getGlobalBounds().width / 2, txt_numberGen.getGlobalBounds().height / 2);
	txt_numberGen.setPosition(windowX / 4 * 3, windowY / 5);
	textList.push_back(txt_numberGen);

	txt_numberGenFirst.setFont(textFont);
	txt_numberGenFirst.setCharacterSize(windowX / 50);
	txt_numberGenFirst.setString("Mean");
	txt_numberGenFirst.setOrigin(txt_numberGenFirst.getGlobalBounds().width / 2, txt_numberGenFirst.getGlobalBounds().height / 2);
	txt_numberGenFirst.setPosition(windowX / 4 * 3 - txt_numberGenFirst.getGlobalBounds().width, windowY / 5 * 2);
	textList.push_back(txt_numberGenFirst);

	txt_numberGenSecond.setFont(textFont);
	txt_numberGenSecond.setCharacterSize(windowX / 50);
	txt_numberGenSecond.setString("Sigma");
	txt_numberGenSecond.setOrigin(txt_numberGenSecond.getGlobalBounds().width / 2, txt_numberGenSecond.getGlobalBounds().height / 2);
	txt_numberGenSecond.setPosition(windowX / 4 * 3 + txt_numberGenSecond.getGlobalBounds().width, windowY / 5 * 2);
	textList.push_back(txt_numberGenSecond);

	txt_smoothing.setFont(textFont);
	txt_smoothing.setCharacterSize(windowX / 50);
	txt_smoothing.setString("Smoothing");
	txt_smoothing.setOrigin(txt_smoothing.getGlobalBounds().width / 2, txt_smoothing.getGlobalBounds().height / 2);
	txt_smoothing.setPosition(windowX / 4 * 3, windowY / 5 * 3);
	textList.push_back(txt_smoothing);

	txt_smoothingX.setFont(textFont);
	txt_smoothingX.setCharacterSize(windowX / 75);
	txt_smoothingX.setString("X");
	txt_smoothingX.setOrigin(txt_smoothingX.getGlobalBounds().width / 2, txt_smoothingX.getGlobalBounds().height / 2);
	txt_smoothingX.setPosition(windowX / 4 * 3, (windowY / 5 * 3) + (txt_smoothingX.getGlobalBounds().height * 3));
	textList.push_back(txt_smoothingX);

	// setup data structs
	data_gridSize.isHighlighted = true;
	data_gridSize.menudata = MenuData(MenuData::DataType::GridSize, 3);
	data_gridSize.text.setFont(textFont);
	data_gridSize.text.setCharacterSize(windowX / 75);
	data_gridSize.text.setString(data_gridSize.menudata.GetCurrentValue());
	data_gridSize.text.setOrigin(data_gridSize.text.getGlobalBounds().width / 2, data_gridSize.text.getGlobalBounds().height / 2);
	data_gridSize.text.setPosition(windowX / 4, windowY / 5 + data_gridSize.text.getGlobalBounds().height * 3);
	dataList.push_back(data_gridSize);

	data_gridWidth.isHighlighted = false;
	data_gridWidth.menudata = MenuData(MenuData::DataType::GridWidth, 5);
	data_gridWidth.text.setFont(textFont);
	data_gridWidth.text.setCharacterSize(windowX / 75);
	data_gridWidth.text.setString(data_gridWidth.menudata.GetCurrentValue());
	data_gridWidth.text.setOrigin(data_gridWidth.text.getGlobalBounds().width / 2, data_gridWidth.text.getGlobalBounds().height / 2);
	data_gridWidth.text.setPosition(windowX / 4, (windowY / 5 * 2) + (data_gridWidth.text.getGlobalBounds().height * 3));
	dataList.push_back(data_gridWidth);

	data_heightMin.isHighlighted = false;
	data_heightMin.menudata = MenuData(MenuData::DataType::MinHeight, 1);
	data_heightMin.text.setFont(textFont);
	data_heightMin.text.setCharacterSize(windowX / 75);
	data_heightMin.text.setString(data_heightMin.menudata.GetCurrentValue());
	data_heightMin.text.setOrigin(data_heightMin.text.getGlobalBounds().width / 2, data_heightMin.text.getGlobalBounds().height / 2);
	data_heightMin.text.setPosition(txt_heightMin.getPosition().x, (windowY / 5 * 3) + (data_heightMin.text.getGlobalBounds().height * 3));
	dataList.push_back(data_heightMin);

	data_heightMax.isHighlighted = false;
	data_heightMax.menudata = MenuData(MenuData::DataType::MaxHeight, 9);
	data_heightMax.text.setFont(textFont);
	data_heightMax.text.setCharacterSize(windowX / 75);
	data_heightMax.text.setString(data_heightMax.menudata.GetCurrentValue());
	data_heightMax.text.setOrigin(data_heightMax.text.getGlobalBounds().width / 2, data_heightMax.text.getGlobalBounds().height / 2);
	data_heightMax.text.setPosition(txt_heightMax.getPosition().x, (windowY / 5 * 3) + (data_heightMax.text.getGlobalBounds().height * 3));
	dataList.push_back(data_heightMax);

	data_numberGen.isHighlighted = false;
	data_numberGen.menudata = MenuData(MenuData::DataType::NumberGen, 1);
	data_numberGen.text.setFont(textFont);
	data_numberGen.text.setCharacterSize(windowX / 75);
	data_numberGen.text.setString(data_numberGen.menudata.GetCurrentValue());
	data_numberGen.text.setOrigin(data_numberGen.text.getGlobalBounds().width / 2, data_numberGen.text.getGlobalBounds().height / 2);
	data_numberGen.text.setPosition(txt_numberGen.getPosition().x, (windowY / 5) + (data_numberGen.text.getGlobalBounds().height * 3));
	dataList.push_back(data_numberGen);

	data_numberGenFirst.isHighlighted = false;
	data_numberGenFirst.menudata = MenuData(MenuData::DataType::Mean, 0);
	data_numberGenFirst.text.setFont(textFont);
	data_numberGenFirst.text.setCharacterSize(windowX / 75);
	data_numberGenFirst.text.setString(data_numberGenFirst.menudata.GetCurrentValue());
	data_numberGenFirst.text.setOrigin(data_numberGenFirst.text.getGlobalBounds().width / 2, data_numberGenFirst.text.getGlobalBounds().height / 2);
	data_numberGenFirst.text.setPosition(txt_numberGenFirst.getPosition().x, (windowY / 5 * 2) + (data_numberGenFirst.text.getGlobalBounds().height * 3));
	dataList.push_back(data_numberGenFirst);

	data_numberGenSecond.isHighlighted = false;
	data_numberGenSecond.menudata = MenuData(MenuData::DataType::Range, 3);
	data_numberGenSecond.text.setFont(textFont);
	data_numberGenSecond.text.setCharacterSize(windowX / 75);
	data_numberGenSecond.text.setString(data_numberGenSecond.menudata.GetCurrentValue());
	data_numberGenSecond.text.setOrigin(data_numberGenSecond.text.getGlobalBounds().width / 2, data_numberGenSecond.text.getGlobalBounds().height / 2);
	data_numberGenSecond.text.setPosition(txt_numberGenSecond.getPosition().x, (windowY / 5 * 2) + (data_numberGenSecond.text.getGlobalBounds().height * 3));
	dataList.push_back(data_numberGenSecond);

	data_smoothingCount.isHighlighted = false;
	data_smoothingCount.menudata = MenuData(MenuData::DataType::SmoothCount, 3);
	data_smoothingCount.text.setFont(textFont);
	data_smoothingCount.text.setCharacterSize(windowX / 75);
	data_smoothingCount.text.setString(data_smoothingCount.menudata.GetCurrentValue());
	data_smoothingCount.text.setOrigin(data_smoothingCount.text.getGlobalBounds().width / 2, data_smoothingCount.text.getGlobalBounds().height / 2);
	data_smoothingCount.text.setPosition(txt_smoothingX.getPosition().x - data_smoothingCount.text.getGlobalBounds().width * 2, (windowY / 5 * 3) + (data_smoothingCount.text.getGlobalBounds().height * 3));
	dataList.push_back(data_smoothingCount);

	data_smoothingType.isHighlighted = false;
	data_smoothingType.menudata = MenuData(MenuData::DataType::Smooth, 1);
	data_smoothingType.text.setFont(textFont);
	data_smoothingType.text.setCharacterSize(windowX / 75);
	data_smoothingType.text.setString(data_smoothingType.menudata.GetCurrentValue());
	data_smoothingType.text.setOrigin(data_smoothingType.text.getGlobalBounds().width / 2, data_smoothingType.text.getGlobalBounds().height / 2);
	data_smoothingType.text.setPosition(txt_smoothingX.getPosition().x + data_smoothingType.text.getGlobalBounds().width * 0.75, (windowY / 5 * 3) + (data_smoothingCount.text.getGlobalBounds().height * 3));
	dataList.push_back(data_smoothingType);
}

void Menu::Update()
{
	int padding = 10;
	bool toHighlight = false;
	for (std::list<Data>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		if ((*it).text.getGlobalBounds().contains(m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window))))
		{
			toHighlight = true;
			highlightedData = &(*it);
			highlight.setPosition((*it).text.getGlobalBounds().left - padding, (*it).text.getGlobalBounds().top - padding);
			highlight.setSize(sf::Vector2f((*it).text.getGlobalBounds().width + padding * 2, (*it).text.getGlobalBounds().height + padding * 2));
			highlight.setFillColor(highlightColour);
			(*it).isHighlighted = true;
		}
		else
			(*it).isHighlighted = false;

	}
	if (!toHighlight)
	{
		highlightedData = NULL;
		highlight.setFillColor(sf::Color(0,0,0,0));
	}
}

void Menu::Run()
{
	sf::Event menuEvent;
	bool runMenu = true;
	
	while (runMenu)
	{
		m_window->pollEvent(menuEvent);

		if (menuEvent.type == sf::Event::Closed)
			return;

		Menu::Update();


		if (menuEvent.key.code == sf::Keyboard::Return)
		{
			for (std::list<Data>::iterator it = dataList.begin(); it != dataList.end(); ++it)
			{
				if ((*it).menudata.GetDataType() == MenuData::DataType::GridSize)
					finalGridSize = (*it).menudata.GetSelection(1);
				if ((*it).menudata.GetDataType() == MenuData::DataType::GridWidth)
					finalGridWidth = (*it).menudata.GetSelection(1);
				if ((*it).menudata.GetDataType() == MenuData::DataType::MinHeight)
					finalMinHeight = (*it).menudata.GetSelection(1.0f);
				if ((*it).menudata.GetDataType() == MenuData::DataType::MaxHeight)
					finalMaxHeight = (*it).menudata.GetSelection(1.0f);
				if ((*it).menudata.GetDataType() == MenuData::DataType::Smooth)
					finalSmoother = (*it).menudata.GetSelection(HeightMap::Smoother::aggressiveSmoothing);
				if ((*it).menudata.GetDataType() == MenuData::DataType::SmoothCount)
					finalSmoothCount = (*it).menudata.GetSelection(1);
				if ((*it).menudata.GetDataType() == MenuData::DataType::NumberGen)
					finalNumberGen = (*it).menudata.GetSelection(HeightMap::RandomNumber::logNormalDistribution);
				if ((*it).menudata.GetDataType() == MenuData::DataType::Mean)
					finalNumberGenFirst = (*it).menudata.GetSelection(1.0f);
				if ((*it).menudata.GetDataType() == MenuData::DataType::Range)
					finalNumberGenSecond = (*it).menudata.GetSelection(1.0f);
			}
			finalOffset = 1.2f;
			return;
		}

		if (menuEvent.type == sf::Event::MouseWheelMoved)
		{
			if (highlightedData != NULL)
			{
				for (std::list<Data>::iterator it = dataList.begin(); it != dataList.end(); ++it)
				{
					if ((*it).isHighlighted)
					{
						if (menuEvent.mouseWheel.delta > 0)
						{
							(*it).text.setString(std::string((*it).menudata.GetNextValue()));
						}
						else if (menuEvent.mouseWheel.delta < 0)
						{
							(*it).text.setString(std::string((*it).menudata.GetPrevValue()));
						}
					}
				}
				{
					
				}
			}
		}

		Menu::Draw();
	}
}

void Menu::HighlightSelection()
{

}

void Menu::Draw()
{
	m_window->clear();
	m_window->pushGLStates();
	m_window->draw(backgroundSprite);
	
	for (sf::Text item : textList)
	{
		m_window->draw(item);

	}
	for (std::list<Data>::iterator it = dataList.begin(); it != dataList.end(); ++it)
	{
		(*it).text.setString((*it).menudata.GetCurrentValue());
		m_window->draw((*it).text);
	}

	m_window->draw(highlight);
	m_window->popGLStates();

	m_window->display();
}

HeightMap & Menu::GetHeightMap()
{
	return heightmap;
}


Menu::~Menu()
{
}
