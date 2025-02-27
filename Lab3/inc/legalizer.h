# ifndef LEGALIZER_H
# define LEGALIZER_H

# include <bits/stdc++.h>

using namespace std;

struct Point
{
    double x;
    double y;
};

struct Size
{
    double width;
    double height;
};

struct Cell
{
    string name;
    Point point;
    Size size;
    int fixed;
};

struct PlacementRow
{
    Point point;
    Size siteSize;
    int siteCount;
    vector<Cell> cellList;
    int istIdx;
};

class Legalizer
{
    public:
        double alpha;
        double beta;
        Point diePointLeft;
        Point diePointRight;
        Size dieSize;

        Point prPoint;
        Size prSize;

        map<string, Cell> cellMap;
        vector<PlacementRow> cellInPR;

        Legalizer() {};
        void Read(string logPath);
        void Run(string optPath, string outPath);
        void Remove(string cellName);
        void Insert(Cell c, fstream & outFile);
        bool Overlap(double x, double y, double width, double height);
        double CalCost(double x1, double y1, double x2, double y2);
        void Debugger();
        void Dump(string lay);

};

# endif