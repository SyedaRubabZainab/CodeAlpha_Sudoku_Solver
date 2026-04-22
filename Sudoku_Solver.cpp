// Sudoku Solver
// Purpose : Solve any valid Sudoku puzzle using backtracking

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

const int SIZE = 9;
const int BOX  = 3;

typedef int Grid[SIZE][SIZE];

void printHeader() {
    cout << "\n";
    cout << "  +======================================+\n";
    cout << "  |      SUDOKU SOLVER  --  CodeAlpha    |\n";
    cout << "  +======================================+\n\n";
}

void printGrid(const Grid grid, const string& title) {
    cout << "  " << title << "\n";
    cout << "  +---------+---------+---------+\n";
    for (int r = 0; r < SIZE; r++) {
        if (r == 3 || r == 6)
            cout << "  +---------+---------+---------+\n";
        cout << "  |";
        for (int c = 0; c < SIZE; c++) {
            if (c == 3 || c == 6) cout << " |";
            if (grid[r][c] == 0)
                cout << " .";
            else
                cout << " " << grid[r][c];
        }
        cout << " |\n";
    }
    cout << "  +---------+---------+---------+\n\n";
}

bool inRow(const Grid grid, int row, int num) {
    for (int c = 0; c < SIZE; c++)
        if (grid[row][c] == num) return true;
    return false;
}

bool inCol(const Grid grid, int col, int num) {
    for (int r = 0; r < SIZE; r++)
        if (grid[r][col] == num) return true;
    return false;
}

bool inBox(const Grid grid, int row, int col, int num) {
    int startRow = row - row % BOX;
    int startCol = col - col % BOX;
    for (int r = 0; r < BOX; r++)
        for (int c = 0; c < BOX; c++)
            if (grid[startRow + r][startCol + c] == num) return true;
    return false;
}

bool isSafe(const Grid grid, int row, int col, int num) {
    return !inRow(grid, row, num)
        && !inCol(grid, col, num)
        && !inBox(grid, row, col, num);
}

bool findEmpty(const Grid grid, int& row, int& col) {
    for (row = 0; row < SIZE; row++)
        for (col = 0; col < SIZE; col++)
            if (grid[row][col] == 0) return true;
    return false;
}

bool solveSudoku(Grid grid) {
    int row, col;
    if (!findEmpty(grid, row, col)) return true;

    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveSudoku(grid)) return true;
            grid[row][col] = 0;
        }
    }
    return false;
}

bool isValidPuzzle(const Grid grid) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            int val = grid[r][c];
            if (val < 0 || val > 9) return false;
            if (val != 0) {
                const_cast<int(*)[SIZE]>(grid)[r][c] = 0;
                bool ok = isSafe(grid, r, c, val);
                const_cast<int(*)[SIZE]>(grid)[r][c] = val;
                if (!ok) return false;
            }
        }
    }
    return true;
}

void inputPuzzle(Grid grid) {
    cout << "  Enter the Sudoku puzzle row by row.\n";
    cout << "  Use 0 (zero) for empty cells.\n";
    cout << "  Separate digits with spaces.\n\n";

    for (int r = 0; r < SIZE; r++) {
        bool validRow = false;
        while (!validRow) {
            cout << "  Row " << (r + 1) << " : ";
            validRow = true;
            for (int c = 0; c < SIZE; c++) {
                if (!(cin >> grid[r][c]) || grid[r][c] < 0 || grid[r][c] > 9) {
                    cout << "  [!] Invalid value. Each digit must be 0-9. Re-enter row.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    validRow = false;
                    break;
                }
            }
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void loadDemo(Grid grid) {
    int demo[SIZE][SIZE] = {
        {5, 3, 0,  0, 7, 0,  0, 0, 0},
        {6, 0, 0,  1, 9, 5,  0, 0, 0},
        {0, 9, 8,  0, 0, 0,  0, 6, 0},

        {8, 0, 0,  0, 6, 0,  0, 0, 3},
        {4, 0, 0,  8, 0, 3,  0, 0, 1},
        {7, 0, 0,  0, 2, 0,  0, 0, 6},

        {0, 6, 0,  0, 0, 0,  2, 8, 0},
        {0, 0, 0,  4, 1, 9,  0, 0, 5},
        {0, 0, 0,  0, 8, 0,  0, 7, 9}
    };
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            grid[r][c] = demo[r][c];
}

void copyGrid(const Grid src, Grid dst) {
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            dst[r][c] = src[r][c];
}

int main() {
    printHeader();

    Grid puzzle, solution;

    int choice;
    cout << "  [1] Enter puzzle manually\n";
    cout << "  [2] Use built-in demo puzzle\n\n";
    cout << "  Choice : ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) {
        cout << "\n";
        inputPuzzle(puzzle);
    } else {
        loadDemo(puzzle);
        cout << "\n  Demo puzzle loaded.\n\n";
    }

    if (!isValidPuzzle(puzzle)) {
        cout << "  [!] The puzzle has conflicting clues and cannot be solved.\n\n";
        return 1;
    }

    printGrid(puzzle, "Unsolved Puzzle:");

    copyGrid(puzzle, solution);

    cout << "  Solving...\n\n";

    bool solved = solveSudoku(solution);

    if (solved) {
        printGrid(solution, "[OK] Solved Puzzle:");
        cout << "  Solution found successfully!\n\n";
    } else {
        cout << "  [!] No solution exists for this puzzle.\n\n";
    }

    return 0;
}
