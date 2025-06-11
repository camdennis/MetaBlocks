#include "MetaBlocks.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <fstream>
//#include <SFML/Window.hpp>
//#include <SFML/OpenGL.hpp>
//#include <SFML/Graphics.hpp>
//#include <GL/glu.h>
#include <sstream>
#include <cmath>
#include <unordered_set>
#include <queue>
#include <climits>
#include <random>

using namespace std;
random_device rd;
mt19937 gen(rd());

string MetaBlocks::getJSONString() {
    string sol = "{ \"layout\": [[";
    for (int i = b; i < grid.size() - b; i++) {
        for (int j = b; j < grid[0].size() - b - 1; j++) {
            sol += to_string(grid[i][j]) + ',';
        }
        sol += to_string(grid[i][grid[0].size() - b - 1]) + ']';
        if (i == grid.size() - b - 1) {
            sol += "],\n";
        }
        else {
            sol += ",\n[";
        }
    }
    sol += "\"b\": " + to_string(b) + ",\n";
    sol += "\"solutionString\": \"" + optimalSolution + "\"\n}";
    return sol;
}

string MetaBlocks::getFullGridString() {
    string sol = "";
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            sol += to_string(grid[i][j]) + ',';
        }
    }
    return sol;
}

void MetaBlocks::setFullGridString(string gridString) {
    int index = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            string curr = "";
            while (index < gridString.size() && gridString[index] != ',') {
                curr += gridString[index];
                index++;
            }
            index++;
            grid[i][j] = stoi(curr);
        }
    }
}

void MetaBlocks::move(int moveId, bool undo) {
    if (undo) {
        unordered_map<int, int> undoMap = {{0, 1}, {1, 0}, {2, 3}, {3, 2}};
        moveId = undoMap[moveId];
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
        buttons[val % 100] = !deactivate;
    }
}

void MetaBlocks::transport() {
    if (state != 0) {
        return;
    }
    int val = grid[currPos.first][currPos.second];
    if (val / 100 == 1) {
        currPos = transporters[val % 100];
    }
}

bool MetaBlocks::checkValidSolution() {
    resetPuzzle();
    unordered_map<char, int> moveMap = {{'r', 0}, {'l', 1}, {'u', 2}, {'d', 3}};
    for (int i = 0; i < optimalSolution.size(); i++) {
        move(moveMap[optimalSolution[i]]);
        activateButton();
        transport();
        if (! checkValid()) {
            return false;
        }
    }
    return checkWin();
}

pair<int, int> MetaBlocks::showOptimalSolutions() {
    resetPuzzle();
    // This is going to be a BFS Algorithm
    unordered_map<string, int> stateStrings;
    stateStrings[getState()] = 0;
    queue<tuple<int, string, vector<int>>> q;
    q.push({0, getState(), {}});
    int bestTime = INT_MAX;
    int sol = 0;
    string newState;
    vector<vector<int>> bestMoveSets;
    while (! q.empty() && get<0>(q.front()) <= bestTime) {
        auto [t, stateString, moveSet] = q.front();
        q.pop();
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
                    q.push({t + 1, newState, newMoveSet});
                }
            }
            loadState(stateString);
        }
    }
    resetPuzzle();    
    string moveMap = "rlud";
    for (int mv : bestMoveSets[0]) {
        optimalSolution += moveMap[mv];
    }
    return {bestTime, sol};
}

pair<int, int> MetaBlocks::getNumOptimalSolutions() {
    resetPuzzle();
    // This is going to be a BFS Algorithm
    unordered_map<string, int> stateStrings;
    stateStrings[getState()] = 0;
    queue<pair<int, string>> q;
    q.push({0, getState()});
    int bestTime = INT_MAX;
    string newState;
    while (! q.empty() && q.front().first <= bestTime) {
        auto [t, stateString] = q.front();
        q.pop();
        loadState(stateString);
        for (int i = 0; i < 4; i++) {
            move(i);
            activateButton();
            transport();
            if (checkWin()) {
                if (t + 1 < bestTime) {
                    bestTime = t + 1;
                }
            }
            if (checkValid()) {
                newState = getState();
                if (stateStrings.find(newState) == stateStrings.end() || stateStrings[newState] >= t + 1) {
                    stateStrings[newState] = t + 1;
                    q.push({t + 1, newState});
                }
            }
            loadState(stateString);
        }
    }
    resetPuzzle();
    if (bestTime == INT_MAX) {
        return {-1, -1};
    }
    return {bestTime, q.size()};
}

