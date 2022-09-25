// Lab1_Problem1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Vector>
#include <fstream>

using namespace std;
uint64_t numberGridPaths(unsigned int nRows, unsigned int nCols);

int main()
{
    unsigned int nRows;
    unsigned int nCols;

    cout << "Enter nRows: ";
    cin >> nRows;
    cout << "Enter nCols: ";
    cin >> nCols;

    uint64_t path = numberGridPaths(nRows, nCols);
    cout << path << endl;

    ofstream pathFile("NumberPaths.txt");
    pathFile << path;
    pathFile.close();

    return 0;
}