/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 9/26/2022

Description:

A researcher is running an experiment where a mouse is placed 
at the top left position on a m x n grid maze. The mouse can 
only either move 1 position to right or one position down at 
a time in order to reach a piece of cheese at the lower right 
corner of the grid

*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
bool cvtRaw2Valid(int argc, char* argv[], unsigned int& nRows, unsigned int& nCols);
uint64_t numberGridPaths(unsigned int nRows, unsigned int nCols);

int main(int argc, char* argv[])
{
	fstream outFile;
	outFile.open("NumberPaths.txt", ios::out);

	unsigned int nRows;
	unsigned int nCols;
	if (!cvtRaw2Valid(argc, argv, nRows, nCols))
	{
		outFile << "Invalid Input!";
	}
	else
	{
		outFile <<"Total Number Paths: "<< numberGridPaths(nRows, nCols);
	}

	outFile.close();
}

/******************************************************************/
// @brief: convert command line input to valid digit
// @para: argc: number of parameters
//        argv[]: content of parameters
//        nRows: number of rows
//		  nCols: number of cols
// @ret: return true if successfully
/******************************************************************/
bool cvtRaw2Valid(int argc, char* argv[], unsigned int& nRows, unsigned int& nCols)
{
	int digitPara[3] = { 0,0,0 };

	// determine the number of arguments
	if (argc != 3)
	{
		return false;
	}
	else
	{
		// convert raw to valid digit
		for (int i = 1; i < argc; i++)
		{
			string rawSeq = argv[i];
			string rawDigit;
			for (int j = 0; j < rawSeq.length(); j++)
			{
				// detect non-digit input
				if (isdigit(rawSeq[j]))
				{
					rawDigit += rawSeq[j];
				}
				else
				{
					return false;
				}
			}

			// avoid multiple zeros
			if (rawDigit[0] == '0' && rawDigit.length() > 1)
			{
				return false;
			}
			else
			{
				digitPara[i] = stoi(rawDigit);
			}
		}
		nRows = digitPara[1];
		nCols = digitPara[2];
		return true;
	}
}