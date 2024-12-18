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
#include <queue>
#include <chrono>

using namespace std;

void day18();
void day17();
void day16();
void day15part2();
void day15();
void day14part2();
void day14();
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
    auto then = chrono::steady_clock::now();
    day18();
    cout << "Time taken (s): " << chrono::duration<double>(chrono::steady_clock::now() - then).count() << endl;
}

int day18SolveMaze(const vector<string>& mymap)
{
    map<pair<int, int>, int> costs;
    costs[{0, 0}] = 0;
    int height = mymap.size();
    int width = mymap.back().size();

    priority_queue<pair<int, pair<int, int>>> nodes;

    nodes.emplace(make_pair(0, make_pair(0, 0)));
    int endCost = -1;

    while (!nodes.empty())
    {
        auto currNode = nodes.top();
        nodes.pop();

        if (costs.count(currNode.second) && costs[currNode.second] < currNode.first)
            continue; // Already visited

        if (endCost > 0 && currNode.first > endCost)
            continue;

        auto& n = currNode.second;

        if (n.first == width - 1 && n.second == height - 1 && (currNode.first < endCost || endCost < 0))
        {
            endCost = currNode.first;
            continue;
        }

        vector<pair<int, pair<int, int>>> nextCostAndNode;
        int dx = 1, dy = 0;
        for (size_t i = 0; i < 4; i++)
        {
            if (n.first + dx >= 0 && n.first + dx < width && n.second + dy >= 0 && n.second + dy < height)
                nextCostAndNode.push_back(make_pair(currNode.first + 1, make_pair(n.first + dx, n.second + dy)));
            int olddx = dx;
            dx = -dy;
            dy = olddx;
        };

        for (auto& [nextcost, nextnode] : nextCostAndNode)
        {
            if (mymap[nextnode.second][nextnode.first] == '.')
            {
                if ((costs.count(nextnode) == 0 || costs[nextnode] > nextcost))
                {
                    nodes.push({ nextcost, nextnode });
                    costs[nextnode] = nextcost;
                }
            }
        }

    }
    return endCost;

}

void day18()
{
    constexpr int height = 71, width = 71;
    vector<pair<int,int>> positions;
    {
        ifstream f{ "Day18.txt" };
        std::string line;

        while (std::getline(f, line))
        {
            stringstream ss{ line };
            string s;
            int x = 0, y = 0;
            getline(ss, s, ',');
            x = stoi(s);
            getline(ss, s, ',');
            y = stoi(s);

            positions.push_back({ x,y });
        }
    }

    int lowerBound = 1024;
    int upperBound = positions.size();

    bool found = false;
    int toCheck = 0;


    while (!found)
    {
        vector<string> mymap(height, string(width, '.'));
        toCheck = (lowerBound + upperBound) / 2;
        //cout << toCheck << endl;


        for (int i = 0; i < toCheck; i++)
        {
            auto p = positions[i];
            mymap[p.second][p.first] = '#';
        }

        if (day18SolveMaze(mymap) > -1)
        {
            auto p = positions[toCheck];
            mymap[p.second][p.first] = '#';
            if (day18SolveMaze(mymap) == -1)
            {
                found = true;
            }
            else
            {
                lowerBound = toCheck;
            }
        }

        else
            upperBound = toCheck;
    }

    cout <<"Found: " << positions[toCheck].first << "," << positions[toCheck].second << endl;
}

vector<int> day17RunProgram(int64_t a, int64_t b, int64_t c, vector<int>& instructions)
{
    vector<int> result;
    int currOutput = 0;
    for (size_t i = 0; i < instructions.size(); i += 2)
    {
        int instruction = instructions[i];
        int64_t operand = instructions[i + 1];
        if (instruction == 0 || instruction == 2 || instruction == 5 || instruction == 6 || instruction == 7) // Combo operand
        {
            if (operand == 4)
                operand = a;
            else if (operand == 5)
                operand = b;
            else if (operand == 6)
                operand = c;
        }

        switch (instruction)
        {
        case 0:
            a = a >> operand;
            break;
        case 1:
            b = b ^ operand;
            break;
        case 2:
            b = operand % 8;
            break;
        case 3:
            if (a != 0)
            {
                i = operand;
                i -= 2;
            }
            break;
        case 4:
            b = b ^ c;
            break;
        case 5:
            result.push_back(operand % 8);
            break;
        case 6:
            b = a >> operand;
            break;
        case 7:
            c = a >> operand;
            break;

        default:
            break;
        }
    }
    return result;
}

