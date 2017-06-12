#pragma once
#include "SFML/Graphics.hpp"
#include <sstream>

class Overlay
{
public:
	Overlay();
	~Overlay();

	void Overlay::Setup(sf::RenderWindow &window);
	void Overlay::Update(float x, float y, float z, float speed);
	void Overlay::Draw(sf::RenderWindow &window);

private:
	sf::Font font;
	sf::Text text_fpsCount, text_xPos, text_yPos, text_zPos, text_speed;
	sf::RectangleShape uiRect;
	sf::Clock fpsClock;
};

