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
#include <unordered_set>
#include <queue>
#include <climits>
#include <random>

using namespace std;

void MetaBlocks::move(int moveId, bool undo) {
    if (undo) {
        switch (moveId) {
            case 0:
                moveId = 1;
                break;
            case 1:
                moveId = 0;
                break;
            case 2:
                moveId = 3;
                break;
            case 3:
                moveId = 2;
                break;
        }
    }
    pair<int, int> nextState;
    switch (moveId) {
        case 0: // Right
            nextState = mp[state][2];
            state = nextState.first;
            currPos.first += offset[nextState.second].first;
            currPos.second += offset[nextState.second].second;
            break;
        case 1: // Left:
            nextState = mp[state][3];
            state = nextState.first;
            currPos.first += offset[nextState.second].first;
            currPos.second += offset[nextState.second].second;
            break;
        case 2: // Up:
            nextState = mp[state][1];
            state = nextState.first;
            currPos.first += offset[nextState.second].first;
            currPos.second += offset[nextState.second].second;
            break;
        case 3: // Down:
            nextState = mp[state][0];
            state = nextState.first;
            currPos.first += offset[nextState.second].first;
            currPos.second += offset[nextState.second].second;
            break;
        default:
            break;
    }
}

void MetaBlocks::activateButton(bool deactivate) {
    if (state != 0) {
        return;
    }
    int val = grid[currPos.first][currPos.second]; 
    if (val / 100 == 2) {
        if (! deactivate) {
            buttons[val % 100] = true;
        }
        else {
            buttons[val % 100] = false;
        }
    }
}

void MetaBlocks::transport() {
    if (state != 0) {
        return;
    }
    int val = grid[currPos.first][currPos.second];
    if (val / 100 == 1) {
        if (val % 2) {
            currPos = transporters[val % 100];
        }
        else {
            currPos = transporters[val % 100];
        }
    }
}

pair<int, int> MetaBlocks::showOptimalSolutions() {
    resetPuzzle();
    // This is going to be Dijkstra's Algorithm
    unordered_map<string, int> stateStrings;
    stateStrings[getState()] = 0;
    priority_queue<tuple<int, string, vector<int>>, vector<tuple<int, string, vector<int>>>, greater<tuple<int, string, vector<int>>>> pq;
    pq.push({0, getState(), {}});
    int bestTime = INT_MAX;
    int sol = 0;
    string newState;
    vector<vector<int>> bestMoveSets;
    while (! pq.empty() && get<0>(pq.top()) <= bestTime) {
        auto [t, stateString, moveSet] = pq.top();
        pq.pop();
        loadState(stateString);
        for (int i = 0; i < 4; i++) {
            move(i);
            activateButton();
            transport();
            if (checkWin()) {
                if (t + 1 < bestTime) {
                    bestTime = t + 1;
                    bestMoveSets = {moveSet};
                    bestMoveSets[0].push_back(i);
                    sol = 1;
                }
                else if (t + 1 == bestTime) {
                    bestMoveSets.push_back(moveSet);
                    bestMoveSets[bestMoveSets.size() - 1].push_back(i);
                    sol++;
                }
            }
            if (checkValid()) {
                newState = getState();
                if (stateStrings.find(newState) == stateStrings.end() || stateStrings[newState] >= t + 1) {
                    stateStrings[newState] = t + 1;
                    auto newMoveSet = moveSet;
                    newMoveSet.push_back(i);
                    pq.push({t + 1, newState, newMoveSet});
                }
            }
            loadState(stateString);
        }
    }
    resetPuzzle();
    vector<string> moveMap = {"right", "left", "up", "down"};
   cout << "Found " << bestMoveSets.size() << " optimal solutions in " << bestTime << " moves:" << endl;
    for (vector<int> mvs : bestMoveSets) {
        for (int mv : mvs) {
            cout << moveMap[mv] << " ";
        }
        cout << "\n";
    }
    cout << endl;
    return {bestTime, sol};
}

