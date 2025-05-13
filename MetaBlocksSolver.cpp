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

pair<int, int> MetaBlocks::getNumOptimalSolutions() {
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
                if (stateStrings.find(newState) == stateStrings.end() || stateStrings[newState] > t + 1) {
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
    for (vector<int> mvs : bestMoveSets) {
        for (int mv : mvs) {
            cout << mv << " ";
        }
        cout << "\n";
    }
    cout << endl;
    return {bestTime, sol};
}

