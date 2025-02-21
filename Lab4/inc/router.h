# ifndef ROUTER_H
# define ROUTER_H

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

struct Info
{
    Point point;
    Size size;
};

struct Capacity
{
    int left;
    int bottom;
};

struct Position
{
    int r;
    int c;
};

struct Cost
{
    Position position;
    double cost;
    int metal;
};

struct Bump
{
    Point b1;
    Point b2;
    int length;
    int idx;
};

class Router
{
    public:
        double alpha;
        double beta;
        double gamma;
        double delta;
        double viaCost;
        
        Info raInfo;
        Size gridSize;
        Info chipInfo1;
        Info chipInfo2;
        int GCellRow;
        int GCellCol;
        int bumpNum;
        vector<Point> bump1;
        vector<Point> bump2;
        vector<Bump> bump1to2;

        vector<vector<Capacity>> GCell;
        vector<vector<double>> m1Cost;
        vector<vector<double>> m2Cost;
        double maxCost;

        Router() {};
        void Read(string gmp, string gcl, string cst);
        void Dijkstra(string lg, string out);
        void Layout(string lg, string lay);
};

# endif