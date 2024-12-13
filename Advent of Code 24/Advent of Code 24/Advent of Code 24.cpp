// Advent of Code 24.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <numeric>
#include <list>

using namespace std;

void day13();
void day12();
void day11part2();
void day11();
void day10();
void day9();
void day8();
void day7();
void day6();
void day5();
void day4();
void day4part2();


int main()
{
    day13();
}

void day13()
{
    uint64_t acc = 0;
    {
        ifstream f{ "Day13.txt" };

        string line;

        while (true) {

            vector<vector<int64_t>> m(2, {});
            vector<int64_t> target;
            
            for (size_t i = 0; i < 2; i++)
            {
                std::getline(f, line);
                stringstream ss{ line };
                string s;
                int x = 0, y = 0;
                getline(ss, s, '+');
                getline(ss, s, ',');
                m[0].push_back(stoi(s));

                getline(ss, s, '+');
                getline(ss, s, ',');
                m[1].push_back(stoi(s));

            }


            std::getline(f, line);
            stringstream ss{ line };
            string s;
            int x = 0, y = 0;
            getline(ss, s, '=');
            getline(ss, s, ',');
            target.push_back(stoi(s)+ 10000000000000);

            getline(ss, s, '=');
            getline(ss, s, ',');
            target.push_back(stoi(s)+ 10000000000000);

            int det = m[0][0] * m[1][1] - m[1][0] * m[0][1];

            if (det == 0) // linearly dependent
            {
                if (target[0] / m[0][0] == target[1] / m[1][1] && target[0] % m[0][1] == 0) // In the span of the matrix
                {
                    if (target[0] % m[0][0] == 0 && target[0] / m[0][0] * 3 < target[0] / m[0][1])
                    {
                        acc += 3 * target[0] / m[0][0];
                    }
                    else
                    {
                        acc += target[0] / m[0][0];
                    }
                }
            }
            else {

                int64_t a = (m[1][1] * target[0] + -m[0][1] * target[1]) / det; /// Using inverse matrix
                int64_t b = (-m[1][0] * target[0] + m[0][0] * target[1]) / det;

                if (a * m[0][0] + b * m[0][1] == target[0] && a * m[1][0] + b * m[1][1] == target[1])
                    acc += a * 3 + b;
            }

            if (!getline(f, line))
                break;

        }
    }
    std::cout << acc << endl;
}

pair<int, int> day12CalculateSection(vector<vector<char>>& matrix, vector<vector<bool>>& covered, std::multiset<pair<int,int>>& outside, int x, int y)
{
    char plot = matrix[y][x];
    int perimeter = 0;
    int area = 1;
    covered[y][x] = true;


    int startx = x, starty = y;
    int dx = 1, dy = 0;

    for (size_t _ = 0; _ < 4; _++)
    {
        x = startx + dx;
        y = starty + dy;
        if (x < 0 || y < 0 || y >= matrix.size() || x >= matrix[y].size()) // On edge
        {
            perimeter++;
            outside.insert({ startx*4+dx,starty*4+dy });
        }
        else if (matrix[y][x] == plot)
        {
            if (!covered[y][x])
            {
                auto res = day12CalculateSection(matrix, covered, outside, x, y);
                perimeter += res.first;
                area += res.second;
            }
        }
        else {
            perimeter++;
            outside.insert({ startx * 4 + dx, starty * 4 + dy });
        }

        int olddx = dx;
        dx = dy;
        dy = -olddx;
    }

    return {perimeter, area};
    
}