void day17()
{
    int64_t a = 0, b = 0, c = 0;
    vector<int> instructions;
    {
        ifstream f{ "Day17.txt" };
        std::string line;


        std::getline(f, line);
        stringstream ss{ line };
        string s;
        getline(ss, s, ':');
        getline(ss, s);
        a = stoll(s);

        std::getline(f, line);
        ss = stringstream{ line };
        getline(ss, s, ':');
        getline(ss, s);
        b = stoll(s);

        std::getline(f, line);
        ss = stringstream{ line };
        getline(ss, s, ':');
        getline(ss, s);
        c = stoll(s);


        std::getline(f, line);
        std::getline(f, line);
        ss = stringstream{ line };
        getline(ss, s, ':');
        while (getline(ss, s, ','))
        {
            instructions.push_back(stoi(s));
        }
    }

    // Oki let's not be dumdums
    a = 0;
    while (true)
    {
        auto result = day17RunProgram(a, b, c, instructions);
        
        bool matchSoFar = true;
        for (size_t i = 0; i < result.size(); i++)
        {
            matchSoFar &= result[result.size() - 1 - i] == instructions[instructions.size() - 1 - i];
        }

        if (matchSoFar)
        {
            if (result.size() == instructions.size())
                break;

            a = (a << 3);
        }
        else
            a++;

    }
    cout << a << endl;
    day17RunProgram(a, b, c, instructions);
}

void day16()
{
    vector<string> matrix;
    {
        ifstream f{ "Day16.txt" };

        string line;

        while (std::getline(f, line)) {
            matrix.push_back(line);
        }
    }

    pair<int, int> startpos, endpos;
    for (size_t y = 1; y < matrix.size(); y++)
    {
        if (matrix[y].find("S") != string::npos)
        {
            startpos = { matrix[y].find("S"), y };
            matrix[startpos.second][startpos.first] ='.';
        }
        if (matrix[y].find("E") != string::npos)
        {
            endpos = { matrix[y].find("E"), y };
            matrix[endpos.second][endpos.first] = '.';
        }
    }

    struct Node
    {
        int px = 0, py = 0, dx = 0, dy = 0;
        tuple<int, int, int, int> tup()
        {
            return { px,py,dx,dy };
        }
    };



    map<tuple<int,int,int,int>, int> costs;
    costs[{startpos.first, startpos.second, 1, 0}] = 0;


    auto compNode = [](pair<Node,int> l, pair<Node, int> r) {return l.second > r.second; };

    // Saves the previous nodes for each node in the optimal path
    map<tuple<int, int, int, int>, set<tuple<int, int, int, int>>> optimalPath; 

    priority_queue<pair<Node, int>, vector<pair<Node, int>>, decltype(compNode)> nodes(compNode);

    nodes.emplace(Node{ startpos.first, startpos.second, 1, 0 }, 0);
    int endCost = -1;
    int enddx = 0, enddy = 0;

    while (!nodes.empty())
    {
        auto currNode = nodes.top();
        nodes.pop();

        if (costs.count( currNode.first.tup()) && costs[currNode.first.tup()] < currNode.second)
            continue; // Already visited

        if (endCost > 0 && currNode.second > endCost)
            continue;

        Node& n = currNode.first;

        if (n.px == endpos.first && n.py == endpos.second && (currNode.second < endCost || endCost < 0))
        {
            endCost = currNode.second;
            enddx = n.dx;
            enddy = n.dy;
            continue;
        }

        vector<pair<int, Node>> nextCostAndNode = {
            {currNode.second + 1, { n.px + n.dx, n.py + n.dy, n.dx, n.dy }},
            {currNode.second + 1000, { n.px, n.py, -n.dy, n.dx }},
            {currNode.second + 1000, { n.px, n.py, n.dy, -n.dx }},
        };

        for (auto& [nextcost,nextnode] : nextCostAndNode)
        {
            if (matrix[nextnode.py][nextnode.px] == '.')
            {
                if ((costs.count(nextnode.tup()) == 0 || costs[nextnode.tup()] > nextcost))
                {
                    nodes.push({ nextnode , nextcost });
                    costs[nextnode.tup()] = nextcost;
                    optimalPath[nextnode.tup()] = { n.tup() };
                }
                else if (costs[nextnode.tup()] == nextcost)
                {
                    optimalPath[nextnode.tup()].insert(n.tup());
                }
            }
        }
    }

    queue<tuple<int, int, int, int>> nodesToCheck;
    nodesToCheck.push({ endpos.first,endpos.second, enddx, enddy});
    set<tuple<int, int, int, int>> optimalNodes;
    set<pair<int, int>> optimalPoses;
    while (!nodesToCheck.empty())
    {
        auto n = nodesToCheck.front();
        nodesToCheck.pop();
        if (optimalNodes.count(n))
            continue;
        matrix[get<1>(n)][get<0>(n)] = 'O';
        optimalNodes.insert(n);
        optimalPoses.insert({ get<0>(n), get<1>(n) });
        for (auto& nextNode : optimalPath[n])
        {
            nodesToCheck.push(nextNode);
        }
    }


    cout << endCost << " Num positions: " << optimalPoses.size() << endl;


    for (auto& i : matrix)
    {
        cout << i << endl;
    }
}

