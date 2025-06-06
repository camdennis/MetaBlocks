#include "MetaBlocks.h"

using namespace std;

int main() {
    string inputString;
    cin >> inputString;
    MetaBlocks puzzle;
    puzzle.setInitializationString(inputString);
    puzzle.MCSimulation(10000, 1);
    puzzle.showOptimalSolutions();
    string gridString = puzzle.getJSONString();
    cout << gridString << endl;
    return 0;
}