#pragma warning( disable : 4244 4101)  

#include <glm\glm.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "GL\glew.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/OpenGL.hpp"
#include "QuatCamera.h"
#include "HeightMap.h"
#include "Overlay.h"
#include "Menu.h"
#include "Skybox.h"
#include "Ocean.h"

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;			// Request a 24-bit depth buffer
    settings.stencilBits = 8;           // Request a 8 bits stencil buffer
    settings.antialiasingLevel = 2;		// Request 2 levels of antialiasing

    // Use SFML to handle the window for us
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Height Map Flight Sim", sf::Style::Titlebar, settings);

    // initialise the menu (self contained render loop)
    Menu menu(window);
    // loadup the settings menu
	menu.Run();

    // nicest perspective correction calculations
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //prepare OpenGL surface for HSR
    glClearColor(1.0f, 1.0f, 1.0f, 0.f); // set the background colour

    // Setup a perspective projection & Camera position

    // GL_PROJECTION what we actually see
    glMatrixMode(GL_PROJECTION); // Select the builtin projection matrix
    glLoadIdentity();  // reset the projection matrix by loading the projection identity matrix


    /*---------------------Frustum begin---------------------*/
    GLdouble fovY = 90;
    GLdouble aspect = float(float(window.getSize().x) / float(window.getSize().y)); // 16:9 ascpect ratio = 1.77
    GLdouble zNear = 1.0f;
    GLdouble zFar = 250000.0f;

    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;
    
    // define a perspective projection
    glFrustum(-fW, fW, -fH, fH, zNear, zFar); // multiply the set matrix; by a perspective matrix

    /*---------------------Frustum end----------------------*/

    // initialise skybox
    Skybox skybox;
    // run the skybox setup (texture bindings)
    skybox.Setup(100000.0f);

    // setup the heightmap with the chosen values from the menu
    HeightMap heightmap(
        menu.finalGridSize + 1,			// grid size + 1 to make it odd (make center point a value, not between two)
        menu.finalGridWidth,			// distance between each vertex
        menu.finalMinHeight,			// lowest possible Y coordinate
        menu.finalMaxHeight,			// highest posible Y coordinate
        menu.finalSmoother,				// smoothing method
        menu.finalSmoothCount,			// number of times to run the smoothing method
        menu.finalNumberGen,			// random generation method
        menu.finalNumberGenFirst,		// first input parameter for number generation (mean)
        menu.finalNumberGenSecond,		// second input parameter for number generation (sigma)
        menu.finalOffset				// amount of offset between 'levels' in diamond-square algorithm
    );
    heightmap.GenerateHeightMap();

    Ocean ocean;
    ocean.Setup(skybox.GetSize(), heightmap.GetOceanPoint());
    
    // initialise the game overlay
    Overlay gameOverlay;

    // run the setup method (place the overlay elements and define the properties
    gameOverlay.Setup(window);
    
    // initialise the (new) camera class with initial position/orientation values
    QuatCamera quatCamera(
        0.0f,												// position X
        350.0,												// position Y (equal to maximum height of heightmap)
        -(menu.finalGridSize / 2 + 1),						// position Z (away from the heightmap by half the size of heightmap)
        0.0f,												// rotation X (angle towards centre of map)
        180.0f,												// rotation Y (turn around)
        0.00f												// rotation Z (initialise a non-zero value)
    );

	gameOverlay.setPitch(quatCamera.GetPitch());
    // clock for use with key lockout
    sf::Clock keyTimeout;

    // determines if movement is on or off at start
    bool moveForward = true;

    // bool to identify the first run-through of the game loop
    bool firstLoop = true;

    // value for holding distance travelled for each tick
    float flightSpeed = 100.0f;
    
	// make cursor invisible
	window.setMouseCursorVisible(false);
	
	// 
	sf::Vector2f windowCenter(window.getSize().x / 2, window.getSize().y / 2);
	
	// pin mouse to center of the window
	sf::Mouse mouse;
	mouse.setPosition(window.mapCoordsToPixel(sf::Vector2f(windowCenter.x, windowCenter.y)), window);
	
	sf::Keyboard::Key accelarate, deccelarate, pitchUp, pitchDown, yawUp, yawDown, rollUp, rollDown, halt;
	accelarate = sf::Keyboard::W;
	deccelarate = sf::Keyboard::S;
	pitchUp = sf::Keyboard::Down;
	pitchDown = sf::Keyboard::Up;
	yawUp = sf::Keyboard::D;
	yawDown = sf::Keyboard::A;
	rollUp = sf::Keyboard::Right;
	rollDown = sf::Keyboard::Left;
	halt = sf::Keyboard::Space;

	float pitch, yaw, roll = 0;


    // Start game loop
    while (window.isOpen())
    {
		// only update the camera if the window has focus (no uncontrolled flying if untabbed)
        if (window.hasFocus())
        {
			int mouseX = sf::Mouse::getPosition(window).x;
			int mouseY = sf::Mouse::getPosition(window).y;

			float mouseDeltaX = mouseX - window.mapCoordsToPixel(windowCenter).x;
			float mouseDeltaY = mouseY - window.mapCoordsToPixel(windowCenter).y;
			
			int maxX = window.getSize().x;
			int maxY = window.getSize().y;


			// restrict mouse to window bounds
			if (mouseX < 0 || mouseY < 0 || mouseX > maxX || mouseY > maxY)
			{
				if (mouseX < 0)
					mouseX = 0;
				else if (mouseX > maxX)
					mouseX = maxX;

				if (mouseY < 0)
					mouseY = 0;
				else if (mouseY > maxY)
					mouseY = maxY;

				sf::Mouse::setPosition(sf::Vector2i(mouseX, mouseY), window);
			}
			
			// use mouse input to move camera
			quatCamera.Roll(mouseDeltaX * 0.002, true);
			quatCamera.Pitch(-(mouseDeltaY * 0.002), true);

            // do input checks outside of sf event to avoid 'first key stutter'
            if ((sf::Keyboard::isKeyPressed(accelarate)))
            {
                if (flightSpeed < 5000.0f)
                    flightSpeed += 5.0f;
            }

            if ((sf::Keyboard::isKeyPressed(deccelarate)))
            {
                if (flightSpeed > 100.1f)
                    flightSpeed -= 5.0f;
            }

            if ((sf::Keyboard::isKeyPressed(yawDown)))
            {
                quatCamera.Yaw(-1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(yawUp)))
            {
                quatCamera.Yaw(1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(pitchDown)))
            {
				mouse.setPosition(window.mapCoordsToPixel(windowCenter), window);
                quatCamera.Pitch(1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(pitchUp)))
            {
				mouse.setPosition(window.mapCoordsToPixel(windowCenter), window);
				quatCamera.Pitch(-1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(rollDown)))
            {
				mouse.setPosition(window.mapCoordsToPixel(windowCenter), window);
				quatCamera.Roll(-1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(rollUp)))
            {
				mouse.setPosition(window.mapCoordsToPixel(windowCenter), window);
				quatCamera.Roll(1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(halt)))
            {
                // lock movement boolean changes for 0.5 seconds upon trigger.
                // (stops key spam negating original key press).
                if (keyTimeout.getElapsedTime().asSeconds() > 0.5)
                {
                    if (moveForward)
                        moveForward = false;
                    else
                        moveForward = true;
                    keyTimeout.restart();
                }
            }

            //processing continual movement
            if (moveForward && !firstLoop)
                quatCamera.MoveForward(flightSpeed, true);

			// reset mouse back to middle of the screen
			//mouse.setPosition(window.mapCoordsToPixel(windowCenter), window);
        }
        
        // disable the first-loop functions
        firstLoop = false;

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::F1)))
				gameOverlay.ToggleDebug();
        }

        //Prepare for drawing
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear z-buffer and set previously selected colour
        glMatrixMode(GL_MODELVIEW);

        glLoadIdentity(); // reset

        // apply camera matrix update
        glMultMatrixf(quatCamera.GetViewMatrixAsArray());

        //
        glewInit();

        // draw the skybox
        window.pushGLStates();
        skybox.Render(quatCamera.GetViewMatrixAsArray());
        window.popGLStates();

        // disable texturing before drawing non-textured objects
        glDisable(GL_TEXTURE_2D);

        window.pushGLStates();
        ocean.Render(quatCamera.GetViewMatrixAsArray());
        window.popGLStates();

        // enable EoF value for triangle strip rendering
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(0xff);
        heightmap.Render();
        
        // update game UI values
        float mat[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		

		glm::vec3 eulers(quatCamera.GetEulerAngles());
		gameOverlay.Update(mat[12], mat[13], mat[14], flightSpeed, eulers[2], eulers[1], eulers[0], quatCamera.GetAlt() - heightmap.GetOceanPoint());
    
        // draw the game UI
        window.pushGLStates();
        gameOverlay.Draw(window);
        window.popGLStates();


        // display all the draws
        window.display();
    }
    return EXIT_SUCCESS;
};