void MetaBlocks::updateIndices() {
    zeroIndices.clear();
    oneIndices.clear();
    otherIndices.clear();
    for (int i = b; i < grid.size() - b; i++) {
        for (int j = b; j < grid[0].size() - b; j++) {
            if (grid[i][j] == 0) {
                zeroIndices.insert({i, j});
            }
            else if (grid[i][j] == 1) {
                oneIndices.insert({i, j});
            }
            else {
                otherIndices.insert({i, j, grid[i][j]});
            }
        }
    }
}

void moveFromAtoB(set<pair<int, int>>& A, set<pair<int, int>>& B, tuple<int, int, int> p, vector<vector<int>>& sol, int newType = 1) {
    vector<int> temp;
    auto [x, y, z] = p;
    temp = {x, y, z, newType};
    pair<int, int> coord = {x, y};
    A.erase(coord);
    B.insert(coord);
    sol.push_back(temp);
}

void MetaBlocks::swapBlocks(tuple<int, int, int>& p1, tuple<int, int, int>& p2, vector<vector<int>>& sol) {
    auto [x1, y1, v1] = p1;
    auto [x2, y2, v2] = p2;
    sol.push_back({x1, y1, v1, v2});
    sol.push_back({x2, y2, v2, v1});
    grid[x1][y1] = v2;
    grid[x2][y2] = v1;
    if (v2 == 2) {
        start = {x1, y1};
        currPos = start;
    }
    else if (v2 == 3) {
        end = {x1, y1};
    }
    if (v1 == 2) {
        start = {x2, y2};
        currPos = start;
    }
    else if (v1 == 3) {
        end = {x2, y2};
    }
    if (v1 >= 100 && v1 < 200) {
        if (v1 % 2) {
            transporters[v1 % 100 - 1] = {x1, y1};
        }
        else {
            transporters[v1 % 100 + 1] = {x1, y1};
        }
    }
    if (v2 >= 100 && v2 < 200) {
        if (v2 % 2) {
            transporters[v2 % 100 - 1] = {x2, y2};
        }
        else {
            transporters[v2 % 100 + 1] = {x2, y2};
        }
    }
    tuple<int, int, int> newP;
    // Now go through the cases:
    switch (v2) {
        case 0:
            zeroIndices.erase({x2, y2});
            break;
        case 1:
            oneIndices.erase({x2, y2});
            break;
        default:
            otherIndices.erase(p2);
            break;
    }
    switch (v1) {
        case 0:
            zeroIndices.erase({x1, y1});
            break;
        case 1:
            oneIndices.erase({x1, y1});
            break;
        default:
            otherIndices.erase(p1);
            break;
    }
    switch (v1) {
        case 0:
            zeroIndices.insert({x2, y2});
            break;
        case 1:
            oneIndices.insert({x2, y2});
            break;
        default:
            newP = {x2, y2, v1};
            otherIndices.insert(newP);
            break;
    }
    switch (v2) {
        case 0:
            zeroIndices.insert({x1, y1});
            break;
        case 1:
            oneIndices.insert({x1, y1});
            break;
        default:
            newP = {x1, y1, v2};
            otherIndices.insert(newP);
            break;
    }
}

void MetaBlocks::resetBlocks(vector<int>& p) {
    int x, y, v1, v2;
    x = p[0];
    y = p[1];
    v1 = p[2];
    v2 = p[3];
    grid[x][y] = v1;
    if (v1 == 2) {
        start = {x, y};
        currPos = start;
    }
    else if (v1 == 3) {
        end = {x, y};
    }
    if (v1 >= 100 && v1 < 200) {
        if (v1 % 2) {
            transporters[v1 % 100 - 1] = {x, y};
        }
        else {
            transporters[v1 % 100 + 1] = {x, y};
        }
    }
    // Now go through the cases:
    switch (v2) {
        case 0:
            zeroIndices.erase({x, y});
            break;
        case 1:
            oneIndices.erase({x, y});
            break;
        default:
            otherIndices.erase({x, y, v2});
            break;
    }
    switch (v1) {
        case 0:
            zeroIndices.insert({x, y});
            break;
        case 1:
            oneIndices.insert({x, y});
            break;
        case 2:
            otherIndices.insert({x, y, v1});
            break;
        default:
            break;
    }
}

