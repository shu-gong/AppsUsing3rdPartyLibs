#include <iostream>
#include <Vector>
using namespace std;

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