pair<int, int> MetaBlocks::getNumOptimalSolutions() {
    resetPuzzle();
    // This is going to be Dijkstra's Algorithm
    unordered_map<string, int> stateStrings;
    stateStrings[getState()] = 0;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push({0, getState()});
    int bestTime = INT_MAX;
    int sol = 0;
    string newState;
    while (! pq.empty() && pq.top().first <= bestTime) {
        auto [t, stateString] = pq.top();
        pq.pop();
        loadState(stateString);
        for (int i = 0; i < 4; i++) {
            move(i);
            activateButton();
            transport();
            if (checkWin()) {
                if (t + 1 < bestTime) {
                    bestTime = t + 1;
                    sol = 1;
                }
                else if (t + 1 == bestTime) {
                    sol++;
                }
            }
            if (checkValid()) {
                newState = getState();
                if (stateStrings.find(newState) == stateStrings.end() || stateStrings[newState] >= t + 1) {
                    stateStrings[newState] = t + 1;
                    pq.push({t + 1, newState});
                }
            }
            loadState(stateString);
        }
    }
    resetPuzzle();
    if (bestTime == INT_MAX) {
        return {-1, -1};
    }
    return {bestTime, sol};
}

void MetaBlocks::updateIndices() {
    zeroIndices.clear();
    oneIndices.clear();
    for (int i = b; i < grid.size() - b; i++) {
        for (int j = b; j < grid[0].size() - b; j++) {
            if (grid[i][j] == 0) {
                zeroIndices.insert({i, j});
            }
            else if (grid[i][j] == 1) {
                oneIndices.insert({i, j});
            }
        }
    }
}

pair<int, int> MetaBlocks::basicMCMove() {
    // Make a move--choose either to add or subtract a singular block from the grid:
    random_device rd;
    mt19937 gen(rd());
    int totalIndices = oneIndices.size() + zeroIndices.size();
    uniform_int_distribution<> flip(0, totalIndices - 1);
    cout << "0" << endl;
    int index = flip(gen);
    cout << "0" << endl;
    pair<int, int> coord;
    cout << "0" << endl;
    if (index < oneIndices.size()) {
        auto it = oneIndices.begin();
        advance(it, index);
        coord = *it;
        oneIndices.erase(coord);
        zeroIndices.insert(coord);
    }
    else {
        auto it = zeroIndices.begin();
        advance(it, index - oneIndices.size());
        coord = *it;
        zeroIndices.erase(coord);
        oneIndices.insert(coord);
    }
    grid[coord.first][coord.second] = 1 - grid[coord.first][coord.second];
    return coord;
}

void MetaBlocks::undoBasicMCMove(pair<int, int>& coord) {
    if (grid[coord.first][coord.second]) {
        zeroIndices.insert(coord);
        oneIndices.erase(coord);
    }
    else {
        zeroIndices.erase(coord);
        oneIndices.insert(coord);
    }
    grid[coord.first][coord.second] = 1 - grid[coord.first][coord.second];
}

double MetaBlocks::getEnergy() {
    auto [bestTime, numSolutions] = getNumOptimalSolutions();
    cout << "bestTime=" << bestTime << endl;
    return -(double) bestTime;
}

double MetaBlocks::MCStep(double energy, double temperature) {
    pair<int, int> coord = basicMCMove();
    double deltaE = getEnergy() - energy;
    if (deltaE < 0) {
        return energy + deltaE;
    }

    double prob = exp(-deltaE / temperature);
    cout << "prob=" << prob << endl;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    if (dis(gen) > prob) {
        undoBasicMCMove(coord);
        return energy;
    }
    return energy + deltaE;
}

void MetaBlocks::MCSimulation(int numSteps, double energyThreshold, double temperature) {
    updateIndices();
    int i = 0;
    double energy = getEnergy();
    while (i < numSteps && energy > energyThreshold) {
        energy = MCStep(energy, temperature);
        i++;
    }
}