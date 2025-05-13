#include "MetaBlocks.h"

int main() {
    MetaBlocks puzzle(5, 15, 2);
    puzzle.initialize();
    string fileName = "level3.csv";
    puzzle.loadGrid(fileName);
    pair<int, int> sol = puzzle.getNumOptimalSolutions();
    cout << sol.first << " " << sol.second << endl;
    puzzle.view();
    return 0;
}