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

        string line;
        int numsafe = 0;
        char currcarr = 0;
        int acc = 0;
        bool doit = true;
        while (std::getline(f, line))
        {
            int state= 0;
            int dostate= 0;
            int num1 = 0, num2 = 0;
            string numstr = "";
            for (size_t pos = 0; pos < line.size(); pos++)
            {
                currcarr = line[pos];
                if (state==5)
                {
                    if (isdigit(currcarr))
                    {
                        numstr = numstr+currcarr;
                    }
                    else if (currcarr == ')' && numstr.size() > 0 && numstr.size() < 4)
                    {
                        num2 = stoi(numstr);
                        if (doit)
                        {
                            acc += num1*num2;
                        }
                        state=0;
                        numstr = "";
                        // cout << line.substr(std::max(0,int(pos-15)),16) << endl;
                    }
                    else
                    {
                        state=0;
                        numstr = "";
                    }
                }
                if (state==4)
                {
                    if (isdigit(currcarr))
                    {
                        numstr = numstr+currcarr;
                    }
                    else if (currcarr == ',' && numstr.size() > 0 && numstr.size() < 4)
                    {
                        num1 = stoi(numstr);
                        state++;
                        numstr = "";
                    }
                    else
                    {
                        state=0;
                        numstr = "";
                    }
                }
                if (state < 4)
                {
                    if (currcarr == "mul("[state])
                        state++;
                    else
                        state = 0;
                }

                if (doit){
                    if (dostate < 7)
                    {
                        if (currcarr == "don't()"[dostate])
                            dostate++;
                        else
                            dostate = 0;
                        
                    }
                    else{
                        doit = false;
                        dostate = 0;
                    }
                }
                else
                {
                    if (dostate < 4)
                    {
                        if (currcarr == "do()"[dostate])
                            dostate++;
                        else
                            dostate = 0;
                        
                    }
                    else{
                        doit = true;
                        dostate = 0;
                    }

                }
            }
        }
        cout << acc << endl;
    }
}