vector<vector<int>> MetaBlocks::basicMCMove() {
    // Make a move--choose either to add or subtract a singular block from the grid:
    int totalIndices = zeroIndices.size() + oneIndices.size() + otherIndices.size();
    uniform_int_distribution<> flip(0, totalIndices - 1);
    uniform_real_distribution<> pickCase(0, 1.0);
    int index1 = flip(gen);
    int index2 = flip(gen);
    while (index2 == index1) {
        index2 = flip(gen);
    }
    tuple<int, int, int> p1, p2, temp;
    pair<int, int> coord;
    int type1, type2;
    if (index1 < zeroIndices.size()) {
        type1 = 0;
        auto it = zeroIndices.begin();
        advance(it, index1);
        coord = *it;
        p1 = {coord.first, coord.second, grid[coord.first][coord.second]};
    } 
    else if (index1 < (oneIndices.size() + zeroIndices.size())) {
        type1 = 1;
        index1 -= zeroIndices.size();
        auto it = oneIndices.begin();
        advance(it, index1);
        coord = *it;
        p1 = {coord.first, coord.second, grid[coord.first][coord.second]};
    }
    else {
        type1 = 2;
        index1 -= (zeroIndices.size() + oneIndices.size());
        auto it = otherIndices.begin();
        advance(it, index1);
        p1 = *it;
    }
    if (index2 < zeroIndices.size()) {
        type2 = 0;
        auto it = zeroIndices.begin();
        advance(it, index2);
        coord = *it;
        p2 = {coord.first, coord.second, grid[coord.first][coord.second]};
    } 
    else if (index2 < (oneIndices.size() + zeroIndices.size())) {
        type2 = 1;
        index2 -= zeroIndices.size();
        auto it = oneIndices.begin();
        advance(it, index2);
        coord = *it;
        p2 = {coord.first, coord.second, grid[coord.first][coord.second]};
    }
    else {
        type2 = 2;
        index2 -= (zeroIndices.size() + oneIndices.size());
        auto it = otherIndices.begin();
        advance(it, index2);
        p2 = *it;
    }
    if (type2 < type1) {
        swap(type1, type2);
        swap(index1, index2);
        swap(p1, p2);
    }
    vector<vector<int>> sol;
    float pick = pickCase(gen);
    switch (type2 * 3 + type1) {
        case 0:
            if (pick < 1.0f / 3.0f) {
                // set 0, 0 to 0, 1
                // move p2 from zeroIndices to oneIndices:
                moveFromAtoB(zeroIndices, oneIndices, p2, sol, 1);
                grid[get<0>(p2)][get<1>(p2)] = 1;
            }
            else if (pick < 2.0f / 3.0f) {
                // set 0, 0 to 1, 0
                // move p1 from zeroIndices to oneIndices:
                moveFromAtoB(zeroIndices, oneIndices, p1, sol, 1);
                grid[get<0>(p1)][get<1>(p1)] = 1;
            }
            else {
                // set 0, 0 to 1, 1
                // move p1 and p2 from zeroIndices to oneIndices:
                moveFromAtoB(zeroIndices, oneIndices, p1, sol, 1);
                grid[get<0>(p1)][get<1>(p1)] = 1;
                moveFromAtoB(zeroIndices, oneIndices, p2, sol, 1);
                grid[get<0>(p2)][get<1>(p2)] = 1;
            }
            break;
        case 1:
            if (pick < 1.0f / 3.0f) {
                // set 0, 1 to 0, 0
                // move p2 from 1 to 0:
                moveFromAtoB(oneIndices, zeroIndices, p2, sol, 0);
                grid[get<0>(p2)][get<1>(p2)] = 0;
            }
            else if (pick < 2.0f / 3.0f) {
                // set 0, 1 to 1, 1
                // Move p1 from 0 to 1
                moveFromAtoB(zeroIndices, oneIndices, p1, sol, 1);
                grid[get<0>(p1)][get<1>(p1)] = 1;
            }
            else {
                // set 0, 1 to 1, 0
                // Move p1 from 0 to 1
                moveFromAtoB(zeroIndices, oneIndices, p1, sol, 1);
                grid[get<0>(p1)][get<1>(p1)] = 1;
                // Move p2 from 1 to 0
                moveFromAtoB(oneIndices, zeroIndices, p2, sol, 0);
                grid[get<0>(p2)][get<1>(p2)] = 0;
            }
            break;
        case 2:
            // 0 2 to 2 0
            swapBlocks(p1, p2, sol);
            break;
        case 4:
            if (pick < 1.0f / 3.0f) {
                // set 1, 1 to 0, 1
                moveFromAtoB(oneIndices, zeroIndices, p1, sol, 0);
                grid[get<0>(p1)][get<1>(p1)] = 0;
            }
            else if (pick < 2.0f / 3.0f) {
                // set 1, 1 to 1, 0
                moveFromAtoB(oneIndices, zeroIndices, p2, sol, 0);
                grid[get<0>(p2)][get<1>(p2)] = 0;
            }
            else {
                // set 1, 1 to 0, 0
                moveFromAtoB(oneIndices, zeroIndices, p1, sol, 0);
                grid[get<0>(p1)][get<1>(p1)] = 0;
                moveFromAtoB(oneIndices, zeroIndices, p2, sol, 0);
                grid[get<0>(p2)][get<1>(p2)] = 0;
            }
            break;
        case 5:
            // 1 2 to 2 1
            swapBlocks(p1, p2, sol);
            break;
        case 8:
            // 2 2 to 2 2
            swapBlocks(p1, p2, sol);
            break;
        default:
            break;        
    }
    return sol;
}

