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
#include <map>
#include <random>

using namespace std;

MetaBlocks::MetaBlocks(int in, int im, int ib, int ielementIntensity) : n(in), m(im), b(ib), elementIntensity(ielementIntensity), gen(random_device{}()) {}

void MetaBlocks::initialize() {
    grid.resize(n, vector<int>(m, 1));
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
    mp[0] = {{1, 0}, {1, 1}, {2, 2 }, {2, 3 }};
    mp[1] = {{0, 4}, {0, 5}, {1, 6 }, {1, 7 }};
    mp[2] = {{2, 8}, {2, 9}, {0, 10}, {0, 11}};
    // Initialize the offsets
    offset.resize(12);
    offset[0]  =  {1 , 0 };
    offset[1]  =  {-b, 0 };
    offset[2]  =  {0 , 1 };
    offset[3]  =  {0 , -b};
    offset[4]  =  {b , 0 };
    offset[5]  =  {-1, 0 };
    offset[6]  =  {0 , 1 };
    offset[7]  =  {0 , -1};
    offset[8]  =  {1 , 0 };
    offset[9]  =  {-1, 0 };
    offset[10] =  {0 , b };
    offset[11] =  {0 , -1};

    // How many buttons do we need?
    applyIntensity();

}

float vecSum(vector<float>& vec) {
    return accumulate(vec.begin(), vec.end(), 0.0f);
}

void MetaBlocks::applyIntensity() {
    int numButtons = 0, deadSpots = 0, transporterPairs = 0;
    vector<int> numButtonsOn, numButtonsOff;
    map<int, vector<vector<int>>> options = {{1, {{1}, {2}, {3}, {4}}},
        {2, {{2}, {1, 3}, {3, 4}}},
        {3, {{1, 3, 4}, {1, 3}, {1, 4}, {2}}},
        {4, {{2, 3, 4}, {1, 3, 4}, {2, 3}, {2, 4}}},
        {5, {{2, 3, 4}}}
    };
    vector<vector<int>> optionList = options[elementIntensity];
    uniform_int_distribution<int> dist(0, optionList.size() - 1);
    vector<int> puzzleType = optionList[dist(gen)];
    for (int p : puzzleType) {
        cout << p << " ";
    }
    cout << endl;
    vector<int> numButtonsVec = {1, 1, 1, 2, 3};
    vector<int> numButtonsOnVec = {1, 2, 2, 5, 7};
    vector<int> numButtonsOffVec = {1, 1, 2, 5, 7};
    vector<int> numDeadCellsVec = {1, 2, 3, 4, 5};
    vector<int> transporterPairsVec = {1, 1, 2, 3, 3};
    int index = b + 2;
    otherIndices.insert({b, b, 2});
    start = {b, b};
    currPos = start;
    otherIndices.insert({b, 2 * b + 1, 3});
    end = {b, 2 * b + 1};
    vector<tuple<int, int, bool>> buttonMapEntry;
    for (int key : puzzleType) {
        switch (key) {
            case 1:
                numButtons = numButtonsVec[elementIntensity - 1];
                buttons.resize(numButtons + 1, false);
                for (int button = 0; button < numButtons; button++) {
                    otherIndices.insert({index / m + b, index % m + b, 201 + button});
                    index++;
                    for (int j = 0; j < numButtonsOnVec[elementIntensity - 1]; j++) {
                        buttonMapEntry.push_back({index / m + b, index % m + b, false});
                        otherIndices.insert({index / m + b, index % m + b, -1 - button});
                        index++;
                    }
                    buttonMap.push_back(buttonMapEntry);
                    buttonMapEntry.clear();
                }
                break;
            case 2:
                numButtons = numButtonsVec[elementIntensity - 1];
                buttons.resize(numButtons + 1, false);
                for (int button = 0; button < numButtons; button++) {
                    otherIndices.insert({index / m + b, index % m + b, 201 + button});
                    index++;
                    for (int j = 0; j < numButtonsOnVec[elementIntensity - 1]; j++) {
                        buttonMapEntry.push_back({index / m + b, index % m + b, false});
                        otherIndices.insert({index / m + b, index % m + b, -1 - button});
                        index++;
                    }
                    for (int j = 0; j < numButtonsOffVec[elementIntensity - 1]; j++) {
                        buttonMapEntry.push_back({index / m + b, index % m + b, true});
                        otherIndices.insert({index / m + b, index % m + b, -201 - button});
                        index++;
                    }
                    buttonMap.push_back(buttonMapEntry);
                    buttonMapEntry.clear();
                }
                break;
            case 3:
                deadSpots = numDeadCellsVec[elementIntensity - 1];
                for (int i = 0; i < deadSpots; i++) {
                    otherIndices.insert({index / m + b, index % m + b, 4});
                    index++;
                }
                break;
            case 4:
                transporterPairs = transporterPairsVec[elementIntensity - 1];
                for (int i = 0; i < transporterPairs; i++) {
                    otherIndices.insert({index / m + b, index % m + b, 100 + 2 * i});
                    pair<int, int> t1 = {index / m + b, index % m + b};
                    index++;
                    otherIndices.insert({index / m + b, index % m + b, 100 + 2 * i + 1});
                    transporters.push_back({index / m + b, index % m + b});
                    transporters.push_back(t1);
                    index++;
                }
            default:
                break;
        }
    }
    initializeGridAndGridFurniture();
}

