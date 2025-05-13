#ifndef METABLOCKS_H
#define METABLOCKS_H

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
#include <unordered_set>

using namespace std;

class MetaBlocks
{
public:
    const int CELL_SIZE = 300;
    int n = 9;
    int m = 9;
    int b = 2;
    pair<int, int> start;
    pair<int, int> currPos;
    pair<int, int> end;
    int state;
    vector<vector<int>> grid;
    // We want a map to tell us what states you can reach from a given state
    // We want the map to ALSO tell us the x/y offset state of the block
    unordered_map<int, vector<pair<int, int>>> mp;
    // We actually don't care where most of the blocks, just the ones that touch the bottom
    // Index into states to get the state you want. Index into the state to get the LAYER you want
    // Index into the layer to get the boolean value if it's there or not. For rendering, we need all of this
    // For computation, we just need the first layer.
    vector<vector<vector<vector<bool>>>> states;
    // So for each layer, we can find the x/y offset for transitions between states
    // Get the offset you want by indexing into the offset vector.
    vector<pair<int, int>> offset;
    // We want to create a transporter array. They elements tell you what index to transport
    // to and the indices of the array come in pairs and transport to each other.
    // A transport pad starts with 1 and has 3 digits st t / 100 == 1.
    vector<pair<int, int>> transporters;
    // We want buttons
    vector<bool> buttons;
    // Which elements are affected by the buttons?
    vector<vector<tuple<int, int, bool>>> buttonMap;
    float lineWidth = 4.0f;

    MetaBlocks(int in, int im, int ib); // Declaration only

    void initialize();
    string getState();
    void loadState(string stateString);
    bool checkWin();
    bool checkValid();
    void resetPuzzle();

    void backtrack(unordered_map<string, int>& stateStrings, vector<int>& sol, int t);
    void move(int moveId, bool undo = false);
    void activateButton(bool deactivate = false);
    void transport();
    pair<int, int> getNumOptimalSolutions();

    string eventTypeToString(sf::Event::EventType type);
    void loadGrid(const string& filename);
    void saveGrid(const string& filename);
    void drawCube(float x, float y, float z, float length = 1.0f, float width = 1.0f, float height = 1.0f, tuple<float, float, float> color = {0.8f, 0.1f, 0.1f});
    void drawCubeWithText(sf::RenderWindow& renderWindow, float x, float y, float z, const string& text, float length = 1.0f, float width = 1.0f, float height = 0.2f, tuple<float, float, float> color = {0.8f, 0.1f, 0.1f});
    void drawCubeWithDisk(float x, float y, float z, float length = 1.0f, float width = 1.0f, float height = 0.2f, 
                          tuple<float, float, float> cubeColor = {0.8f, 0.1f, 0.1f}, 
                          tuple<float, float, float> diskColor = {0.1f, 0.8f, 0.1f});
    void drawWinState(float x, float y, float z, float length = 1.0f, float width = 1.0f, float height = 0.2f, tuple<float, float, float> color = {0.1f, 0.8f, 0.1f});
    void drawGrid(sf::RenderWindow& renderWindow);
    void drawState(int x, int y, tuple<float, float, float> color = {0.1f, 0.1f, 0.8f});
    int view();
};

#endif