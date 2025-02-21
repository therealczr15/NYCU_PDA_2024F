# ifndef SP_H
# define SP_H

# include <bits/stdc++.h>

using namespace std;

struct Point
{
    int x;
    int y;
};

struct Size
{
    int width;
    int height;
};

struct Block
{
    string name;
    Point point;
    Size size;
    bool blk;
};

struct Net
{
    int blkNum;
    vector<string> blkName;
};

class SP
{
    public:
        Size outlineSize;
        Size curSize;
        int blkNum;
        int tmnNum;
        int netNum;
        
        vector<Block> blkList;
        vector<Net> netList;
        map<string, int> blkMap;
        vector<int> posLoci;
        vector<int> negLoci;
        vector<int> posInvLoci;
        vector<int> negInvLoci;

        vector<int> bestPosLoci;
        vector<int> bestNegLoci;
        vector<int> bestPosInvLoci;
        vector<int> bestNegInvLoci;

        double alpha;
        int minY;
        int maxX;
        int area;
        int bestArea;
        int hpwl;
        int cost;
        int bestCost;
        int bestbestCost;
        int count;
        vector<Block> bestResult;
        vector<Size> bestSize;
        
        SP() {};
        void Read(double a, string blkPath, string netPath);
        void SimulatedAnnealing(clock_t &startTime, clock_t &endTime);
        void UdtPosition();
        void SwapPosLoci(int &overArea, int mode);
        void SwapNegLoci(int &overArea, int mode);
        void SwapBothLoci(int &overArea, int mode);
        void RttOneBlk(int &overArea, int mode);
        void RttAllBlk();
        void CalOverArea(int &oArea);
        void CalHPWL();
        void CalCost();
        void Debugger();
        void PrintBlk();
        void PrintTmn();
        void PrintNet();
        void PrintLoci();
        void PrintInfo();
        void Dump(string outPath, string layPath, clock_t &startTime, clock_t &endTime);
};

# endif