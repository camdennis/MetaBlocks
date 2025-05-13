#include "MetaBlocks.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glu.h>
#include <sstream>
#include <cmath>

using namespace std;

// Function to convert event type to string
string MetaBlocks::eventTypeToString(sf::Event::EventType type) {
    switch (type) {
        case sf::Event::Closed:
            return "Closed";
        case sf::Event::Resized:
            return "Resized";
        case sf::Event::LostFocus:
            return "LostFocus";
        case sf::Event::GainedFocus:
            return "GainedFocus";
        case sf::Event::TextEntered:
            return "TextEntered";
        case sf::Event::KeyPressed:
            return "KeyPressed";
        case sf::Event::KeyReleased:
            return "KeyReleased";
        case sf::Event::MouseWheelMoved:
            return "MouseWheelMoved";
        case sf::Event::MouseWheelScrolled:
            return "MouseWheelScrolled";
        case sf::Event::MouseButtonPressed:
            return "MouseButtonPressed";
        case sf::Event::MouseButtonReleased:
            return "MouseButtonReleased";
        case sf::Event::MouseMoved:
            return "MouseMoved";
        case sf::Event::MouseEntered:
            return "MouseEntered";
        case sf::Event::MouseLeft:
            return "MouseLeft";
        case sf::Event::JoystickButtonPressed:
            return "JoystickButtonPressed";
        case sf::Event::JoystickButtonReleased:
            return "JoystickButtonReleased";
        case sf::Event::JoystickMoved:
            return "JoystickMoved";
        case sf::Event::JoystickConnected:
            return "JoystickConnected";
        case sf::Event::JoystickDisconnected:
            return "JoystickDisconnected";
        case sf::Event::TouchBegan:
            return "TouchBegan";
        case sf::Event::TouchMoved:
            return "TouchMoved";
        case sf::Event::TouchEnded:
            return "TouchEnded";
        case sf::Event::SensorChanged:
            return "SensorChanged";
        default:
            return "Unknown Event Type";
    }
}

