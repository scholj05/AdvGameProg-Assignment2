#include "GL\glew.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <sstream>

std::default_random_engine generator;
std::lognormal_distribution<double> normal(0.0, 1.0);

const float piover180 = 0.0174532925f;
float heading;
float xpos;
float ypos;
float zpos;
float flightSpeed = 1;

GLfloat	yaw = 1 * piover180;				// Y Rotation
GLfloat pitch = 0.0f;
GLfloat roll = 0.0f;
GLfloat	z = 0.0f;				// Depth Into The Screen

class UI
{
private:

	sf::Font font;
	sf::Text text_fpsCount, text_xPos, text_yPos, text_zPos, text_speed;
	sf::RectangleShape uiRect;
	sf::Clock fpsClock;
public:
	UI::UI()
	{
	}

	void UI::Setup(sf::RenderWindow &window)
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

	void UI::Update(float x, float y, float z, float speed)
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

	void UI::Draw(sf::RenderWindow &window)
	{
		window.draw(uiRect);
		window.draw(text_fpsCount);
		window.draw(text_xPos);
		window.draw(text_yPos);
		window.draw(text_zPos);
		window.draw(text_speed);
	}
};

class Mesh
{
	private:
		std::vector<GLfloat> vectorBuffer;
		std::vector<GLint> facesIndex;

		void square(int x, int y, int size, float* heightmap[], float displacement)
		{
			int half_size = size / 2;

			float top_center = 0;
			if (y - half_size >= 0)
			{
				top_center = heightmap[x + half_size][y - half_size];
			}
			float top_left = heightmap[x][y];
			float top_right = heightmap[x][y + size - 1];
			float center = heightmap[x + half_size][y + half_size];


			float bottom_left = heightmap[x + size - 1][y];
			float bottom_right = heightmap[x + size - 1][y + size - 1];

			float a = top_left + center + top_right / 3.0f;
			float b = top_left + center + bottom_left / 3.0f;
			float c = top_right + center + bottom_right / 3.0f;
			float d = bottom_left + center + bottom_right / 3.0f;

			//  a
			//b   c
			//  d
			heightmap[x][y + half_size] = a + normal(generator)*displacement; // a
			heightmap[x + half_size][y] = b + normal(generator)*displacement; // b
			heightmap[x + half_size][y + size - 1] = c + normal(generator)*displacement; // c
			heightmap[x + size - 1][y + half_size] = d + normal(generator)*displacement; // d
		}

		void diamond(int x, int y, int size, float* heightmap[], float displacement)
		{
			int half_size = size / 2;

			float top_left = heightmap[x][y];
			float bottom_left = heightmap[x + size - 1][y];
			float top_right = heightmap[x][y + size - 1];
			float bottom_right = heightmap[x + size - 1][y + size - 1];

			float average = (top_left + bottom_left + top_right + bottom_right) / 4.0f;

			heightmap[x + half_size][y + half_size] = average + normal(generator)*displacement;

			square(x, y, size, heightmap, displacement);

			if (half_size > 1)
			{
				diamond(x, y, half_size + 1, heightmap, displacement / 3.0f);
				diamond(x + half_size, y, half_size + 1, heightmap, displacement / 3.0f);
				diamond(x, y + half_size, half_size + 1, heightmap, displacement / 3.0f);
				diamond(x + half_size, y + half_size, half_size + 1, heightmap, displacement / 3.0f);
			}
		}
	public:
		std::pair <int, int> convert1Dto2D(int id, int sides)
		{
			int row = id / sides;
			int col = id % sides;

			return std::pair<int, int>(row, col);
		}

		int convert2Dto1D(int row, int col, int sides)
		{
			return (row * sides) + col;
		}
		Mesh()
		{
			int sides = pow(2, 8) + 1;

			// note this is not the most efficient way of allocating a 2D array but it does allow us to use the
			// already familiar subscript operators as usual

			float** heightmap = new float*[sides];
			for (int i = 0; i < sides; ++i)
			{
				heightmap[i] = new float[sides];
			}

			heightmap[0][0] = 0.0001;
			heightmap[sides - 1][0] = 0.0001;
			heightmap[0][sides - 1] = 0.0001;
			heightmap[sides - 1][sides - 1] = 0.0001;

			// left-right
			diamond(0, 0, sides, heightmap, 1.0f);

			//for (int i = 0; i < sides; i++)
			//{
			//	for (int j = 0; j < sides; j++)
			//	{
			//		heightmap[i][j] = 0.0f;
			//	}
			//}

			int v = 0;
			float width = 10.0f;

			float maxWidth = width * sides;

			for (int i = 0; i <sides; i++)
			{
				for (int j = 0; j < sides; j++)
				{
					float x = i*width - (maxWidth /2);
					float y = heightmap[i][j];
					float z = j*width;

					vectorBuffer.push_back(x);
					vectorBuffer.push_back(y);
					vectorBuffer.push_back(z);

					v++;
				}
			}

			facesIndex.push_back(0);

			int limit = sides * sides - 1;

			for (int i = 0; i < sides; i++)
			{
				for (int j = 0; j < sides; j++)
				{
					int vertex = convert2Dto1D(i, j,sides); // little method to calculate the vertex identifier
					int adjacent = vertex + 1;
					int below = vertex + sides;

					if (below <= limit)
					{
						facesIndex.push_back(below);
					}
					if (j == sides - 1) // is this the end of the row?
					{
						facesIndex.push_back(0xff);
					}
					else
					{
						if (adjacent <= limit)
						{
							facesIndex.push_back(adjacent);
						}

					}
				}
			}
		}
		void render()
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe only