void day15part2()
{

    vector<string> matrix;
    string instructions = "";
    constexpr bool display = false;
    {
        ifstream f{ "Day15.txt" };

        string line;


        while (std::getline(f, line)) {
            if (line.empty())
                break;
            matrix.push_back("");
            for (auto c : line)
            {
                if (c == '.')
                    matrix.back() = matrix.back() + "..";
                else if (c == '#')
                    matrix.back() = matrix.back() + "##";
                else if (c == '@')
                    matrix.back() = matrix.back() + "@.";
                else if (c == 'O')
                    matrix.back() = matrix.back() + "[]";
            }
        }


        while (std::getline(f, line)) {
            instructions = instructions + line;
        }
    }

    pair<int, int> pos;

    int found = false;
    for (size_t y = 1; y < matrix.size(); y++)
    {
        if (matrix[y].find("@") != string::npos)
        {
            pos = { matrix[y].find("@"), y };
            found = true;
        }
        if (found)
            break;
    }

    if constexpr (display)
    {

        for (auto& i : matrix)
        {
            cout << i << endl;
        }
        matrix[pos.second][pos.first] = '.';
        char c;
        cin >> c;
    }
    matrix[pos.second][pos.first] = '.';

    for (auto ins : instructions)
    {
        int dx = 0;
        int dy = 0;
        if (ins == '^')
            dy = -1;
        else if (ins == 'v')
            dy = 1;
        else if (ins == '<')
            dx = -1;
        else if (ins == '>')
            dx = 1;


        int currx = pos.first + dx, curry = pos.second + dy;
        int robx = currx, roby = curry;

        


        queue<pair<int, int>> tomove;
        tomove.push({ currx,curry });
        set<pair<char, pair<int, int>>> confirmedMove;

        bool possibleMove = true;

        while (!tomove.empty())
        {
            auto p = tomove.front();
            tomove.pop();
            currx = p.first;
            curry = p.second;
            char c = matrix[p.second][p.first];
            if (c == '#')
            {
                possibleMove = false;
                break;
            }
            else if (c == ']')
            {
                confirmedMove.insert({ c,{ currx,curry } });
                confirmedMove.insert({ '[',{currx - 1,curry}});
                if (dx != -1)
                    tomove.push({ currx+dx,curry +dy});
                tomove.push({ currx-1+dx,curry +dy});
            }
            else if (c == '[')
            {
                confirmedMove.insert({ c,{ currx,curry } });
                confirmedMove.insert({ ']',{currx + 1,curry}});
                if (dx != 1)
                    tomove.push({ currx + dx,curry + dy });
                tomove.push({ currx + 1 + dx,curry + dy });
            }
        }

        if (possibleMove)
        {
            for (auto& i : confirmedMove)
            {
                matrix[i.second.second][i.second.first] = '.';
            }
            for (auto& i : confirmedMove)
            {
                matrix[i.second.second + dy][i.second.first + dx] = i.first;
            }
            pos = { robx, roby };
        }



        if constexpr (display)
        {
            matrix[pos.second][pos.first] = '@';

            for (auto& i : matrix)
            {
                cout << i << endl;
            }
            matrix[pos.second][pos.first] = '.';
            char c;
            cin >> c;
        }
    }


    matrix[pos.second][pos.first] = '@';
    int acc = 0;
    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size(); x++)
        {
            if (matrix[y][x] == '[')
                acc += x + 100 * y;
        }
    }
    cout << acc << endl;

    for (auto& i : matrix)
    {
        cout << i << endl;
    }
}