void MetaBlocks::drawCube(float x, float y, float z, float length, float width, float height, tuple<float, float, float> color) {
    float c1 = get<0>(color);
    float c2 = get<1>(color);
    float c3 = get<2>(color);       

    // Translate the cube to the specified position
    glPushMatrix(); // Save the current transformation matrix
    glTranslatef(x, y, z);

    // Draw the faces of the cube
//        glBegin(GL_QUADS);
    // Bottom
    glLineWidth(lineWidth);
    glBegin(GL_QUADS);
    glColor3f(c1, c2, c3);
    glVertex3f(0.0f, 0.0f, -height);
    glVertex3f(0.0f, width, -height);
    glVertex3f(length, width, -height);
    glVertex3f(length, 0.0f, -height);
    glEnd();
    // Top
    glBegin(GL_QUADS);
    glColor3f(c1, c2, c3);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, width, 0.0f);
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();
    // Left
    glBegin(GL_QUADS);
    glColor3f(c1, c2, c3);
    glVertex3f(0.0f, 0.0f, -height);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, -height);
    glEnd();
    // Right
    glBegin(GL_QUADS);
    glColor3f(c1, c2, c3);
    glVertex3f(0.0f, width, -height);
    glVertex3f(0.0f, width, 0.0f);
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, width, -height);
    glEnd();
    // Front
    glBegin(GL_QUADS);
    glColor3f(c1, c2, c3);
    glVertex3f(0.0f, 0.0f, -height);
    glVertex3f(0.0f, width, -height);
    glVertex3f(0.0f, width, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    // Back
    glBegin(GL_QUADS);
    glColor3f(c1, c2, c3);
    glVertex3f(length, 0.0f, -height);
    glVertex3f(length, width, -height);
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();
    // Draw the edges of the cube
    // Top
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -height);
    glVertex3f(0.0f, width, -height);
    glVertex3f(length, width, -height);
    glVertex3f(length, 0.0f, -height);
    glEnd();
    // Bottom
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, width, 0.0f);
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();
    // Left
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -height);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, -height);
    glEnd();
    // Right
    glBegin(GL_LINE_LOOP);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, width, -height);
    glVertex3f(0.0f, width, 0.0f);
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, width, -height);
    glEnd();
    // Front
    glBegin(GL_LINE_LOOP);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -height);
    glVertex3f(0.0f, width, -height);
    glVertex3f(0.0f, width, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    // Back
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(length, 0.0f, -height);
    glVertex3f(length, width, -height);
    glVertex3f(length, width, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();

    glPopMatrix(); // Restore the previous transformation matrix
}        

void MetaBlocks::drawCubeWithText(sf::RenderWindow& renderWindow, float x, float y, float z, const string& text, float length, float width, float height, tuple<float, float, float> color) {
    // Draw the cube (same as your existing drawCube function)
    drawCube(x, y, z, length, width, height, color);

    // Get the model-view and projection matrices
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Calculate the 3D position of the top center of the cube
    GLdouble screenX, screenY, screenZ;
    gluProject(x + length / 2.0, y + width / 2.0, z + height, modelview, projection, viewport, &screenX, &screenY, &screenZ);

    // SFML's origin is at the top-left, but OpenGL's is at the bottom-left, so flip the Y coordinate
    screenY = viewport[3] - screenY;

    // Render text using SFML
    sf::Font font;
    if (!font.loadFromFile("ARIAL.TTF")) { // Load a font file
        cerr << "Error: Could not load font!" << endl;
        return;
    }

    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(24); // Adjust text size to fit the cube face
    sfText.setFillColor(sf::Color::White);
    sfText.setPosition(screenX, screenY); // Position text at the projected 2D coordinates

    // Use the provided renderWindow to draw the text
    renderWindow.pushGLStates(); // Save OpenGL state
    renderWindow.draw(sfText);   // Draw the text
    renderWindow.popGLStates();  // Restore OpenGL state
}

void MetaBlocks::drawCubeWithDisk(float x, float y, float z, float length, float width, float height, 
                        tuple<float, float, float> cubeColor, 
                        tuple<float, float, float> diskColor) {
    float c1 = get<0>(cubeColor);
    float c2 = get<1>(cubeColor);
    float c3 = get<2>(cubeColor);

    // Draw the cube (same as your existing drawCube function)
    drawCube(x, y, z, length, width, height, cubeColor);

    // Cube faces
    // Draw the disk in the middle of the top face
    GLUquadric* quad = gluNewQuadric();
    glColor3f(get<0>(diskColor), get<1>(diskColor), get<2>(diskColor));
    glPushMatrix();
    glTranslatef(x + length / 2.0f, y + width / 2.0f, 0.02f); // Slightly above the top face to avoid z-fighting
    gluDisk(quad, 0.0f, min(length, width) / 4.0f, 32, 1); // Inner radius = 0, outer radius = 1/4 of cube size
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void MetaBlocks::drawWinState(float x, float y, float z, float length, float width, float height, tuple<float, float, float> color) {
    float c1 = get<0>(color);
    float c2 = get<1>(color);
    float c3 = get<2>(color);       

    // Translate the cube to the specified position
    glPushMatrix(); // Save the current transformation matrix
    glTranslatef(x, y, z);

    // Draw the faces of the cube
//        glBegin(GL_QUADS);
    // Bottom
    glBegin(GL_QUADS);
    glColor3f(c1, c2, c3);
    glVertex3f(0.0f, 0.0f, -height);
    glVertex3f(0.0f, width, -height);
    glVertex3f(length, width, -height);
    glVertex3f(length, 0.0f, -height);
    glEnd();

    glPopMatrix(); // Restore the previous transformation matrix
}        

void MetaBlocks::drawGrid(sf::RenderWindow& renderWindow) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 1 || grid[i][j] == 2 || (grid[i][j] < 0 && buttons[-grid[i][j]])) {
                drawCube(i, j, 0, 1, 1, 0.2, {0.8f, 0.1f, 0.1f});
            }
            else if (grid[i][j] == 4) {
                drawCube(i, j, 0, 1, 1, 0.2, {1.0f, 1.0f, 1.0f});
            }
            else if (grid[i][j] == 3) {
                drawWinState(i, j, 0, 1, 1, 0.2, {0.1f, 0.8f, 0.1f});
            }
            else if (grid[i][j] / 100 == 1) {
                drawCubeWithText(renderWindow, i, j, 0, "t" + to_string((grid[i][j] % 100) / 2 + 1));
            }
            else if (grid[i][j] / 100 == 2) {
                drawCubeWithDisk(i, j, 0); 
            }
        }
    }
}

