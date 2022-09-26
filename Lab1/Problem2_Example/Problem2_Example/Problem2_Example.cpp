#include <iostream>
#include <String>
#include <mutex>
#include <fstream>
#include <thread>

#define N 9
using namespace std;

fstream outFile;
fstream inFile;

mutex outFileMutex;
mutex inFileMutex;

int grid[N][N] = {
   {3, 0, 6, 5, 0, 8, 4, 0, 0},
   {5, 2, 0, 0, 0, 0, 0, 0, 0},
   {0, 8, 7, 0, 0, 0, 0, 3, 1},
   {0, 0, 3, 0, 1, 0, 0, 8, 0},
   {9, 0, 0, 8, 6, 3, 0, 0, 5},
   {0, 5, 0, 0, 9, 0, 6, 0, 0},
   {1, 3, 0, 0, 0, 0, 2, 5, 0},
   {0, 0, 0, 0, 0, 0, 0, 7, 4},
   {0, 0, 5, 2, 0, 6, 3, 0, 0}
};

bool readFile(int(&grid)[N][N], fstream& inFile)
{
	if (!inFile.is_open())
	{
		cout << "file open fail!";
		return false;
	}
	int rowNum = 0;
	char buf[1024] = {0};
	while (inFile.getline(buf, sizeof(buf)))
	{
		if (isdigit(buf[0]))
		{
			for (int i = 0; i < N; i++)
			{
				grid[rowNum][i] = static_cast<int>(buf[i])-'0';
			}
			//grid[rowNum][N] = '/0';
			rowNum += 1;
		}
		if (rowNum == N)
		{
			break;
		}
	}
	return true;
}
bool writeFile(const int(&grid)[N][N], fstream& outFile, mutex& outFileMutex)
{

	if (!outFile.is_open())
	{
		cout << "file open fail!";
		return false;
	}

	for (int row = 0; row < N; row++)
	{
		for (int col = 0; col < N; col++)
		{
			if (col == 3 || col == 6)
				outFile << " | ";
			outFile << grid[row][col] << " ";
		}
		if (row == 2 || row == 5)
		{
			outFile << endl;
			for (int i = 0; i < N; i++)
				outFile << "---";
		}
		outFile << endl;
	}

	return true;
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
void sudokuGrid()
{ //print the sudoku grid after solve
	for (int row = 0; row < N; row++)
	{
		for (int col = 0; col < N; col++)
		{
			if (col == 3 || col == 6)
				cout << " | ";
			cout << grid[row][col] << " ";
		}
		if (row == 2 || row == 5)
		{
			cout << endl;
			for (int i = 0; i < N; i++)
				cout << "---";
		}
		cout << endl;
	}
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
bool solveSudoku()
{
	int row, col;
	if (!findEmptyPlace(row, col))
		return true; //when all places are filled
	for (int num = 1; num <= 9; num++)
	{ //valid numbers are 1 - 9
		if (isValidPlace(row, col, num))
		{ //check validation, if yes, put the number in the grid
			grid[row][col] = num;
			if (solveSudoku()) //recursively go for other rooms in the grid
				return true;
			grid[row][col] = 0; //turn to unassigned space when conditions are not satisfied
		}
	}
	return false;
}

void solveThread(int (&grid)[N][N], fstream& inFile, fstream& outFile, mutex& inFileMutex, mutex& outFileMutex)
{
	while (inFile.peek() != EOF)
	{

		inFileMutex.lock();
		if (readFile(grid, inFile))
		{
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					cout << grid[i][j];
				}
				cout << endl;
			}
		}
		inFileMutex.unlock();

		if (solveSudoku() == true)
			sudokuGrid();
		else
			cout << "No solution exists";

		outFileMutex.lock();
		writeFile(grid, outFile, outFileMutex);
		outFileMutex.unlock();

	}
}

int main()
{
	inFile.open("input_sudoku.txt", ios::in);
	outFile.open("result.txt", ios::out);

	int numThread = 5;

	thread t1(solveThread, ref(grid), ref(inFile), ref(outFile), ref(inFileMutex), ref(outFileMutex));
	//thread t2(solveThread, ref(grid), ref(inFile), ref(outFile), ref(inFileMutex), ref(outFileMutex));

	t1.join();
	//t2.join();
	
	inFile.close();
	outFile.close();
}

