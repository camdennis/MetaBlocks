#include "MetaBlocks.h"

using namespace std;

int main() {
    int level;
    cin >> level;
    MetaBlocks puzzle(5, 15, 2, level);
    puzzle.initialize();
//    string fileName = "level3.csv";
//    puzzle.loadGrid(fileName);
//    pair<int, int> sol = puzzle.getNumOptimalSolutions();
//    cout << sol.first << " " << sol.second << endl;
//    puzzle.showOptimalSolutions();
//    cout << puzzle.grid.size() << " " << puzzle.grid[0].size() << endl;
//    puzzle.view();
    puzzle.MCSimulation(100000, -100, 0.001);
    puzzle.showOptimalSolutions();
    for (int i = 0; i < puzzle.n; i++) {
        for (int j = 0; j < puzzle.m; j++) {
            cout << puzzle.grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << endl;
    puzzle.view();
    return 0;
}