/*
*	This file and its contents were written by the author, unless stated otherwise.
*	The code is in no way guaranteed to be in working condition,
*	nor does the author claim any responsibility for negative outcomes of running
*	the contents of this file.
*  Any use of this file must be done with the permission of the author.
*
*	@author		Jesse Schollitt
*   @Co-author  Aquila Halpe
*/

#include "Overlay.h"



Overlay::Overlay()
{
}

void Overlay::Setup(sf::RenderWindow &window)
{
	m_window = &window;

	debug = false;
	
	if (!font.loadFromFile("../resources/DS-DIGIT.TTF"))
	{
		printf("error loading font 'DS-DIGIT.TTF'");
	}	

	uiRect.setSize(sf::Vector2f((float)window.getSize().x / 10, (float)window.getSize().y / 10));
	uiRect.setPosition(0, 0);
	uiRect.setFillColor(sf::Color::White);

	int scalarX = m_window->getSize().x / 100;
	int scalarY = m_window->getSize().y / 100;

	float xStart = uiRect.getPosition().x;
	float yStart = uiRect.getPosition().y;
	unsigned int xPadding = unsigned int(uiRect.getSize().x / 10);
	unsigned int yPadding = unsigned int(uiRect.getSize().y / 9);

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

	if (!gyroTexture.loadFromFile("../resources/gyro.png"))
	{
		printf("Could not load gyro.png");
	}

	gyroSprite.setTextureRect(sf::IntRect(710, 710, 180, 180));
	gyroSprite.setOrigin(gyroSprite.getGlobalBounds().width / 2, gyroSprite.getGlobalBounds().height / 2);
	gyroSprite.setTexture(gyroTexture);
	gyroSprite.setPosition(float(m_window->getSize().x / 2), float(m_window->getSize().y/100*94));

	if (!pitchTexture.loadFromFile("../resources/pitch.png"))
	{
		printf("Could not load pitch.png");
	}

	pitchSprite.setTexture(pitchTexture);
	pitchSprite.setOrigin(pitchSprite.getGlobalBounds().width / 2, pitchSprite.getGlobalBounds().height / 2);
	pitchSprite.setScale(float(scalarY*0.152/3), float(scalarY*0.152 / 3));
	pitchSprite.setPosition(float(scalarX * 33.5), float(m_window->getSize().y) * 0.92f);

	if (!yawTexture.loadFromFile("../resources/yaw.png"))
	{
		printf("Could not load yaw.png");
	}

	yawSprite.setTexture(yawTexture);
	yawSprite.setOrigin(yawSprite.getGlobalBounds().width / 2, yawSprite.getGlobalBounds().height / 2);
	yawSprite.setScale(float(scalarY*0.152 / 3), float(scalarY*0.152 / 3));
	yawSprite.setPosition(scalarX * 71.75, float(m_window->getSize().y) * 0.92f);

	if (!uiTexture.loadFromFile("../resources/UI.png"))
	{
		printf("Could not load UI.png");
	}
	
	uiSprite.setTexture(uiTexture);
	uiSprite.setScale(
		m_window->getSize().x / uiSprite.getLocalBounds().width,
		m_window->getSize().y / uiSprite.getLocalBounds().height
	);


	text_velocity.setFont(font);
	text_velocity.setCharacterSize(m_window->getSize().y*0.065);
	text_velocity.setFillColor(sf::Color::Green);
	text_velocity.setPosition(scalarX * 5, scalarY * 97);
	text_velocity.setString("SPD:");

	text_alt.setFont(font);
	text_alt.setCharacterSize(m_window->getSize().y*0.065);
	text_alt.setFillColor(sf::Color::Green);
	text_alt.setPosition(scalarX * 82, scalarY * 97);
	text_alt.setString("ALT:");

}

void Overlay::Update(float x, float y, float z, float speed, float pitch, float yaw, float roll, float alt)
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

	text_velocity.setString("SPD: " + (sf::String(speedBuffer.str())));

	std::ostringstream pchBuffer;
	pchBuffer << pitch;
	text_pitch.setString("PCH: " + (sf::String(pchBuffer.str())));

	std::ostringstream yawBuffer;
	yawBuffer << yaw;
	text_yaw.setString("YAW: " + (sf::String(yawBuffer.str())));

	std::ostringstream rolBuffer;
	rolBuffer << roll;
	text_roll.setString("ROL: " + (sf::String(rolBuffer.str())));

	std::ostringstream altBuffer;
	altBuffer << alt;
	text_alt.setString("ALT: " + (sf::String(altBuffer.str())));

	gyroSprite.setRotation(roll);
	pitchSprite.setRotation(pitch);
	yawSprite.setRotation(yaw);
}

void Overlay::Draw(sf::RenderWindow &window)
{
	if (debug)
	{
		window.draw(uiRect);
		window.draw(text_fpsCount);
		window.draw(text_xPos);
		window.draw(text_yPos);
		window.draw(text_zPos);
		window.draw(text_speed);
	}
	window.draw(gyroSprite);
	window.draw(pitchSprite);
	window.draw(yawSprite);
	window.draw(uiSprite);
	window.draw(text_velocity);
	//window.draw(text_pitch);
	//window.draw(text_yaw);
	//window.draw(text_roll);
	window.draw(text_alt);
}

void Overlay::ToggleDebug()
{
	if (debug)
		debug = false;
	else
		debug = true;
}

Overlay::~Overlay()
{
}

void Overlay::setPitch(float p)
{
	pitchSprite.setRotation(p);
}

