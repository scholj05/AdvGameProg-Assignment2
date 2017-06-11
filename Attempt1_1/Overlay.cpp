#include "Overlay.h"



Overlay::Overlay()
{
}

void Overlay::Setup(sf::RenderWindow &window)
{
	if (!font.loadFromFile("../resources/arial.ttf"))
	{
		printf("error loading font 'arial.ttf'");
	}


	uiRect.setSize(sf::Vector2f((float)window.getSize().x / 10, (float)window.getSize().y / 10));
	uiRect.setPosition(0, 0);
	uiRect.setFillColor(sf::Color::White);

	float xStart = uiRect.getPosition().x;
	float yStart = uiRect.getPosition().y;
	float xPadding = uiRect.getSize().x / 10;
	float yPadding = uiRect.getSize().y / 9;

	text_fpsCount.setFont(font);
	text_xPos.setFont(font);
	text_yPos.setFont(font);
	text_zPos.setFont(font);
	text_speed.setFont(font);

	text_fpsCount.setCharacterSize(yPadding);
	text_xPos.setCharacterSize(yPadding);
	text_yPos.setCharacterSize(yPadding);
	text_zPos.setCharacterSize(yPadding);
	text_speed.setCharacterSize(yPadding);

	text_fpsCount.setFillColor(sf::Color::Black);
	text_xPos.setFillColor(sf::Color::Black);
	text_yPos.setFillColor(sf::Color::Black);
	text_zPos.setFillColor(sf::Color::Black);
	text_speed.setFillColor(sf::Color::Black);

	text_fpsCount.setPosition(xStart + xPadding, yStart + (yPadding + 1));
	text_xPos.setPosition(xStart + xPadding, yStart + (yPadding + 1) * 2);
	text_yPos.setPosition(xStart + xPadding, yStart + (yPadding + 1) * 3);
	text_zPos.setPosition(xStart + xPadding, yStart + (yPadding + 1) * 4);
	text_speed.setPosition(xStart + xPadding, yStart + (yPadding + 1) * 5);
}

void Overlay::Update(float x, float y, float z, float speed)
{
	float FPS = 1 / fpsClock.restart().asSeconds();
	std::ostringstream fpsBuffer;
	fpsBuffer << FPS;
	text_fpsCount.setString("FPS: " + (sf::String(fpsBuffer.str())));

	std::ostringstream xBuffer;
	xBuffer << x;
	text_xPos.setString("X: " + (sf::String(xBuffer.str())));

	std::ostringstream yBuffer;
	yBuffer << y;
	text_yPos.setString("Y: " + (sf::String(yBuffer.str())));

	std::ostringstream zBuffer;
	zBuffer << z;
	text_zPos.setString("Z: " + (sf::String(zBuffer.str())));

	std::ostringstream speedBuffer;
	speedBuffer << speed;
	text_speed.setString("Speed: " + (sf::String(speedBuffer.str())));
}

void Overlay::Draw(sf::RenderWindow &window)
{
	window.draw(uiRect);
	window.draw(text_fpsCount);
	window.draw(text_xPos);
	window.draw(text_yPos);
	window.draw(text_zPos);
	window.draw(text_speed);
}


Overlay::~Overlay()
{
}
