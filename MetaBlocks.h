#ifndef METABLOCKS_H
#define METABLOCKS_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <fstream>
//#include <SFML/Window.hpp>
//#include <SFML/OpenGL.hpp>
//#include <SFML/Graphics.hpp>
#include <GL/glu.h>
#include <sstream>
#include <cmath>
#include <unordered_set>
#include <random>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class MetaBlocks
{
public:
    mt19937 gen;
    const int CELL_SIZE = 300;
    int n = 9;
    int m = 9;
    int b = 2;
    int elementIntensity = 2;
    int difficulty = 5;
    int puzzleNum = 0;
    pair<int, int> start;
    pair<int, int> currPos;
    pair<int, int> end;
    int state;
    int accept = 0;
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
    string optimalSolution;
    // options
    // 0 -- empty
    set<pair<int, int>> zeroIndices;
    // 1 -- filled
    set<pair<int, int>> oneIndices;
    // 2 -- start
    // 3 -- end
    // 4 -- dead
    // 10x -- teleporter(s)--in pairs
    // 20x -- buttons 
    // -x -- button activated block
    // -20x -- button deactivated block
    set<tuple<int, int, int>> otherIndices;
    // We want to set some integers that tell us what TYPE of game it is:
    // 1) numDead
    // 2) numTeleporterPairs
    // 3) numButtons
    // 4) numButtonActivatedBlocks (for each button)
    // 5) numButtonDeactivatedBlocks (for each button)
    // On screen, select b, select grid size, select elementIntensity, select difficulty
    // elementIntensity randomly generates some number of each element
    // 1) User selects the grid size, b, and the element intensity and there's a shuffle option to shuffle the elements
    // 2) A screen to the side shows the STARTING configuration for the game with all of the buttons and panels nicely lined up
    // 3) Clicking start makes it generate the puzzle--then it shows it and you're playing
    // 4) A help button shows what each of the elements mean (always available)
    // 5) On the front should be a tutorial button that walks through a trivial game with each element and has helpful tips.
    // 6) A move counter counts the number of moves you've made and compares it with the optimal number.
    // 7) Infinite moves should be a 0 score and the perfect # of moves should be a 100 score. One formula is 100 * possible / used

    MetaBlocks(int in = 0, int im = 0, int ib = 0, int ielementIntensity = 0, int idifficulty = 0); // Declaration only

    void initialize();
    void initializeMaps();
    void initializeGridAndGridFurniture();
    void setInitializationString(string init);
    void applyIntensity();
    string getState();
    void loadState(string stateString);
    bool checkWin();
    bool checkValid();
    void resetPuzzle();
    string getFullGridString();
    string getJSONString();
    void setFullGridString(string gridString);


    void backtrack(unordered_map<string, int>& stateStrings, vector<int>& sol, int t);
    void move(int moveId, bool undo = false);
    void activateButton(bool deactivate = false);
    void transport();
    bool checkValidSolution();
    pair<int, int> getNumOptimalSolutions();
    pair<int, int> showOptimalSolutions();
    vector<vector<int>> basicMCMove();
    void undoBasicMCMove(vector<vector<int>>& swapPairs);
    void swapBlocks(tuple<int, int, int>& p1, tuple<int, int, int>& p2, vector<vector<int>>& sol);
    void resetBlocks(vector<int>& p);
    void updateIndices();
    double getEnergy();
    double MCStep(double energy, double temperature);
    void MCSimulation(int numSteps, double temperature);

    string EvolutionAlgorithm(string init, int minPopulation, int maxPopulation);
    void saveJSONFILE(string outputDir, string inputString, string gridString);
    void loadJSONFILE(const string& fileName);
    void printGrid();

//    string eventTypeToString(sf::Event::EventType type);
    void loadGrid(const string& filename);
    void saveGrid(const string& filename);
//    void drawCube(float x, float y, float z, float length = 1.0f, float width = 1.0f, float height = 1.0f, tuple<float, float, float> color = {0.8f, 0.1f, 0.1f});
//    void drawCubeWithText(sf::RenderWindow& renderWindow, float x, float y, float z, const string& text, float length = 1.0f, float width = 1.0f, float height = 0.2f, tuple<float, float, float> color = {0.8f, 0.1f, 0.1f});
//    void drawCubeWithDisk(float x, float y, float z, float length = 1.0f, float width = 1.0f, float height = 0.2f, 
//                          tuple<float, float, float> cubeColor = {0.8f, 0.1f, 0.1f}, 
//                          tuple<float, float, float> diskColor = {0.1f, 0.8f, 0.1f});
//    void drawWinState(float x, float y, float z, float length = 1.0f, float width = 1.0f, float height = 0.2f, tuple<float, float, float> color = {0.1f, 0.8f, 0.1f});
//    void drawGrid(sf::RenderWindow& renderWindow);
//    void drawState(int x, int y, tuple<float, float, float> color = {0.1f, 0.1f, 0.8f});
//    int view();
};

#endif