void MetaBlocks::initializeGridAndGridFurniture() {
    n += 2 * b;
    m += 2 * b;
    vector<vector<int>> newGrid(n, vector<int>(m, 0));
    for (int i = b; i < n - b; i++) {
        for (int j = b; j < m - b; j++) {
            newGrid[i][j] = 1;
            oneIndices.insert({i, j});
        }
    }
    grid = newGrid; // Assign the temporary grid to the class grid
    // Now put the things in correctly:
    for (auto [x, y, z] : otherIndices) {
        grid[x][y] = z;
        oneIndices.erase({x, y});
    }
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

bool MetaBlocks::checkWin() {
    auto [x, y] = currPos;
    if (state == 0 && x == end.first && y == end.second) {
        return true;
    }
    return false;
}

bool checkOOB(const pair<int, int>& currPos, int n, int m) {
    const auto& [x, y] = currPos;
    // Check if the current position is out of bounds
    if (x < 0 || x >= n || y < 0 || y >= m) {
        return false;
    }
    return true;    
}

bool checkValid0(const pair<int, int>& currPos, const vector<vector<int>>& grid, const vector<bool>& buttons) {
    const auto& [x, y] = currPos;
    if (grid[x][y] == 4 || grid[x][y] == 0) {
        return false;
    }
    if (grid[x][y] < 0) {
        // Here's where it's actually interesting.
        if (grid[x][y] < -200 && buttons[-grid[x][y] % 200]) {
            return false;
        }
        if (grid[x][y] > -200 && !buttons[-grid[x][y]]) {
            return false;
        }
    }
    return true;
}

bool checkValid12(const pair<int, int>& currPos, const vector<vector<int>>& grid, const vector<bool>& buttons, int b, bool isOne) {
    const auto& [x, y] = currPos;
    bool firstHalf = false;
    bool secondHalf = false;
    for (int i = 0; i < b; i++) {
        if (firstHalf && secondHalf) {
            break;
        }
        int val = grid[x + i * isOne][y + i * (! isOne)];
        if (val == 4) {
            return false;
        }
        if (val == 1 || val == 2 || val == 3 || val > 99) {
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
        else if ((val < -200 && !buttons[-val % 200]) || (val > -200 && buttons[-val])) {
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
    return true;
}

bool MetaBlocks::checkValid() {
    if (! checkOOB(currPos, n, m)) {
        return false;
    }
    switch (state) {
        case 0:
            return checkValid0(currPos, grid, buttons);
            break;
        case 1:
            return checkValid12(currPos, grid, buttons, b, true);
            break;
        case 2:
            return checkValid12(currPos, grid, buttons, b, false);
            break;
        default:
            break;
    }
    return false;
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
    vector<tuple<int, int, int, bool>> bridgeIndices;
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
                bridgeIndices.push_back({(-grid[i][j]) % 200, i, j, (grid[i][j] < -200)});
            }
        }
    }
    buttons.resize(numButtons + 1, false);
    buttonMap.resize(numButtons + 1);
    for (auto [k, i, j, t] : bridgeIndices) {
        buttonMap[k].push_back({i, j, t});
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

void MetaBlocks::resetPuzzle() {
    state = 0;
    currPos = start;
    fill(buttons.begin(), buttons.end(), false);
}