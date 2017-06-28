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
	sf::RenderWindow * m_window;
	sf::Font font;
	sf::Text text_fpsCount, text_xPos, text_yPos, text_zPos, text_speed;
	sf::RectangleShape uiRect;
	sf::Clock fpsClock;

	sf::Texture uiTexture;
	sf::Sprite uiSprite;
	sf::Texture gyroTexture;
	sf::Sprite gyroSprite;
	sf::Text text_pitch, text_yaw, text_roll, /*text_speed,*/ text_alt, 
		data_pitch, data_yaw, data_roll, data_speed, data_alt;
	sf::Text * texts;
};

