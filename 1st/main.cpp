#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int64_t> leftvec,rightvec;

    {
        ifstream f{"input.txt"};

        int64_t left, right;
        while (f >> left >> right)
        {
            leftvec.push_back(left);
            rightvec.push_back(right);
        }
        
    }
    sort(leftvec.begin(), leftvec.end());
    sort(rightvec.begin(), rightvec.end());
    int64_t acc = 0;
    for (size_t i = 0; i < leftvec.size(); i++)
    {
        acc += abs(leftvec[i] - rightvec[i]);
    }
    cout << "Puzzle 1 solution:" << int(acc) << endl;

    int previndex = 0;
    acc = 0;
    for (auto& i : leftvec)
    {
        int index = previndex;
        while (index < rightvec.size() && rightvec[index] < i)
            index+=1;

        previndex = index;
        
        while (index < rightvec.size() && rightvec[index] == i)
        {
            index+=1;
        }

        acc += i*(index-previndex);
        previndex=index;
    }
    cout << "Puzzle 2 solution: " << int(acc) << endl;
    

    return 0;
}