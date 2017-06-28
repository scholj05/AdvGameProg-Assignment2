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

	float scalarX = m_window->getSize().x / 100;
	float scalarY = m_window->getSize().y / 100;

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

	if (!gyroTexture.loadFromFile("../resources/gyro.png"))
	{
		printf("Could not load gyro.png");
	}

	gyroSprite.setTextureRect(sf::IntRect(710, 710, 180, 180));
	gyroSprite.setOrigin(gyroSprite.getGlobalBounds().width / 2, gyroSprite.getGlobalBounds().height / 2);
	gyroSprite.setTexture(gyroTexture);
	gyroSprite.setPosition(m_window->getSize().x / 2, m_window->getSize().y/100*94);

	if (!pitchTexture.loadFromFile("../resources/pitch.png"))
	{
		printf("Could not load pitch.png");
	}

	pitchSprite.setTexture(pitchTexture);
	pitchSprite.setOrigin(pitchSprite.getGlobalBounds().width / 2, pitchSprite.getGlobalBounds().height / 2);
	pitchSprite.setScale(0.36, 0.36);
	pitchSprite.setPosition(scalarX * 33.5, float(m_window->getSize().y) * 0.92);

	if (!yawTexture.loadFromFile("../resources/yaw.png"))
	{
		printf("Could not load yaw.png");
	}

	yawSprite.setTexture(yawTexture);
	yawSprite.setOrigin(yawSprite.getGlobalBounds().width / 2, yawSprite.getGlobalBounds().height / 2);
	yawSprite.setScale(0.36, 0.36);
	yawSprite.setPosition(scalarX * 71.75, float(m_window->getSize().y) * 0.92);

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
	text_velocity.setColor(sf::Color::Green);
	text_velocity.setPosition(scalarX * 5, scalarY * 97);
	text_velocity.setString("SPD:");


	text_pitch.setFont(font);
	text_pitch.setCharacterSize(30);
	text_pitch.setColor(sf::Color::Green);
	text_pitch.setPosition(scalarX * 46, scalarY * 87);
	text_pitch.setString("PCH:");

	text_yaw.setFont(font);
	text_yaw.setCharacterSize(30);
	text_yaw.setColor(sf::Color::Green);
	text_yaw.setPosition(scalarX * 45.80, scalarY * 91);
	text_yaw.setString("YAW:");

	text_roll.setFont(font);
	text_roll.setCharacterSize(30);
	text_roll.setColor(sf::Color::Green);
	text_roll.setPosition(scalarX * 46, scalarY * 95);
	text_roll.setString("ROL:");

	text_alt.setFont(font);
	text_alt.setCharacterSize(50);
	text_alt.setColor(sf::Color::Green);
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

	gyroSprite.setRotation(-roll);
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