void MetaBlocks::drawState(int x, int y, tuple<float, float, float> color) {
    float c1 = get<0>(color);
    float c2 = get<1>(color);
    float c3 = get<2>(color);       
    // Draw the cube at the specified position
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < b; j++) {
            for (int k = 0; k < b; k++) {
                if (states[state][k][i][j]) {
                    drawCube(x + i, y + j, k + 1, 1, 1, 1, {c1, c2, c3});
                }
            }
        }
    }
}

int MetaBlocks::view() {
    sf::ContextSettings settings;
    settings.depthBits = 16; // Use 16-bit depth buffer
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * (m - 2 * b), CELL_SIZE * (n - 2 * b)), "3D Block Placer", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    state = 0;

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LESS);    // Specify depth comparison function
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set background color

    float angle = -90.0f; // Rotation angle
    float angle2 = 45.0f; // Rotation angle
    float cameraX = m / 2.0f; // Center horizontally
    float cameraY = (n - 2 * b) / 2.0f - 16.0f; // Center vertically
    float cameraZ = max(n, m) * 1.0f; // Move back to fit the grid

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            pair<int, int> nextState;
            // Handle key presses for camera movement
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Right:
                        nextState = mp[state][2];
                        state = nextState.first;
                        currPos.first += offset[nextState.second].first;
                        currPos.second += offset[nextState.second].second;
                        break;
                    case sf::Keyboard::Left:
                        nextState = mp[state][3];
                        state = nextState.first;
                        currPos.first += offset[nextState.second].first;
                        currPos.second += offset[nextState.second].second;
                        break;
                    case sf::Keyboard::Up:
                        nextState = mp[state][1];
                        state = nextState.first;
                        currPos.first += offset[nextState.second].first;
                        currPos.second += offset[nextState.second].second;
                        break;
                    case sf::Keyboard::Down:
                        nextState = mp[state][0];
                        state = nextState.first;
                        currPos.first += offset[nextState.second].first;
                        currPos.second += offset[nextState.second].second;
                        break;
                    default:
                        break;
                }
                if (! checkValid(currPos.first, currPos.second)) {
                    currPos = start;
                    state = 0;
                }
                if (checkWin(currPos.first, currPos.second)) {
                    cout << "Winner!" << endl;
                }
                int val = grid[currPos.first][currPos.second];
                if (state == 0 && val / 100 == 1) {
                    if (val % 2) {
                        currPos = transporters[val % 100];
                    }
                    else {
                        currPos = transporters[val % 100];
                    }
                }
                if (state == 0 && val / 100 == 2) {
                    buttons[val % 100] = !buttons[val % 100];
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(angle2, (static_cast<double>(m) - 2 * b) / (n - 2 * b), 0.1, 100.0); // Adjust FOV and aspect ratio

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(cameraX, cameraY, cameraZ, // Camera position
                    m / 2.0f, n / 2.0f - 5.0f, 0.0f, // Look at the center of the grid
                    0.0f, 1.0f, 0.0f);        // Up vector

        glRotatef(-30.0f, 1.0f, 0.0f, 0.0f); // Tilt the grid downward
        glRotatef(angle, 0.0f, 0.0f, 1.0f);  // Rotate around the Y-axis

        drawGrid(window);
        drawState(currPos.first, currPos.second);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            cerr << "OpenGL error: " << gluErrorString(err) << endl;
        }    
        window.display();
    }
    return 0;
}