void day15()
{

    vector<string> matrix;
    string instructions = "";
    {
        ifstream f{ "Day15.txt" };

        string line;


        while (std::getline(f, line)) {
            if (line.empty())
                break;

            matrix.emplace_back(line.begin(), line.end());
        }


        while (std::getline(f, line)) {
            instructions = instructions + line;
        }
    }

    pair<int, int> pos;

    int found = false;
    for (size_t y = 1; y < matrix.size(); y++)
    {
        if (matrix[y].find("@") != string::npos)
        {
            pos = { matrix[y].find("@"), y};
            found = true;
        }
    }

    matrix[pos.second][pos.first] = '.';

    for (auto ins : instructions)
    {
        int dx = 0;
        int dy = 0;
        if (ins == '^')
            dy = -1;
        else if (ins == 'v')
            dy = 1;
        else if (ins == '<')
            dx = -1;
        else if (ins == '>')
            dx = 1;

        int currx = pos.first+ dx, curry = pos.second + dy;
        int robx = currx, roby = curry;

        while (matrix[curry][currx] == 'O')
        {
            currx += dx;
            curry += dy;
        }

        if (matrix[curry][currx] == '.')
        {
            matrix[curry][currx] = 'O';
            matrix[roby][robx] = '.';
            pos.first = robx;
            pos.second = roby;
        }
    }

    matrix[pos.second][pos.first] = '@';
    int acc = 0;
    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size(); x++)
        {
            if (matrix[y][x] == 'O')
                acc += x + 100 * y;
        }
    }
    cout << acc << endl;

    for (auto& i : matrix)
    {
        cout << i << endl;
    }
}

void day14part2()
{
    struct Robot {
        int px, py, vx, vy;
    };

    uint64_t acc = 0;
    {
        ifstream f{ "Day14.txt" };
        vector<Robot> robots;

        string line;

        constexpr int width = 101, height = 103;
        constexpr int nIts = 100;

        while (std::getline(f, line)) {

            pair<int, int> p;
            pair<int, int> v;

            auto* tofill = &p;
            stringstream ss{ line };
            for (size_t i = 0; i < 2; i++)
            {
                string s;
                int x = 0, y = 0;
                getline(ss, s, '=');
                getline(ss, s, ',');
                tofill->first = stoi(s);

                getline(ss, s, ' ');                
                tofill->second= stoi(s);

                tofill = &v;

            }

            robots.push_back({ p.first, p.second, v.first, v.second });
        }
        set<pair<int, int>> poses;
        for (size_t sec = 0; true; sec++)
        {
            vector<vector<int>> data(103, vector<int>(101, 0));
            for (auto& r : robots) {
                r.px += r.vx;
                r.px %= width;
                if (r.px < 0)
                    r.px += width;
                r.py += r.vy;
                r.py %= height;
                if (r.py < 0)
                    r.py += height;

                poses.insert({ r.px, r.py });

                data[r.py][r.px]++;
            }

            bool allneighbours = true;

            int hasNeighbour = 0;
            
            for (auto& i : poses)
            {
                bool found = false;
                for (int dx = -1; dx < 2; dx++)
                {
                    for (int dy = -1; dy < 2; dy++)
                    {
                        if (dy == 0 && dx == 0)
                            continue;
                        if (poses.count({ i.first + dx, i.second + dy }))
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        break;
                }
                if (found)
                    hasNeighbour++;
            }

            if (hasNeighbour/ float(robots.size()) > 0.7f)
            {
                cout << "Lots have neighbours at : " << sec << endl;
                for (int j = 0; j < data.size(); j++)
                {
                    for (int x : data[j]) {
                        cout << (x == 0 ? ' ' : 'x');
                    }
                    cout << "\n";
                }
                cout << endl;
                char c;
                cin >> c;
            }

            if (sec % 10000 == 0)
                cout << sec << endl;

            poses.clear();
        }
    }
}

void day14()
{
    vector<vector<int>> place(2, vector<int>{ 0, 0 });
    uint64_t acc = 0;
    {
        ifstream f{ "Day14.txt" };

        string line;

        constexpr int width = 101, height = 103;
        constexpr int nIts = 100;

        while (std::getline(f, line)) {

            pair<int64_t, int64_t> p;
            pair<int64_t, int64_t> v;

            auto* tofill = &p;
            stringstream ss{ line };
            for (size_t i = 0; i < 2; i++)
            {
                string s;
                int x = 0, y = 0;
                getline(ss, s, '=');
                getline(ss, s, ',');
                tofill->first = stoi(s);

                getline(ss, s, ' ');                
                tofill->second= stoi(s);

                tofill = &v;

            }

            int yfinal = (p.second + v.second * nIts);
            int xfinal = (p.first + v.first * nIts);
            yfinal %= height;
            xfinal %= width;
            if (yfinal < 0)
                yfinal += height;
            if (xfinal < 0)
                xfinal += width;

            if (yfinal == height / 2  || xfinal == width / 2)
                continue;

            place[yfinal > height / 2][xfinal > width / 2]++;

            cout << xfinal << ", " << yfinal << "\n";


        }
    }
    std::cout << place[0][0]*place[1][0]* place[0][1] * place[1][1] << endl;
}

void day13()
{
    uint64_t acc = 0;
    {
        ifstream f{ "Day13.txt" };

        string line;

        while (true) {

            vector<vector<int64_t>> m(2, vector<int64_t>{});
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
