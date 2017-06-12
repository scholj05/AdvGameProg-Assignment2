#pragma once
#include "SFML\Graphics.hpp"
#include <list>
class Menu
{
public:
	Menu(sf::RenderWindow &window);

	void Update();

	~Menu();

	bool Menu::Run();

	void Menu::HighlightSelection();

	void Menu::Draw();

private:
	sf::RenderWindow * m_window;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	sf::Font textFont;

	//text objects for the titles of data
	sf::Text txt_title, txt_gridSize, txt_gridWidth, txt_heightMin, txt_heightMax, txt_numberGen, txt_numberGenFirst, txt_numberGenSecond, txt_smoothing;
	
	//text objects for the data
	sf::Text data_gridSize, data_gridWidth, data_heightMin, data_heightMax, data_numberGen, data_numberGenFirst, data_numberGenSecond, data_smoothingCount, data_smoothingType;

	std::list<sf::Text> textList;

	sf::RectangleShape highlight;

	
};

