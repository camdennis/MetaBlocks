#include "MetaBlocks.h"

using namespace std;

int main() {
    MetaBlocks puzzle(5, 15, 3);
    puzzle.initialize();
    string fileName = "level3.csv";
    puzzle.loadGrid(fileName);
//    pair<int, int> sol = puzzle.getNumOptimalSolutions();
//    cout << sol.first << " " << sol.second << endl;
//    puzzle.showOptimalSolutions();
//    cout << puzzle.grid.size() << " " << puzzle.grid[0].size() << endl;
//    puzzle.view();
    puzzle.MCSimulation(10000, -100, 0.001);
    puzzle.showOptimalSolutions();
    puzzle.view();
    return 0;
}