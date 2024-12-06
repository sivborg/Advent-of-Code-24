#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

pair<bool, int> line_ok(vector<int> line)
{
    if (line.size() < 2)
    {
        return {true, -1};
    }

    int oldnum = line.front();
    int olddiff = 0;
    
    for (size_t i = 1; i < line.size(); i++)
    {
        int newnum = line[i];
    
        if (abs(newnum - oldnum) < 1 || abs(newnum - oldnum) > 3 || ( (olddiff != 0 ) && (((newnum - oldnum) < 0) != (olddiff < 0))))
        {
            return {false, i-1};
        }
        olddiff = newnum - oldnum;
        oldnum = newnum;
    }
    return {true, -1};
}

int main()
{
    vector<int64_t> leftvec,rightvec;

    {
        ifstream f{"input.txt"};

        string line;
        int numsafe = 0;
        1+1;
        while (std::getline(f, line))
        {
            stringstream ss{line};
            vector<int> numbers((istream_iterator<int>(ss)), 
                    istream_iterator<int>());
            
            auto res = line_ok(numbers);
            if (!res.first)
            {
                if (res.second == 1) // Edge case when removing the first may be necessary
                {
                    if (line_ok({numbers.begin() + 1, numbers.end()}).first) // Iterator constructor
                    {
                        numsafe++;
                        continue;
                    }
                }
                int ele = numbers[res.second];
                numbers.erase(numbers.begin()+res.second);
                if (!line_ok(numbers).first)
                {
                    numbers[res.second] = ele;
                    if (!line_ok(numbers).first)
                        continue;                
                }
                
            }
            numsafe++;
        }

        std::cout << "Num safe: " << numsafe << "\n";
        
    }
}