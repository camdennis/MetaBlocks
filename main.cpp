#include "MetaBlocks.h"

using namespace std;

int main() {
    string inputString;
    cin >> inputString;
    string outputDirectory = "/home/rdennis/data/levelFiles";
    MetaBlocks puzzle;
//    puzzle.setInitializationString(inputString);
//    puzzle.MCSimulation(10000, 1);
    string gridString = puzzle.EvolutionAlgorithm(inputString, 5, 5);
    puzzle.saveJSONFILE(outputDirectory, inputString, gridString);
    return 0;
}