			glColor3f(0, 1, 0);

			glEnableClientState(GL_VERTEX_ARRAY);
			
			glVertexPointer(3, GL_FLOAT, 0, &vectorBuffer[0]);

			glDrawElements(GL_TRIANGLE_STRIP, facesIndex.size(), GL_UNSIGNED_INT, &facesIndex[0]);
			
			glDisableClientState(GL_VERTEX_ARRAY);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // wireframe only
		}
};



int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;          // Request a 24-bit depth buffer
	settings.stencilBits = 8;           // Request a 8 bits stencil buffer
	settings.antialiasingLevel = 2; // Request 2 levels of antialiasing

	// Use SFML to handle the window for us
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML OpenGL Mesh", sf::Style::Close, settings);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // nicest perspective correction calculations

	//prepare OpenGL surface for HSR
	//glClearDepth(1.f); // clear the z-buffer/depth buffer completely +1.0f is the furtherest away
	glClearColor(0.3f, 0.3f, 0.3f, 0.f); // set the background colour for when we clear the screen RGBA values in the 0.0 to 1.0 range. This gives us a nice grey background.

	// Setup a perspective projection & Camera position

	// GL_PROJECTION what we actually see
	glMatrixMode(GL_PROJECTION); // Select the builtin projection matrix
	glLoadIdentity();  // reset the projection matrix by loading the projection identity matrix

	GLdouble fovY = 90;
	GLdouble aspect = 1.0f;
	GLdouble zNear = 1.0f; 
	GLdouble zFar = 10000.0f;

	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	float angle = 0;

	// define a perspective projection
	glFrustum(-fW, fW, -fH, fH, zNear, zFar); // multiply the set matrix; by a perspective matrix

	Mesh cube;

	UI gameUI;
	gameUI.Setup(window);


	// Start game loop
	while (window.isOpen())
	{
		xpos -= (float)sin(heading*piover180) * flightSpeed;
		ypos -= (float)sin(pitch * piover180) * flightSpeed;
		zpos -= (float)cos(heading*piover180) * flightSpeed;

		/// do input checks outside of sf event to avoid 'first key stutter'
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
		{
			if (flightSpeed < 5.0f)
				flightSpeed += 0.01f;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
		{
			heading += 1.0f;
			yaw = heading;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
		{
			heading -= 1.0f;
			yaw = heading;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			if (flightSpeed > 1.01f)
				flightSpeed -= 0.01f;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
		{
			pitch += 0.5f;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
		{
			pitch -= 0.5f;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
		{
			roll += 0.5f;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
		{
			roll -= 0.5f;
		}


		/// keep forward movement constant
		//z += zSpeed;
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseWheelMoved)
			{
				z = z + (event.mouseWheel.delta*5.0f);
			}
		}

		//Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear z-buffer and set previously selected colour

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity(); // reset

		GLfloat x_m, y_m, z_m, u_m, v_m;
		GLfloat xtrans = -xpos;
		GLfloat ztrans = -zpos;
		GLfloat ytrans = -ypos;

		glRotatef(pitch, 1.0f, 0, 0);
		glRotatef(360.0f - yaw, 0, 1.0f, 0);
		glRotatef(roll, 0, 0, 1.0f);
		printf("%f\n", roll);

		glTranslatef(xtrans, ytrans, ztrans);

		glewInit();

		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xff);
		
		glTranslatef(0.f, 0.f, z);

		cube.render();

		float mat[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		gameUI.Update(mat[12], mat[13], mat[14], flightSpeed);
		

		window.pushGLStates();
		gameUI.Draw(window);
		window.popGLStates();

		window.display();


	}

	return EXIT_SUCCESS;
}