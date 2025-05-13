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

MetaBlocks::MetaBlocks(int in, int im, int ib) : n(in), m(im), b(ib) {}

void MetaBlocks::initialize() {
    grid.resize(n, vector<int>(m, 0));
    // There are always only 3 states
    states.resize(3, vector<vector<vector<bool>>>(b, vector<vector<bool>>(b, vector<bool>(b, false))));
    // Initialize the states
    for (int i = 0; i < b; i++) {
        states[0][i][0][0] = true;
        states[1][0][i][0] = true;
        states[2][0][0][i] = true;
    }
    // There should be 12 transitions we can make (4 buttons for each of the 3 states)
    // Put these in order from ->, <-, ^, v
    mp[0] = {{1, 0}, {1, 1}, {2, 2}, {2, 3}};
    mp[1] = {{0, 4}, {0, 5}, {1, 6}, {1, 7}};
    mp[2] = {{2, 8}, {2, 9}, {0, 10}, {0, 11}};
    // Initialize the offsets
    offset.resize(12);
    offset[0] = {1, 0};
    offset[1] = {-b, 0};
    offset[2] = {0, 1};
    offset[3] = {0, -b};
    offset[4] = {b, 0};
    offset[5] = {-1, 0};
    offset[6] = {0, 1};
    offset[7] = {0, -1};
    offset[8] = {1, 0};
    offset[9] = {-1, 0};
    offset[10] = {0, b};
    offset[11] = {0, -1};
}

string MetaBlocks::getState() {
    // string together the state, the current position, and the buttons that are pressed:
    string sol = to_string(state) + "/" + to_string(currPos.first) + "," + to_string(currPos.second) + "/";
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i]) {
            sol += to_string(i) + "/";
        }
    }
    return sol;
}

void MetaBlocks::loadState(string stateString) {
    vector<int> stateVec;
    int currNum = 0;
    for (int i = 0; i < stateString.size(); i++) {
        if (stateString[i] == '/' || stateString[i] == ',') {
            stateVec.push_back(currNum);
            currNum = 0;
        }
        else {
            currNum *= 10;
            currNum += stateString[i] - '0';
        }
    }
    state = stateVec[0];
    currPos = {stateVec[1], stateVec[2]};
    std::fill(buttons.begin(), buttons.end(), 0);
    for (int i = 3; i < stateVec.size(); i++) {
        buttons[stateVec[i]] = true;
    }
}

bool MetaBlocks::checkWin(int x, int y) {
    if (state == 0 && x == end.first && y == end.second) {
        return true;
    }
    return false;
}

bool MetaBlocks::checkValid(int x, int y) {
    // Check if the current position is out of bounds
    if (x < 0 || x >= n || y < 0 || y >= m) {
        return false;
    }
    bool firstHalf, secondHalf;
    switch (state)
    {
    case 0:
        if (grid[x][y] == 4 || grid[x][y] == 0 || (grid[x][y] < 0 && !buttons[-grid[x][y]])) {
            return false;
        }
        break;
    case 1:
        firstHalf = false;
        secondHalf = false;
        for (int i = 0; i < b; i++) {
            if (grid[x + i][y] == 4) {
                return false;
            }
            if (grid[x + i][y] == 1 || grid[x + i][y] == 2 || grid[x + i][y] == 3 || grid[x + i][y] / 100 == 1 || grid[x + i][y] / 100 == 2 || (grid[x + i][y] < 0 && buttons[-grid[x + i][y]])) {
                if (i < b / 2) {
                    firstHalf = true;
                }
                else {
                    secondHalf = true;
                }
                if (b % 2 && i == b / 2) {
                    firstHalf = true;
                    secondHalf = true;
                    break;
                }
            }
        }
        if (! firstHalf || ! secondHalf) {
            return false;
        }
        break;
    case 2:
        firstHalf = false;
        secondHalf = false;
        for (int i = 0; i < b; i++) {
            if (grid[x][y + i] == 4) {
                return false;
            }
            if (grid[x][y + i] == 1 || grid[x][y + i] == 2 || grid[x][y + i] == 3 || grid[x][y + i] / 100 == 1 || grid[x][y + i] / 100 == 2 || (grid[x][y + i] < 0 && buttons[-grid[x][y + i]])) {
                if (i < b / 2) {
                    firstHalf = true;
                }
                else {
                    secondHalf = true;
                }
                if (b % 2 && i == b / 2) {
                    firstHalf = true;
                    secondHalf = true;
                    break;
                }
            }
        }
        if (! firstHalf || ! secondHalf) {
            return false;
        }
        break;
    break;        
    default:
        break;
    }
    return true;
}

void MetaBlocks::loadGrid(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    vector<vector<int>> tempGrid; // Temporary grid to store values

    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        string value;

        // Split the line by commas and convert to integers
        while (getline(ss, value, ',')) {
            try {
                if (stoi(value) == 2) {
                    start = {tempGrid.size() + b, row.size() + b};
                    currPos = start;
                }
                else if (stoi(value) == 3) {
                    end = {tempGrid.size() + b, row.size() + b};   
                }
                row.push_back(stoi(value));
            } catch (const invalid_argument& e) {
                cerr << "Error: Invalid data in file " << filename << endl;
                file.close();
                return;
            }
        }
        tempGrid.push_back(row);
    }

    file.close();
    // Update grid dimensions
    n = tempGrid.size();
    m = n > 0 ? tempGrid[0].size() : 0;
    // Validate that all rows have the same number of columns
    for (const auto& row : tempGrid) {
        if (row.size() != m) {
            cerr << "Error: Inconsistent row sizes in file " << filename << endl;
            return;
        }
    }
    // Here's the problem. Given that we know b, we need to pad the grid so we can never
    // accidentally fall out of bounds. That will cause a lot of headache. So pad some zero rows
    n += 2 * b;
    m += 2 * b;
    vector<vector<int>> newGrid(n, vector<int>(m, 0));
    for (int i = b; i < n - b; i++) {
        for (int j = b; j < m - b; j++) {
            newGrid[i][j] = tempGrid[i - b][j - b];
        }
    }
    grid = newGrid; // Assign the temporary grid to the class grid
    // We have to propagate the transporters
    // How many transporters are there?
    int numTransporters = 0;
    vector<pair<int, int>> transporterIndices;
    int numButtons = 0;
    vector<tuple<int, int, int>> bridgeIndices;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] / 100 == 1) {
                numTransporters++;
                transporterIndices.push_back({i, j});
            }
            else if (grid[i][j] / 100 == 2) {
                numButtons++;
            }
            else if (grid[i][j] < 0) {
                bridgeIndices.push_back({-grid[i][j], i, j});
            }
        }
    }
    buttons.resize(numButtons + 1, false);
    buttonMap.resize(numButtons + 1);
    for (auto [k, i, j] : bridgeIndices) {
        buttonMap[k].push_back({i, j});
    }
    transporters.resize(numTransporters);
    int val;
    for (auto [i, j] : transporterIndices) {
        val = grid[i][j] % 100;
        if (val % 2) {
            transporters[val - 1] = {i, j};
        }
        else {
            transporters[val + 1] = {i, j};
        }
    }
    cout << "Grid loaded successfully from " << filename << endl;
}

void MetaBlocks::saveGrid(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            file << grid[i][j] << " ";
        }
        file << endl;
    }

    file.close();
    cout << "Grid saved successfully to " << filename << endl;
}
