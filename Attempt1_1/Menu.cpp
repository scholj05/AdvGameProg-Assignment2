#include "Menu.h"



Menu::Menu(sf::RenderWindow &window)
{
	m_window = &window;
	
	if (!textFont.loadFromFile("../resources/arial.ttf"))
		printf("error loading font 'arial.ttf'");

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

	highlight.setOutlineColor(sf::Color::White);

	int windowX = m_window->getSize().x;
	int windowY = m_window->getSize().y;

	// setup title texts
	txt_title.setFont(textFont);
	txt_title.setCharacterSize(windowX / 25);
	txt_title.setString("Menu");
	txt_title.setOrigin(txt_title.getGlobalBounds().width / 2, 0.0f);
	txt_title.setPosition(windowX / 2, windowY / 10);
	textList.push_back(txt_title);

	txt_gridSize.setFont(textFont);
	txt_gridSize.setCharacterSize(windowX / 50);
	txt_gridSize.setString("Grid Size");
	txt_gridSize.setOrigin(txt_gridSize.getGlobalBounds().width / 2, 0.0f);
	txt_gridSize.setPosition(windowX / 4, windowY / 5);
	textList.push_back(txt_gridSize);

	txt_gridWidth.setFont(textFont);
	txt_gridWidth.setCharacterSize(windowX / 50);
	txt_gridWidth.setString("Grid Width");
	txt_gridWidth.setOrigin(txt_gridWidth.getGlobalBounds().width / 2, 0.0f);
	txt_gridWidth.setPosition(windowX / 4, windowY / 5 * 2);
	textList.push_back(txt_gridWidth);

	txt_heightMin.setFont(textFont);
	txt_heightMin.setCharacterSize(windowX / 50);
	txt_heightMin.setString("Min Height");
	txt_heightMin.setOrigin(txt_heightMin.getGlobalBounds().width / 2, 0.0f);
	txt_heightMin.setPosition(windowX / 4 - txt_heightMin.getGlobalBounds().width, windowY / 5 * 3);
	textList.push_back(txt_heightMin);

	txt_heightMax.setFont(textFont);
	txt_heightMax.setCharacterSize(windowX / 50);
	txt_heightMax.setString("Max Height");
	txt_heightMax.setOrigin(txt_heightMax.getGlobalBounds().width / 2, 0.0f);
	txt_heightMax.setPosition(windowX / 4 + txt_heightMax.getGlobalBounds().width, windowY / 5 * 3);
	textList.push_back(txt_heightMax);

	txt_numberGen.setFont(textFont);
	txt_numberGen.setCharacterSize(windowX / 50);
	txt_numberGen.setString("Number Generation");
	txt_numberGen.setOrigin(txt_numberGen.getGlobalBounds().width / 2, 0.0f);
	txt_numberGen.setPosition(windowX / 4 * 3, windowY / 5);
	textList.push_back(txt_numberGen);

	txt_numberGenFirst.setFont(textFont);
	txt_numberGenFirst.setCharacterSize(windowX / 50);
	txt_numberGenFirst.setString("Min");
	txt_numberGenFirst.setOrigin(txt_numberGenFirst.getGlobalBounds().width / 2, 0.0f);
	txt_numberGenFirst.setPosition(windowX / 4 * 3 - txt_numberGenFirst.getGlobalBounds().width, windowY / 5 * 2);
	textList.push_back(txt_numberGenFirst);

	txt_numberGenSecond.setFont(textFont);
	txt_numberGenSecond.setCharacterSize(windowX / 50);
	txt_numberGenSecond.setString("Max");
	txt_numberGenSecond.setOrigin(txt_numberGenSecond.getGlobalBounds().width / 2, 0.0f);
	txt_numberGenSecond.setPosition(windowX / 4 * 3 + txt_numberGenSecond.getGlobalBounds().width, windowY / 5 * 2);
	textList.push_back(txt_numberGenSecond);

	txt_smoothing.setFont(textFont);
	txt_smoothing.setCharacterSize(windowX / 50);
	txt_smoothing.setString("Smoothing");
	txt_smoothing.setOrigin(txt_smoothing.getGlobalBounds().width / 2, 0.0f);
	txt_smoothing.setPosition(windowX / 4 * 3, windowY / 5 * 3);
	textList.push_back(txt_smoothing);

	// setup data texts
	data_gridSize.setFont(textFont);
	data_gridSize.setCharacterSize(windowX / 75);
	data_gridSize.setString("32 x 32");
	data_gridSize.setOrigin(data_gridSize.getGlobalBounds().width / 2, 0.0f);
	data_gridSize.setPosition(windowX / 4, windowY / 5 + data_gridSize.getGlobalBounds().height * 3);
	textList.push_back(data_gridSize);

	data_gridWidth.setFont(textFont);
	data_gridWidth.setCharacterSize(windowX / 75);
	data_gridWidth.setString("250");
	data_gridWidth.setOrigin(data_gridWidth.getGlobalBounds().width / 2, 0.0f);
	data_gridWidth.setPosition(windowX / 4, (windowY / 5 * 2) + (data_gridWidth.getGlobalBounds().height * 3));
	textList.push_back(data_gridWidth);
}

void Menu::Update()
{
	
}

bool Menu::Run()
{
	sf::Event menuEvent;
	bool runMenu = true;
	while (runMenu)
	{
		m_window->pollEvent(menuEvent);

		for (sf::Text item : textList)
		{
			if (item.getGlobalBounds().contains(m_window->mapPixelToCoords(sf::Mouse::getPosition())))
			{
				highlight.setPosition(item.getPosition());
				highlight.setOrigin(item.getOrigin());
				highlight.setSize(sf::Vector2f(item.getGlobalBounds().width, item.getGlobalBounds().height));
			}
		}
		if (menuEvent.key.code == sf::Keyboard::Escape)
		{
			return false;
		}

		if (menuEvent.key.code == sf::Keyboard::Return)
		{
			
			return true;
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
	m_window->draw(highlight);
	m_window->popGLStates();

	m_window->display();
}


Menu::~Menu()
{
}
