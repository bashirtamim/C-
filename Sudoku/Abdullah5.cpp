//Bashir Abdullah
// Sudoku game board
// Implemented methods:
//     1. show;
//     2. swap;
//     3. verify;
//     4. erase;
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <regex>
#include <ctime>

using namespace std;

class Board {
    
public:
    
    Board() {
        
        // Init random generator.
        srand(time(NULL));
        
        // Create valid sudoku board
        sudoku = {
            {1, 2, 3, 7, 8, 0, 4, 5, 6},
            {4, 5, 6, 1, 2, 3, 7, 8, 0},
            {7, 8, 0, 4, 5, 6, 1, 2, 3},
            {2, 3, 4, 8, 0, 1, 5, 6, 7},
            {5, 6, 7, 2, 3, 4, 8, 0, 1},
            {8, 0, 1, 5, 6, 7, 2, 3, 4},
            {0, 1, 2, 6, 7, 8, 3, 4, 5},
            {3, 4, 5, 0, 1, 2, 6, 7, 8},
            {6, 7, 8, 3, 4, 5, 0, 1, 2}
        };
        
        
    }
    
    // Print game state at console.
    void show() {
        cout << " ";
        // print columns captions
        for (int column = 0; column < 9; ++column)
            cout << " " << getColumnLetter(column);
        cout << endl;
        for (int row = 0; row < 9; ++row) {
            // print row caption
            cout << getRowLetter(row);
            // print row values
            for (int column = 0; column < 9; ++column) {
                cout << ' ';
                // erased values shown as '-'
                sudoku[row][column] == -1 ? (cout << '-') : (cout << sudoku[row][column]+1);
            }
            cout << endl;
        }
    }
    
    //  swap.
    void swap() {
        // choose randomly to swap rows or columns
        (rand()&1)
        ? swapRows(rand()%9, rand()%9)
        : swapColumns(rand()%9, rand()%9);
    }
    
    // check validity of sudoku board.
    void verify() {
        if (verifyRows() & verifyColumns() & verifyComponents())
            cout << "- All columns, rows, and components are OK..." << endl;
    }
    
    // Interface method for erase random cell.
    // Erased cell represented as -1.
    void erase() {
        int row = rand()%9, col = rand()%9;
        cout << "Erasing row " << getRowLetter(row) << " column " << getColumnLetter(col) << endl;
        sudoku[row][col] = -1;
    }
    
private:
    
    // return caption of i-th column
    inline char getColumnLetter(int i) { return 'A' + i; }
    
    // return caption of i-th row
    inline char getRowLetter(int i) { return 'P' + i; }
    
    //correct swap of two rows r1 and r2
    void swapRows(int row1, int row2) {
        cout << "Trying to swap rows " << getRowLetter(row1) << " and " << getRowLetter(row2) << "..." << endl;
        
        if (row1/3 == row2/3) {
            // simple swap
            swap2Rows(row1, row2);
        }
        else {
            //extended swap
            for (int i = 0; i < 3; ++i)
                swap2Rows(row1/3*3+i, row2/3*3+i);
        }
    }
    
    //correct swap of two columns c1 and c2
    void swapColumns(int col1, int col2) {
        cout << "Trying to swap columns " << getColumnLetter(col1) << " and " << getColumnLetter(col2) << "..." << endl;
        //simple swap
        if (col1/3 == col2/3) {
            swap2Columns(col1, col2);
        }
        else {
            //extended swap
            for (int i = 0; i < 3; ++i)
                swap2Columns(col1/3*3+i, col2/3*3+i);
        }
    }
    
    // simple swap of two rows r1 and r2
    void swap2Rows(int r1, int r2) {
        for (int i = 0; i < 9; ++i)
            ::swap(sudoku[r1][i], sudoku[r2][i]);
        
        cout << "- Rows " << getRowLetter(r1) << " and " << getRowLetter(r2) << " were swapped..." << endl;
    }
    
    
    // simple swap of two columns c1 and c2
    void swap2Columns(int c1, int c2) {
        for (int i = 0; i < 9; ++i)
            ::swap(sudoku[i][c1], sudoku[i][c2]);
        
        cout << "- Columns " << getColumnLetter(c1) << " and " << getColumnLetter(c2) << " were swapped..." << endl;
    }
    
    // check conditions for columns
    bool verifyColumns() {
        bool ret = true;
        // for each column
        for (int column = 0; column < 9; ++column) {
            bool valid = true;
            // for marking used values
            used.assign(9, false);
            // go through each row and make two checks: erase, recurrence
            for (int row = 0; row < 9; ++row) {
                if (sudoku[row][column] == -1 || used[sudoku[row][column]]) {
                    valid = false;
                    break;
                }
                used[sudoku[row][column]] = true;
            }
            // inconsistency message
            if (!valid) {
                ret = false;
                cout << "- Found inconsistency in column " << getColumnLetter(column) << "..." << endl;
            }
        }
        return ret;
    }
    
    // check conditions for rows
    bool verifyRows() {
        bool ret = true;
        // for each row
        for (int row = 0; row < 9; ++row) {
            bool valid = true;
            // for marking used values
            used.assign(9, false);
            // go through each column and make two checks:erase, recurrence
            for (int column = 0; column < 9; ++column) {
                if (sudoku[row][column] == -1 || used[sudoku[row][column]]) {
                    valid = false;
                    break;
                }
                used[sudoku[row][column]] = true;
            }
            // show message if inconsistency found
            if (!valid) {
                ret = false;
                cout << "- Found inconsistency in row " << getRowLetter(row) << "..." << endl;
            }
        }
        return ret;
    }
    
    // check conditions for components
    bool verifyComponents() {
        bool ret = true;
        // For each component.
        for (int rowComponent = 0; rowComponent < 3; ++rowComponent)
            for (int columnComponent = 0; columnComponent < 3; ++columnComponent) {
                bool valid = true;
                // for marking used values
                used.assign(9, false);
                // go through each row, column for each component and check: erased, recurrence
                for (int row = 0; valid && row < 3; ++row)
                    for (int column = 0; column < 3; ++column) {
                        if (sudoku[rowComponent*3+row][columnComponent*3+columnComponent] == -1
                            || used[sudoku[rowComponent*3+row][columnComponent*3+column]]) {
                            valid = false;
                            break;
                        }
                        used[sudoku[rowComponent*3+row][columnComponent*3+column]] = true;
                    }
                //inconsistency message
                if (!valid) {
                    ret = false;
                    cout << "- Found inconsistency in component starting at row "
                    << getRowLetter(rowComponent*3) << " and column " << getColumnLetter(columnComponent*3) << "..." << endl;
                }
            }
        return ret;
    }
    
    //vector for validity checking
    vector<bool> used;
    //vector for board
    vector<vector<short> > sudoku;
    
} board;

int main() {
    cout << "Welcome Sudoku Initializer!"<< endl;
    do {
        string s;
        getline(cin, s);
        if (s == "show")        board.show();
        else if (s == "swap")   board.swap();
        else if (s == "verify") board.verify();
        else if (s == "erase")  board.erase();
        else if (s == "quit")   break;
        else                    cout << "Input error. Enter show/swap/verify/erase/quit command:";
    }
    while (true);
    
    cout << "Bye..." << endl;
    return 0;
}