void day12()
{

    vector<vector<char>> matrix;
    {
        ifstream f{ "Day12.txt" };

        string line;

        while (std::getline(f, line)) {
            matrix.emplace_back(line.begin(), line.end());
        }
    }

    vector<vector<bool>> covered(matrix.size(), vector<bool>(matrix.back().size(), false));
    std::multiset<pair<int, int>> outside; // Multiples of four needed to differentiate edge directions

    uint64_t acc = 0;
    uint64_t acc2 = 0;
    for (size_t ys = 0; ys < matrix.size(); ys++)
    {
        for (size_t xs = 0; xs < matrix[ys].size(); xs++)
        {
            if (!covered[ys][xs])
            {
                auto res = day12CalculateSection(matrix, covered, outside, xs, ys);
                acc += res.first * res.second;

                int nBorders = 0;

                while (outside.size() > 0) // Remove adjacent neighbours
                {
                    auto ele = outside.begin();
                    auto [x, y] = *ele;
                    outside.erase(ele);
                    nBorders++;

                    if (outside.count({ x,y - 4 }) || outside.count({ x,y + 4 }))
                    {
                        int curry = y-4;
                        while (outside.count({ x,curry}))
                        {
                            outside.erase(outside.find({ x,curry }));
                            curry-=4;
                        }

                        curry = y + 4;
                        while (outside.count({ x,curry}))
                        {
                            outside.erase(outside.find({ x,curry}));
                            curry+=4;
                        }
                    }
                    else if (outside.count({ x-4,y }) || outside.count({ x+4,y }))
                    {
                        int currx = x - 4;
                        while (outside.count({ currx,y }))
                        {
                            outside.erase(outside.find({ currx,y }));
                            currx-=4;
                        }

                        currx = x + 4;
                        while (outside.count({ currx,y }))
                        {
                            outside.erase(outside.find({ currx,y }));
                            currx+=4;
                        }
                    }
                }

                acc2 += res.second * nBorders;
            }
        }
    }
    std::cout << acc2 << endl;
}

void day11part2()
{
    // Let's be smarter about this
    unordered_map<uint64_t, uint64_t> nEachStone;
    {
        ifstream f{ "Day11.txt" };

        string line;
        while (std::getline(f, line))
        {
            stringstream ss(line);
            int num = 0;
            while (ss >> num)
                nEachStone[num] += 1;
        }
    }

    for (size_t blink = 0; blink < 75; blink++)
    {
        unordered_map<uint64_t, uint64_t> nEachNext;
        auto it = nEachStone.begin();
        for (; it != nEachStone.end(); it++)
        {
            string numstring = std::to_string(it->first);
            if (numstring.size() % 2 == 0)
            {
                nEachNext[stoll(numstring.substr(0, numstring.size() / 2))] += it->second;
                nEachNext[stoll(numstring.substr(numstring.size() / 2))] += it->second;
            }
            else if (it->first == 0)
            {
                nEachNext[1] += it->second;
            }
            else
            {
                nEachNext[it->first*2024] += it->second;
            }
        }
        nEachStone = std::move(nEachNext);
        /*for (auto& i : nums)
        {
            std::cout << i << " ";
        }*/
        uint64_t acc = 0;
        for (auto& i : nEachStone)
        {
            acc += i.second;
        }
        cout << blink << " " << acc << endl;
    }
    
}

void day11()
{
    list<uint64_t> nums;
    unordered_map<int, vector<int>> rules;
    {
        ifstream f{ "Day11.txt" };

        string line;
        while (std::getline(f, line))
        {
            stringstream ss(line);
            int num = 0;
            while (ss >> num)
                nums.push_back(num);
        }
    }

    for (size_t blink = 0; blink < 25; blink++)
    {
        auto it = nums.begin();
        for (; it != nums.end(); it++)
        {
            string numstring = std::to_string(*it);
            if (numstring.size() % 2 == 0)
            {
                *it = stoll(numstring.substr(0, numstring.size() / 2));
                it++;
                it = nums.insert(it, stoi(numstring.substr(numstring.size() / 2)));
            }
            else if (*it == 0)
            {
                *it = 1;
            }
            else 
            {
                *it *= 2024;
            }
        }
        /*for (auto& i : nums)
        {
            std::cout << i << " ";
        }*/
        cout << blink << " " << nums.size() << endl;
    }
}

