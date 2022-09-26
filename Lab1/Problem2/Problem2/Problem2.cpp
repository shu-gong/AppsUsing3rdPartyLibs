/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 9/25/2022

Description:

The multi-threaded program solves Su Doku puzzles. 
It replaces the blanks (or zeros) in a 9 by 9 grid 
in such that each row, column, and 3 by 3 box contains 
each of the digits 1 to 9. 

Reference:
[1] https://www.tutorialspoint.com/sudoku-solver-in-cplusplus
*/

#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;
using namespace chrono;

#define N 9
mutex outFileMutex;
mutex inFileMutex;
fstream outFile;
fstream inFile;

/******************************************************************/
// @brief: SudokuGrid is used to hold a single puzzle with 
//         a constant 9 x 9 array of unsigned char elements.
/******************************************************************/
class SudokuGrid
{
public:
	string m_strGridName;
	unsigned char gridElement[N][N];
	int grid[N][N];
	
	/******************************************************************/
	// @brief: reads a single SudokuGrid object from a fstream file
	// @para: os: fstream, gridIn: an instance of SudokuGrid
	// @ret: return the fstream os
	/******************************************************************/
	friend fstream& operator>>(fstream& os, SudokuGrid& gridIn)
	{
		string buf;
		getline(os, buf);
		if (!isdigit(buf[0]))
		{
			gridIn.m_strGridName = buf;
		}

		for (int i = 0; i < N; i++)
		{
			getline(os, buf);
			for (int j = 0; j < N; j++)
			{
				gridIn.grid[i][j] = buf[j] - '0';
			}
		}
		return os;
	}
	
	/******************************************************************/
	// @brief: writes the SudokuGrid object to a file in the same format 
	//         that is used in reading in the object
	// @para: os: fstream, gridIn: an instance of SudokuGrid
	// @ret: return the fstream os
	/******************************************************************/
	friend fstream& operator<<(fstream& os, const SudokuGrid& gridIn)
	{
		os << gridIn.m_strGridName << endl;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				os << gridIn.grid[i][j];
			}
			os << endl;
		}
		return os;
	}

	/******************************************************************/
	// @brief: the function that solves the puzzle
	// @ret: return true if successful
	/******************************************************************/
	bool solve()
	{
		int row, col;
		if (!findEmptyPlace(row, col))
			return true; //when all places are filled
		for (int num = 1; num <= N; num++)
		{ //valid numbers are 1 - 9
			if (isValidPlace(row, col, num))
			{ //check validation, if yes, put the number in the grid
				grid[row][col] = num;
				if (solve()) //recursively go for other rooms in the grid
					return true;
				grid[row][col] = 0; //turn to unassigned space when conditions are not satisfied
			}
		}
		return false;
	}

	/******************************************************************/
	// @brief: check whether num is present in col or not
	// @ret: return true if successful
	/******************************************************************/
	bool isPresentInCol(int col, int num)
	{ 
		for (int row = 0; row < N; row++)
			if (grid[row][col] == num)
				return true;
		return false;
	}
	
	/******************************************************************/
	// @brief: check whether num is present in row or not
	// @ret: return true if successful
	/******************************************************************/
	bool isPresentInRow(int row, int num)
	{
		for (int col = 0; col < N; col++)
			if (grid[row][col] == num)
				return true;
		return false;
	}

	/******************************************************************/
	// @brief: check whether num is present in 3x3 box or not
	// @ret: return true if successful
	/******************************************************************/
	bool isPresentInBox(int boxStartRow, int boxStartCol, int num)
	{
		for (int row = 0; row < 3; row++)
			for (int col = 0; col < 3; col++)
				if (grid[row + boxStartRow][col + boxStartCol] == num)
					return true;
		return false;
	}

	/******************************************************************/
	// @brief: get empty location and update row and column
	// @ret: return true if successful
	/******************************************************************/
	bool findEmptyPlace(int& row, int& col)
	{ 
		for (row = 0; row < N; row++)
			for (col = 0; col < N; col++)
				if (grid[row][col] == 0) //marked with 0 is empty
					return true;
		return false;
	}
	
	/******************************************************************/
	// @brief: when item not found in col, row and current 3x3 box
	// @ret: return true if successful
	/******************************************************************/
	bool isValidPlace(int row, int col, int num)
	{
		return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row % 3,
			col - col % 3, num);
	}
};

/******************************************************************/
// @brief: called by threads to solve puzzles
/******************************************************************/
void solveSudokuPuzzles()
{
	SudokuGrid sudokuGrid;
	do
	{
		inFileMutex.lock();
		inFile >> sudokuGrid;
		inFileMutex.unlock();

		if (sudokuGrid.solve() == false)
		{
			cout << "No solution exists" << endl;
		}

		outFileMutex.lock();
		outFile << sudokuGrid;
		outFileMutex.unlock();
	} while (!inFile.eof());
	return;
}

int main(int argc, char* argv[])
{
	chrono::time_point<chrono::system_clock> statTime, endTime;
	statTime = chrono::system_clock::now();
	inFile.open(argv[1], ios::in);
	outFile.open(argv[2], ios::out);

	int numThreads = thread::hardware_concurrency();
	vector<thread> threads(numThreads-1);
	cout << "numThread: " << numThreads << endl;

	for (int i = 0; i < numThreads-1; i++)
	{
		threads[i] = thread(solveSudokuPuzzles);
	}

	for (int i = 0; i < numThreads-1; i++)
	{
		threads[i].join();
	}

	inFile.close();
	outFile.close();

	endTime = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = endTime - statTime;

	cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}