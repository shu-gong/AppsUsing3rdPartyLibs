/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 10/9/2022

Description:

Write a C++ application that can read in two matrices (A & B) 
from a text file using a command line argument to specify the
file path and name
*/

#include <iostream>
#include <omp.h>
#include <vector>
#include <ctime>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

vector<vector<double>> matrixMultiplication(vector<vector<double>>& matrixLeft, vector<vector<double>>& matrixRight);
void readFile(vector<vector<double>>& matrixLeft, vector<vector<double>>& matrixRight);
fstream inFile;
fstream outFile;

int main(int argc, char* argv[])
{
	inFile.open(argv[1], ios::in);
	outFile.open("MatrixOut.txt", ios::out);

	vector<vector<double>> matrixLeft;
	vector<vector<double>> matrixRight;

	readFile(matrixLeft, matrixRight);

	chrono::time_point<chrono::system_clock> statTime, endTime;
	statTime = chrono::system_clock::now();

	vector<vector<double>> returnMatrix = matrixMultiplication(matrixLeft, matrixRight);

	endTime = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = endTime - statTime;
	cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	outFile << returnMatrix.size() << " " << returnMatrix[0].size() << endl;

	for (int i = 0; i < returnMatrix.size(); i++)
	{
		for (int j = 0; j < returnMatrix[0].size(); j++)
		{
			outFile << returnMatrix[i][j] << " ";
		}
		outFile << endl;
	}

	inFile.close();
	outFile.close();
}

/******************************************************************/
// @brief: standard matrix multiplication for two matrices
// @para: two matrices
// @ret: return the output matrix
/******************************************************************/
vector<vector<double>> matrixMultiplication(vector<vector<double>>& matrixLeft, vector<vector<double>>& matrixRight)
{
	if (matrixLeft[0].size() != matrixRight.size())
	{
		cout << "Matrix Size Illegal" << endl;
	}
	else
	{
		vector<double> vect;
		vector<vector<double>> retMatrix;
		int nRows = matrixLeft.size();
		int nCols = matrixRight[0].size();

		for (int j = 0; j < nCols; j++)
		{
			vect.push_back(0);
		}

		for (int i = 0; i < nRows; i++)
		{
			retMatrix.push_back(vect);
		}
#pragma omp parallel for
		for (int row = 0; row < nRows; row++)
		{
			for (int col = 0; col < nCols; col++)
			{
				for (int j = 0; j < matrixLeft[0].size(); j++)
				{
					retMatrix[row][col] += matrixLeft[row][j] * matrixRight[j][col];
				}
			}
		}
		return retMatrix;
	}
}

/******************************************************************/
// @brief: read two matrices from inFile
// @para: global variables of the two matrices
/******************************************************************/
void readFile(vector<vector<double>>& matrixLeft, vector<vector<double>>& matrixRight)
{
	int numMatrixLeftRow, numMatrixLeftCol, numMatrixRightRow, numMatrixRightCol;

	// read the number of rows and cols of the left matrix
	string buf;
	inFile >> buf;
	numMatrixLeftRow = stoi(buf);
	inFile >> buf;
	numMatrixLeftCol = stoi(buf);

	// read the elements of the left matrix
	vector<double> vect;
	for (int i = 0; i < numMatrixLeftRow; i++)
	{
		vect = { };
		for (int j = 0; j < numMatrixLeftCol; j++)
		{
			inFile >> buf;
			vect.push_back(stod(buf));
		}
		matrixLeft.push_back(vect);
	}

	// read the number of rows and cols of the right matrix
	inFile >> buf;
	numMatrixRightRow = stoi(buf);
	inFile >> buf;
	numMatrixRightCol = stoi(buf);

	// read the elements of the right matrix
	for (int i = 0; i < numMatrixRightRow; i++)
	{
		vect = { };
		for (int j = 0; j < numMatrixRightCol; j++)
		{
			inFile >> buf;
			vect.push_back(stod(buf));
		}
		matrixRight.push_back(vect);
	}
}