int check_trail(const vector<vector<int>>& matrix, int x, int y, set<pair<int,int>>& checked)
{
    int height = matrix[y][x];

    if (height == 9)
    {
        //if (checked.count({ x,y }))
        //    return 0;
        //checked.insert({ x,y });
        return 1;
    }
    int acc = 0;
    if (y > 0 && matrix[y-1][x] == height + 1)
        acc += check_trail(matrix, x, y - 1, checked);
    if (x > 0 && matrix[y][x - 1] == height + 1)
        acc += check_trail(matrix, x - 1, y, checked);
    if (y < matrix.size() - 1 && matrix[y + 1][x] == height + 1)
        acc += check_trail(matrix, x, y + 1, checked);
    if (x < matrix[y].size() - 1 && matrix[y][x + 1] == height + 1)
        acc += check_trail(matrix, x + 1, y, checked);

    return acc;
}

void day10()
{

    vector<vector<int>> matrix;
    {
        ifstream f{ "Day10.txt" };

        string line;

        while (std::getline(f, line)) {
            matrix.push_back({});
            for (auto& i : line)
            {
                matrix.back().push_back(i - '0');
            }
        }
    }

    set<pair<int, int>> start_points;

    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size(); x++)
        {
            if (matrix[y][x] == 0)
            {
                start_points.insert({ x,y });
            }
        }
    }
    int acc = 0;

    for (auto& start : start_points)
    {
        set<pair<int, int>> checked;
        acc += check_trail(matrix, start.first, start.second,checked);
    }

    std::cout << acc << endl;
}

void day9()
{
    vector<int> nums;
    {
        ifstream f{ "Day9.txt" };

        std::istream_iterator<char> start(f), end;

        for (auto it = move(start); it != end; it++)
        {
            nums.push_back(*it - '0');
        }
    }

    uint64_t acc = 0;
    int pos = 0;
    vector<bool> moved(nums.size(),false);

    for (size_t i = 0; i < nums.size(); i++)
    {
        if (i % 2 == 0) 
        {
            for (size_t j = 0; j < nums[i]; j++)
            {
                if (!moved[i])
                    acc += pos * (i / 2);
                pos += 1;
            }
        }
        else
        {
            int j = nums.size() - 1;
            if (j % 2 == 1)
                j--;
            int remaining_space = nums[i];
            while (remaining_space > 0)
            {

                while ( j > i && (moved[j] || remaining_space < nums[j]))
                    j-=2;

                moved[j] = true;
                for (size_t n = 0; n < nums[j]; n++)
                {
                    remaining_space--;
                    acc += pos * (j / 2);
                    pos += 1;
                }
            }
            pos += remaining_space;
        }
    }
    std::cout << acc << endl;
}

void day8()
{
    vector<vector<char>> matrix;
    {
        ifstream f{ "Day8.txt" };

        string line;
        while (std::getline(f, line))
            matrix.emplace_back(line.begin(), line.end());
    }
    bool found = false;
    int currx = 0, curry = 0;

    unordered_map<char, vector<pair<int, int>>> stations;

    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size(); x++)
        {
            if (matrix[y][x] != '.')
            {
                stations[matrix[y][x]].push_back({ x,y });
            }
        }
    }

    auto validBounds = [&](int x, int y) { return y >= 0 && y < matrix.size() && x >= 0 && x < matrix[y].size(); };

    for (auto& val : stations)
    {
        auto& station = val.second;
        for (size_t i = 0; i < station.size()-1; i++)
        {
            for (size_t j = i+1; j < station.size(); j++)
            {
                int dx = station[i].first - station[j].first;
                int dy = station[i].second - station[j].second;

                int step_gcd = std::gcd(dx, dy);
                dx /= step_gcd;
                dy /= step_gcd;

                int x = station[i].first, y = station[i].second;

                while (validBounds(x, y))
                {
                    matrix[y][x] = '#';
                    x += dx;
                    y += dy;
                }

                x = station[i].first, y = station[i].second;
                while (validBounds(x, y))
                {
                    matrix[y][x] = '#';
                    x -= dx;
                    y -= dy;
                }
                // Part I
                /*int x = station[i].first + dx, y = station[i].second + dy;
                if (validBounds(x, y))
                    matrix[y][x] = '#';

                x = station[j].first - dx, y = station[j].second - dy;
                if (validBounds(x, y))
                    matrix[y][x] = '#';*/
            }

        }
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
        nCount += count(i.begin(), i.end(), '#');
    }
    std::cout << "Count " << nCount << endl;
}

