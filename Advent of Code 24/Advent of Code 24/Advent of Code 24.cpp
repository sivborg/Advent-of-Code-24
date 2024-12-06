// Advent of Code 24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

void day4();
void day4part2();

int main()
{
    day4part2();
}

void day4part2()
{
    vector<vector<char>> matrix;
    {
        ifstream f{ "Day4.txt" };

        string line;
        while (std::getline(f, line))
            matrix.emplace_back(line.begin(), line.end());
    }

    int nFound = 0;

    for (size_t i = 1; i < matrix.size()-1; i++)
    {
        for (size_t j = 1; j < matrix[i].size()-1; j++)
        {
            if (matrix[i][j] != 'A')
                continue;

            if ((matrix[i + 1][j + 1] == 'M' || matrix[i + 1][j + 1] == 'S') &&
                (matrix[i - 1][j - 1] == 'M' || matrix[i - 1][j - 1] == 'S') &&
                (matrix[i + 1][j + 1] != matrix[i - 1][j - 1]) &&
                (matrix[i + 1][j - 1] == 'M' || matrix[i + 1][j - 1] == 'S') &&
                (matrix[i - 1][j + 1] == 'M' || matrix[i - 1][j + 1] == 'S') &&
                (matrix[i + 1][j - 1] != matrix[i - 1][j + 1])
                )
                nFound += 1;
        }
    }
    cout << "X-MAS FOUND: " << nFound << endl;

}

void day4()
{
    vector<vector<char>> matrix;
    {
        ifstream f{ "Day4.txt" };

        string line;
        while (std::getline(f, line))
            matrix.emplace_back(line.begin(), line.end());
    }

    string word = "XMAS";
    int nFound = 0;

    for (size_t i = 0; i < matrix.size(); i++)
    {
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            if (matrix[i][j] != 'X')
                continue;

            for (int dx = -1; dx < 2; dx++)
            {
                for (int dy = -1; dy < 2; dy++)
                {
                    if (dy == dx && dy == 0)
                        continue;

                    bool found = true;

                    int currx = j, curry = i;
                    int state = 1;

                    while (state < 4)
                    {
                        currx += dx;
                        curry += dy;

                        if (currx < 0 || curry < 0 || curry >= matrix.size() || currx >= matrix[curry].size() || matrix[curry][currx] != word[state])
                        {
                            break;
                        }
                        else {
                            state += 1;
                        }
                    }

                    if (state == 4)
                        nFound++;
                }
            }
        }
    }
    cout << "XMAS FOUND: " << nFound << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
