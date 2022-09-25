#include <iostream>
#include <String>
#include <mutex>
#include <fstream>

using namespace std;
class SudokuGrid;
void solveSudokuPuzzles();

int main()
{

    std::cout << "Hello World!\n";
    int numThreads;
    solveSudokuPuzzles();
}

class SudokuGrid
{
public:
    string m_strGridName;
    unsigned char gridElement[9][9];

    friend fstream& operator>>(fstream& os, SudokuGrid& gridIn);
    friend fstream& operator<<(fstream& os, const SudokuGrid& gridIn);

    void solve();
};


void SudokuGrid::solve()
{

}

void solveSudokuPuzzles()
{

}

fstream& operator>>(fstream& os, const SudokuGrid& gridIn)
{
    // TODO: insert return statement here
}

fstream& operator<<(fstream& os, const SudokuGrid& gridIn)
{
    // TODO: insert return statement here
}

mutex outFileMutex;
mutex inFileMutex;
fstream outFile;
fstream inFile;