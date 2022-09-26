/*
Author: Shu Gong
Class: ECE6122 QSZ
Last Date Modified: 9/26/2022

Description:

The write a console program that continuously takes in a number n from the console and outputs to
the console the number of 1's that must be added to the positive integer n before the process
above ends.

*/

#include <iostream>
#include <vector>
using namespace std;

/******************************************************************/
// @brief: find the number of paths
// @para: nRows: number of rows
//		  nCols: number of cols
// @ret: return total number of paths
/******************************************************************/
uint64_t numberGridPaths(unsigned int nRows, unsigned int nCols)
{
    vector<vector<unsigned int>> maze;
    vector<unsigned int> vect;

    if (nRows == 0 || nCols == 0)
    {
        return 0;
    }
    else
    {
        for (int j = 0; j < nCols; j++)
        {
            vect.push_back(0);
        }

        for (int i = 0; i < nRows; i++)
        {
            maze.push_back(vect);
        }

        for (int i = 0; i < maze.size(); i++)
        {
            for (int j = 0; j < maze[i].size(); j++)
            {
                if (i == 0 && j == 0)
                {
                    maze[i][j] = 1;
                }
                else if (i == 0 && j != 0)
                {
                    maze[i][j] = maze[i][j - 1];
                }
                else if (i != 0 && j == 0)
                {
                    maze[i][j] = maze[i - 1][j];
                }
                else
                {
                    maze[i][j] = maze[i - 1][j] + maze[i][j - 1];
                }
            }
        }
        return maze.back().back();
    }
}
