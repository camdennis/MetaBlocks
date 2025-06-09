#include "MetaBlocks.h"

using namespace std;

int main() {
    string inputString;
    cin >> inputString;
    MetaBlocks puzzle;
//    puzzle.setInitializationString(inputString);
//    puzzle.MCSimulation(10000, 1);
    string gridString = puzzle.EvolutionAlgorithm(inputString, 1000, 5, 5);
    cout << gridString << endl;
    return 0;
}