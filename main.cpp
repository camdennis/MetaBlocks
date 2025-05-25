#include "MetaBlocks.h"

using namespace std;

int main() {
    string inputString;
    cin >> inputString;
    MetaBlocks puzzle;
    puzzle.setInitializationString(inputString);
    puzzle.MCSimulation(10000, 0.1);
    puzzle.showOptimalSolutions();
    string gridString = puzzle.getFullGridString();
    cout << gridString << endl;
    cout << puzzle.optimalSolution << endl;
    return 0;
}