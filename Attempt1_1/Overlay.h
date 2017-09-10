/*
*	This file and its contents were written by the author, unless stated otherwise.
*	The code is in no way guaranteed to be in working condition,
*	nor does the author claim any responsibility for negative outcomes of running
*	the contents of this file.
*  Any use of this file must be done with the permission of the author.
*
*	@author		Jesse Schollitt
*/

#pragma once
#include "SFML/Graphics.hpp"
#include <sstream>

class Overlay
{
public:
	Overlay();
	~Overlay();

	void setPitch(float p);

	void Overlay::Setup(sf::RenderWindow &window);
	void Overlay::Update(float x, float y, float z, float speed, float pitch, float yaw, float roll, float alt);
	void Overlay::Draw(sf::RenderWindow &window);

	void ToggleDebug();

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
	sf::Texture pitchTexture;
	sf::Sprite pitchSprite;
	sf::Texture yawTexture;
	sf::Sprite yawSprite;
	sf::Text text_pitch, text_yaw, text_roll, text_velocity, text_alt;
	bool debug;
};

