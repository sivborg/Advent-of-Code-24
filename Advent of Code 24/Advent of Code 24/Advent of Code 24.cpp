// Advent of Code 24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

void day6();
void day5();
void day4();
void day4part2();

int main()
{
    day6();
}

void day6()
{
    vector<vector<char>> matrix;
    {
        ifstream f{ "Day6.txt" };

        string line;
        while (std::getline(f, line))
            matrix.emplace_back(line.begin(), line.end());
    }
    bool found = false;
    int currx = 0, curry = 0;
       
    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size(); x++)
        {
            if (matrix[y][x] == '^')
            {
                currx = x;
                curry = y;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    int dx = 0, dy = -1;

    while (curry >= 0 && curry < matrix.size() && currx >= 0 && currx < matrix[curry].size())
    {
        matrix[curry][currx] = 'X';
        int nextx = currx + dx, nexty = curry + dy;

        if (nexty >= 0 && nexty < matrix.size() && nextx >= 0 && nextx < matrix[nexty].size() &&
            matrix[nexty][nextx] == '#')
        {
            int olddx = dx;
            dx = -dy;
            dy = olddx;
            //if (dy == -1)
                //break;
        }
        curry += dy;
        currx += dx;
    }
    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size(); x++)
        {
            cout << matrix[y][x];
        }
        cout << endl;
    }

    int nCount = 0;
    for (auto& i : matrix)
    {
        nCount += count(i.begin(), i.end(), 'X');
    }
    std::cout << "Count " << nCount << endl;
}

void day5()
{
    int acc = 0, acc2 = 0;
    unordered_map<int, vector<int>> rules;
    {
        ifstream f{ "Day5.txt" };

        string line;
        while (std::getline(f, line))
        {
            if (line.empty() || line.find(",") != string::npos) // Done with rules
                break;

            stringstream ss(line);
            string s;
            std::getline(ss, s, '|');
            int num1 = stoi(s);
            std::getline(ss, s, '|');
            int num2 = stoi(s);

            rules[num1].push_back(num2);
        }

        while (std::getline(f, line))
        {
            unordered_set<int> existing;
            vector<int> pages;

            stringstream ss(line);
            string s;
            while (std::getline(ss, s, ','))
            {
                pages.push_back(stoi(s));
            }

            bool valid = true;
            for (auto& i : pages)
            {
                if (existing.count(i))
                    continue;
                existing.insert(i);

                if (rules.count(i) == 0)
                    continue;

                for (auto& forbidden : rules[i])
                {
                    if (existing.count(forbidden))
                    {
                        valid = false;
                        break;
                    }
                }
                if (!valid)
                    break;
            }
            if (valid)
            {
                acc += pages[pages.size() / 2];
            }
            else
            {
                existing.clear();
                existing.insert(pages.begin(), pages.end());

                int totalnums = pages.size();
                pages.clear();
                while (pages.size() <= (totalnums / 2))
                {
                    for (auto& i : existing)
                    {
                        bool valid = true;
                        for (auto& j : existing)
                        {
                            if (i == j)
                                continue;
                            if (find(rules[j].begin(), rules[j].end(), i) != rules[j].end())
                            {
                                valid = false;
                                break;
                            }
                        }
                        if (valid)
                        {
                            pages.push_back(i);
                            existing.erase(i);
                            break;
                        }
                    }
                }
                acc2 += pages.back();
            }
        }
    }
    cout << acc << " " << acc2 << endl;
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
