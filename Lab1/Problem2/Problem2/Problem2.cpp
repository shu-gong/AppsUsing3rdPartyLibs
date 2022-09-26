#include <iostream>
#include <mutex>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;
using namespace chrono;

#define N 9
mutex outFileMutex;
mutex inFileMutex;
fstream outFile;
fstream inFile;

class SudokuGrid
{
public:
	string m_strGridName;
	unsigned char gridElement[9][9];
	int grid[9][9];

	friend fstream& operator>>(fstream& os, SudokuGrid& gridIn)
	{
		string buf;
		getline(os, buf);
		if (!isdigit(buf[0]))
		{
			gridIn.m_strGridName = buf;
		}

		for (int i = 0; i < 9; i++)
		{
			getline(os, buf);
			for (int j = 0; j < 9; j++)
			{
				gridIn.grid[i][j] = buf[j] - '0';
			}
		}
		return os;
	}

	friend fstream& operator<<(fstream& os, const SudokuGrid& gridIn)
	{
		os << gridIn.m_strGridName << endl;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				os << gridIn.grid[i][j];
			}
			os << endl;
		}
		return os;
	}

	bool solve()
	{
		int row, col;
		if (!findEmptyPlace(row, col))
			return true; //when all places are filled
		for (int num = 1; num <= 9; num++)
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
	bool isPresentInCol(int col, int num)
	{ //check whether num is present in col or not
		for (int row = 0; row < N; row++)
			if (grid[row][col] == num)
				return true;
		return false;
	}
	bool isPresentInRow(int row, int num)
	{ //check whether num is present in row or not
		for (int col = 0; col < N; col++)
			if (grid[row][col] == num)
				return true;
		return false;
	}
	bool isPresentInBox(int boxStartRow, int boxStartCol, int num)
	{
		//check whether num is present in 3x3 box or not
		for (int row = 0; row < 3; row++)
			for (int col = 0; col < 3; col++)
				if (grid[row + boxStartRow][col + boxStartCol] == num)
					return true;
		return false;
	}
	bool findEmptyPlace(int& row, int& col)
	{ //get empty location and update row and column
		for (row = 0; row < N; row++)
			for (col = 0; col < N; col++)
				if (grid[row][col] == 0) //marked with 0 is empty
					return true;
		return false;
	}
	bool isValidPlace(int row, int col, int num)
	{
		//when item not found in col, row and current 3x3 box
		return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row % 3,
			col - col % 3, num);
	}
};


void solveSudokuPuzzles()
{
	SudokuGrid sudokuGrid;
	do
	{
		inFileMutex.lock();
		inFile >> sudokuGrid;
		inFileMutex.unlock();

		if (sudokuGrid.solve() == true);
		//sudokuGrid.sudokuGrid();
		else
			cout << "No solution exists";

		outFileMutex.lock();
		outFile << sudokuGrid;
		outFileMutex.unlock();

	} while (!inFile.eof());
}


int main()
{
	chrono::time_point<chrono::system_clock> statTime, endTime;
	statTime = chrono::system_clock::now();
	inFile.open("input_sudoku.txt", ios::in);
	outFile.open("result.txt", ios::out);

	int numThreads = thread::hardware_concurrency();
	vector<thread> threads(numThreads);
	cout << "numThread: " << numThreads << endl;
	for (int i = 0; i < numThreads; i++)
	{
		threads[i] = thread(solveSudokuPuzzles);
	}

	for (int i = 0; i < numThreads; i++)
	{
		threads[i].join();
	}
	inFile.close();
	outFile.close();
	endTime = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = endTime - statTime;

	cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}