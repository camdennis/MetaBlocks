#include "MetaBlocks.h"

int main() {
    MetaBlocks puzzle(5, 15, 2);
    puzzle.initialize();
    string fileName = "level3.csv";
    puzzle.loadGrid(fileName);
    puzzle.view();
    return 0;
}