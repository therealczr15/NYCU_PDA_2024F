# ifndef CORNER_STITCHING_H
# define CORNER_STITCHING_H

# include <iostream>
# include <fstream>
# include <algorithm>
# include <vector>
# include <string>
# include <unordered_set>

using namespace std;

struct Point
{
    int x;
    int y;
};

struct Tile
{
    Point lb;
    Point rt;
    int blkIdx;

    Tile* left;
    Tile* bottom;
    Tile* right;
    Tile* top;
};

bool cmp(Tile* t1, Tile* t2);
void setTile(Tile* tile, Point lb, Point rt, int blkIdx);

class CornerStitching
{
    public:
        int width;
        int height;
        int totalTile;

        Tile* startTile;
        vector<Tile*> tileList;  
        unordered_set<Tile*> visited;

        CornerStitching(int w, int h);
        Tile* PointFinding(Point p);
        void NeighborFinding(Tile* t, int* solidTileNum, int* spaceTileNum);
        void NeighborFindingR(Tile* find, int* solidTileNum, int* spaceTileNum);
        void NeighborFindingL(Tile* find, int* solidTileNum, int* spaceTileNum);
        void NeighborFindingT(Tile* find, int* solidTileNum, int* spaceTileNum);
        void NeighborFindingB(Tile* find, int* solidTileNum, int* spaceTileNum);
        void TileInsertion(Tile* newTile);
        Tile* SpiltTop(Tile* newTile, Tile* topTile);
        Tile* SpiltBottom(Tile* newTile, Tile* bottomTile);
        Tile* SpiltLeft(Tile* newTile, Tile* topBottomTile);
        Tile* SpiltRight(Tile* newTile, Tile* topBottomTile);
        void UpdateTopNeighbor(Tile* t);
        void UpdateBottomNeighbor(Tile* t);
        void UpdateLeftNeighbor(Tile* t);
        void UpdateRightNeighbor(Tile* t);
        void VerticalTileMerging(Tile* t1, Tile* t2);
        void PrintTile();
        void ShowAllTile(fstream& out);
        void EnumerateAllTile(Tile* Tile, fstream& lay);
        
};  

# endif