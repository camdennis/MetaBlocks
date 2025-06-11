#include "MetaBlocks.h"

using namespace std;

int main() {
    string inputFile;
    cin >> inputFile;
    MetaBlocks puzzle;
    puzzle.loadJSONFILE(inputFile);
    bool isValid = puzzle.checkValidSolution();
    cout << isValid << endl;
    return 0;
}