void MetaBlocks::undoBasicMCMove(vector<vector<int>>& swapPairs) {
    for (int i = 0; i  < swapPairs.size(); i++) {
        resetBlocks(swapPairs[i]);
    }
}

double MetaBlocks::getEnergy() {
    auto [bestTime, fakeSteps] = getNumOptimalSolutions();
    return -(double) bestTime * sqrt((double) (fakeSteps + 1));
}

double MetaBlocks::MCStep(double energy, double temperature) {
    vector<vector<int>> coords = basicMCMove();
    double deltaE = getEnergy() - energy;
    if (deltaE < 0) {
        return energy + deltaE;
    }

    double prob = exp(-deltaE / temperature);
    uniform_real_distribution<> dis(0.0, 1.0);
    if (dis(gen) > prob) {
        undoBasicMCMove(coords);
        return energy;
    }
    accept++;
    return energy + deltaE;
}

void MetaBlocks::MCSimulation(int numSteps, double temperature) {
    updateIndices();
    int i = 0;
    double energy = getEnergy();
    double prevEnergy = energy;
    string sol;
    while (i < numSteps && energy > -difficulty) {
        energy = MCStep(energy, temperature);
        if (energy < prevEnergy) {
            prevEnergy = energy;
            sol = getFullGridString();
        }
        i++;
    }
    cout << (double) accept / i << endl;
    setFullGridString(sol);
    resetPuzzle();
}

class gridQueue : public MetaBlocks {
public:
    vector<vector<vector<int>>> allGrids;
    vector<set<pair<int, int>>> zeroIndicesSet;
    vector<set<pair<int, int>>> oneIndicesSet;
    vector<set<tuple<int, int, int>>> otherIndicesSet;
    priority_queue<pair<double, int>> pq;
    set<int> best;
    int minPopulation;
    int numChildren;
    gridQueue(int _minPopulation, int _numChildren) : minPopulation(_minPopulation), numChildren(_numChildren) {}

    void initialize(string init) {
        setInitializationString(init);
        updateIndices();
        double energy = getEnergy();
        for (int i = 0; i < minPopulation * (numChildren + 1); i++) {
            allGrids.push_back(grid);
            zeroIndicesSet.push_back(zeroIndices);
            oneIndicesSet.push_back(oneIndices);
            otherIndicesSet.push_back(otherIndices);
        }
        for (int i = 0; i < minPopulation; i++) {
            best.insert(i);
            pq.push({energy, i});
        }
    }

    void cull() {
        while (pq.size() > minPopulation) {
            auto [en, ind] = pq.top();
            pq.pop();
            best.erase(ind);
        }
    }

    void propagate() {
        int i = 0;
        for (int idx : best) {
            int remainingChildren = numChildren;
            grid = allGrids[idx];
            zeroIndices = zeroIndicesSet[idx];
            oneIndices = oneIndicesSet[idx];
            otherIndices = otherIndicesSet[idx];
            while (remainingChildren > 0) {
                if (best.find(i) != best.end()) {
                    i++;
                    continue;
                }
                vector<vector<int>> coords = basicMCMove();
                double energy = getEnergy();
                allGrids[i] = grid;
                zeroIndicesSet[i] = zeroIndices;
                oneIndicesSet[i] = oneIndices;
                otherIndicesSet[i] = otherIndices;
                pq.push({energy, i});
                undoBasicMCMove(coords);
                i++;
                remainingChildren--;
            }
        }
        // Fill up best with everything:
        for (int j = 0; j < minPopulation * (numChildren + 1); j++) {
            best.insert(j);
        }
    }

    void setBest() {
        while (pq.size() > 1) {
            pq.pop();
        }
        auto [energy, index] = pq.top();
        grid = allGrids[index];
        zeroIndices = zeroIndicesSet[index];
        oneIndices = oneIndicesSet[index];
        otherIndices = otherIndicesSet[index];
        showOptimalSolutions();
    }

    string getSolutionJSON() {
        return getJSONString();
    }
};

// Write a genetic algorithm:
string MetaBlocks::EvolutionAlgorithm(string init, int minPopulation, int numChildren) {
    gridQueue gq(minPopulation, numChildren);
    gq.initialize(init);
    for (int i = 0; i < gq.difficulty; i++) {
        gq.propagate();
        gq.cull();
//        cout << "generation " << i << endl;
    }
    gq.setBest();
    return gq.getSolutionJSON();
}

void MetaBlocks::printGrid() {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
}