bool day7Valid(uint64_t target, uint64_t currval, const vector<int>& nextNums, int vecInd)
{
    if (nextNums.size()-1 == vecInd)
    {
        if (target == currval + nextNums[vecInd])
            return true;
        if (target == currval * nextNums[vecInd])
            return true;;

        string concat = to_string(currval) + to_string(nextNums[vecInd]);
        return target == stoll(concat);
    }

    uint64_t addVal = currval + nextNums[vecInd];
    if (day7Valid(target, addVal, nextNums, vecInd + 1))
        return true;
    uint64_t multVal = currval * nextNums[vecInd];
    if (day7Valid(target, multVal, nextNums, vecInd + 1))
        return true;

    string concat = to_string(currval) + to_string(nextNums[vecInd]);
    return (day7Valid(target, stoll(concat), nextNums, vecInd + 1));

    
}

void day7()
{
    uint64_t nValid = 0;
    {
        ifstream f{ "Day7.txt" };

        string line;
        while (std::getline(f, line))
        {

            stringstream ss(line);
            string targetstr;
            ss >> targetstr;
            targetstr = targetstr.substr(0,targetstr.size() - 1);
            uint64_t target = stoll(targetstr);

            vector<int> nums;

            string s;
            while (ss >> s)
            {
                nums.push_back(stoi(s));
            }

            if (day7Valid(target, nums.front(), nums, 1))
                nValid+= target;
        }
    }
    std::cout << nValid << endl;
}

bool day6CheckLoop(const vector<vector<char>>& map, int startx, int starty, int dx, int dy)
{
    auto& matrix = map;
    int currx = startx;
    int curry = starty;
    std::map<pair<int, int>, vector<int>> previous_states;
    while (curry >= 0 && curry < matrix.size() && currx >= 0 && currx < matrix[curry].size())
    {
        int state = 0;
        state = dx == -1 ?  1  : state;
        state = dy == 1 ? 2: state;
        state = dy == -1 ? 3 : state;

        auto& position_states = previous_states[{currx, curry}];

        if (find(position_states.begin(), position_states.end(), state) != position_states.end())
        {
            return true; // Loop found
        }
        position_states.push_back(state);

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
        else
        {
            curry += dy;
            currx += dx;
        }
    }
    return false;
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

    int startx = currx, starty = curry;

    int dx = 0, dy = -1;
    set<pair<int, int>> loops, beenThere;

    while (curry >= 0 && curry < matrix.size() && currx >= 0 && currx < matrix[curry].size())
    {
        matrix[curry][currx] = 'X';
        beenThere.insert({ currx, curry });
        int nextx = currx + dx, nexty = curry + dy;

        if (nexty >= 0 && nexty < matrix.size() && nextx >= 0 && nextx < matrix[nexty].size())
        {
            if (matrix[nexty][nextx] == '#')
            {
                int olddx = dx;
                dx = -dy;
                dy = olddx;
                //if (dy == -1)
                    //break;
            }
            else
            {
                if ((nexty != starty || nextx != startx) && loops.find({ nextx, nexty }) == loops.end() && beenThere.find({ nextx, nexty }) == beenThere.end())
                {
                    matrix[nexty][nextx] = '#';
                    if (day6CheckLoop(matrix, currx, curry, dx, dy))
                    {
                        loops.insert({ nextx, nexty });
                    }
                    matrix[nexty][nextx] = '.';
                }
                curry += dy;
                currx += dx;
            }
        }
        else
        {
            curry += dy;
            currx += dx;
        }
    }
    /*for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size(); x++)
        {
            cout << matrix[y][x];
        }
        cout << endl;
    }*/

    int nCount = 0;
    for (auto& i : matrix)
    {
        nCount += count(i.begin(), i.end(), 'X');
    }
    std::cout << "Count " << nCount << endl;
    std::cout << "Loops " << loops.size() << endl;
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
