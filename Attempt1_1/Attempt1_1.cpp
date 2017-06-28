#include <glm\glm.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "GL\glew.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/OpenGL.hpp"
#include "Camera.h"
#include "QuatCamera.h"
#include "HeightMap.h"
#include "Overlay.h"
#include "Menu.h"
#include "TGUIMenu.h"
#include "Skybox.h"
#include "Ocean.h"


int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;			// Request a 24-bit depth buffer
    settings.stencilBits = 8;           // Request a 8 bits stencil buffer
    settings.antialiasingLevel = 2;		// Request 2 levels of antialiasing

    // Use SFML to handle the window for us
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Height Map Flight Sim", sf::Style::Close, settings);

    // initialise the menu (self contained render loop)
    TGUIMenu menu(window);
    // if the menu is closed, also close the window
    if (!menu.Run())
        window.close();

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
    GLdouble zFar = 1000000.0f;

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
        menu.finalMaxHeight,								// position Y (equal to maximum height of heightmap)
        -(menu.finalGridSize / 2 + 1),						// position Z (away from the heightmap by half the size of heightmap)
        tan(menu.finalMaxHeight / menu.finalGridSize / 2),	// rotation X (angle towards centre of map)
        180.0f,												// rotation Y (turn around)
        0.01f												// rotation Z (initialise a non-zero value)
    );

    // clock for use with key lockout
    sf::Clock keyTimeout;

    // determines if movement is on or off at start
    bool moveForward = true;

    // bool to identify the first run-through of the game loop
    bool firstLoop = true;

    // value for holding distance travelled for each tick
    float flightSpeed = 100.0f;
    
    // Start game loop
    while (window.isOpen())
    {
        // only update the camera if the window has focus (no uncontrolled flying if untabbed)
        if (window.hasFocus())
        {
            // do input checks outside of sf event to avoid 'first key stutter'
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
            {
                if (flightSpeed < 1000.0f)
                    flightSpeed += 10.0f;
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
            {
                if (flightSpeed > 100.1f)
                    flightSpeed -= 10.0f;
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
            {
                quatCamera.Yaw(-1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
            {
                quatCamera.Yaw(1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
            {
                quatCamera.Pitch(1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
            {
                quatCamera.Pitch(-1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
            {
                quatCamera.Roll(-1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
            {
                quatCamera.Roll(1.0f, true);
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)))
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
        }
        
        // disable the first-loop functions
        firstLoop = false;

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
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
        gameOverlay.Update(mat[12], mat[13], mat[14], flightSpeed);
    
        // draw the game UI
        window.pushGLStates();
        gameOverlay.Draw(window);
        window.popGLStates();


        // display all the draws
        window.display();
    }
    return EXIT_SUCCESS;
};