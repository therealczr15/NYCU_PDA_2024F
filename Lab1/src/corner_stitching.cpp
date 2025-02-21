# ifndef CORNER_STITCHING_CPP
# define CORNER_STITCHING_CPP

# include "corner_stitching.h"

bool cmp(Tile* t1, Tile* t2)
{
    return t1 -> blkIdx < t2 -> blkIdx;
}

void setTile(Tile* tile, Point lb, Point rt, int blkIdx)
{
    tile -> lb = lb;
    tile -> rt = rt;
    tile -> blkIdx = blkIdx;
}

CornerStitching::CornerStitching(int w, int h)
{
    width = w;
    height = h;
    totalTile = 1;
    Point lb = {0, 0};
    Point rt;
    rt.x = lb.x + w;
    rt.y = lb.y + h;
    startTile = new Tile();
    setTile(startTile, lb, rt, -1);
}

Tile* CornerStitching::PointFinding(Point p)
{
    Tile* find = new Tile();
    find = startTile;

    while(!(find -> lb.x <= p.x && find -> lb.y <= p.y && find -> rt.x > p.x && find -> rt.y  > p.y))
    {
        while(find -> lb.y > p.y)
            find = find -> bottom;
        while(find -> rt.y <= p.y)
            find = find -> top;
        while(find -> lb.x > p.x)
            find = find -> left;
        while(find -> rt.x <= p.x)
            find = find -> right;
    }
    return find;
}

void CornerStitching::TileInsertion(Tile* newTile)
{
    Point p = {newTile -> lb.x, newTile -> rt.y - 1};
    Tile* topTile = PointFinding(p);
    Tile* topBottomTile = SpiltTop(newTile, topTile);

    p.x = newTile -> lb.x, p.y = newTile -> lb.y;
    Tile* bottomTile = PointFinding(p);
    
    Tile* bottomBottomTile = SpiltBottom(newTile, bottomTile);
    Tile* leftTile;
    Tile* rightTile;

    int i = 0;
    while(1)
    {

        // Split
        leftTile = SpiltLeft(newTile, topBottomTile);
        rightTile = SpiltRight(newTile, topBottomTile);
       
        // Merge
        if(leftTile -> top && leftTile -> lb.x == leftTile -> top -> lb.x && leftTile -> rt.x == leftTile -> top -> rt.x && leftTile -> top -> blkIdx < 0)
            VerticalTileMerging(leftTile, leftTile -> top);
        if(rightTile -> top && rightTile -> lb.x == rightTile -> top -> lb.x && rightTile -> rt.x == rightTile -> top -> rt.x && rightTile -> top -> blkIdx < 0)
            VerticalTileMerging(rightTile, rightTile -> top); 
        if(i != 0 && topBottomTile -> top && topBottomTile -> lb.x == topBottomTile -> top -> lb.x && topBottomTile -> rt.x == topBottomTile -> top -> rt.x && topBottomTile -> top -> blkIdx < 0)
            VerticalTileMerging(topBottomTile, topBottomTile -> top);
        i++;

        if(topBottomTile -> bottom && topBottomTile -> bottom != bottomBottomTile)
            topBottomTile = topBottomTile -> bottom;
        else
            break;
    }

    leftTile = leftTile -> bottom;
    rightTile = rightTile -> bottom;

    if(leftTile && leftTile -> lb.x == leftTile -> top -> lb.x && leftTile -> rt.x == leftTile -> top -> rt.x && leftTile -> top -> blkIdx < 0 && leftTile -> blkIdx < 0)
            VerticalTileMerging(leftTile, leftTile -> top);
    if(rightTile && rightTile -> lb.x == rightTile -> top -> lb.x && rightTile -> rt.x == rightTile -> top -> rt.x && rightTile -> top -> blkIdx < 0 && rightTile -> blkIdx < 0)
            VerticalTileMerging(rightTile, rightTile -> top); 
    
    topBottomTile -> blkIdx = newTile -> blkIdx;
    tileList.push_back(topBottomTile);
    startTile = tileList.front();
}

Tile* CornerStitching::SpiltTop(Tile* newTile, Tile* topTile)
{
    if(newTile -> rt.y == topTile -> rt.y)
        return topTile;
    else
    {
        Tile* tmpTile = new Tile();
        *tmpTile = *topTile;

        topTile -> lb.y = newTile -> rt.y;
        topTile -> bottom = tmpTile;
        while(topTile -> left && topTile -> left -> rt.y <= topTile -> lb.y)
            topTile -> left = topTile -> left -> top;
        
        tmpTile -> rt.y = newTile -> rt.y;
        tmpTile -> top = topTile;
        while(tmpTile -> right && tmpTile -> right -> lb.y >= tmpTile -> rt.y)
            tmpTile -> right = tmpTile -> right -> bottom;
        
        UpdateBottomNeighbor(tmpTile);
        UpdateLeftNeighbor(tmpTile);
        UpdateRightNeighbor(tmpTile);

        totalTile++;

        return tmpTile;
    }
}

Tile* CornerStitching::SpiltBottom(Tile* newTile, Tile* bottomTile)
{
    if(newTile -> lb.y == bottomTile -> lb.y)
    {
        Tile* tmpTile = bottomTile -> bottom;
        while(tmpTile && tmpTile -> rt.x <= newTile -> lb.x)
            tmpTile = tmpTile -> right;
        return tmpTile;
    }
    else
    {
        Tile* tmpTile = new Tile();
        *tmpTile = *bottomTile;

        tmpTile -> rt.y = newTile -> lb.y;
        tmpTile -> top = bottomTile;
        while(tmpTile -> right && tmpTile -> right -> lb.y >= tmpTile -> rt.y)
            tmpTile -> right = tmpTile -> right -> bottom;

        bottomTile -> lb.y = newTile -> lb.y;
        bottomTile -> bottom = tmpTile;
        while(bottomTile -> left && bottomTile -> left -> rt.y <= bottomTile -> lb.y)
            bottomTile -> left = bottomTile -> left -> top;
        
        UpdateBottomNeighbor(tmpTile);
        UpdateLeftNeighbor(tmpTile);
        UpdateRightNeighbor(tmpTile);

        totalTile++;

        return tmpTile;
    }
}

Tile* CornerStitching::SpiltLeft(Tile* newTile, Tile* topBottomTile)
{
    if(newTile -> lb.x == topBottomTile -> lb.x)
    {
        Tile* tmpTile = new Tile();
        tmpTile -> lb.x = 0, tmpTile -> lb.y = 0;
        tmpTile -> rt.x = 0, tmpTile -> rt.y = 0;
        return tmpTile;
    }
    else
    {
        Tile* tmpTile = new Tile();
        *tmpTile = *topBottomTile;

        topBottomTile -> lb.x = newTile -> lb.x;
        topBottomTile -> left = tmpTile;
        while(topBottomTile -> bottom && topBottomTile -> bottom -> rt.x <= topBottomTile -> lb.x)
            topBottomTile -> bottom = topBottomTile -> bottom -> right;

        tmpTile -> rt.x = newTile -> lb.x;
        tmpTile -> right = topBottomTile;
        while(tmpTile -> top && tmpTile -> top -> lb.x >= tmpTile -> rt.x)
            tmpTile -> top = tmpTile -> top -> left;
        
        UpdateTopNeighbor(tmpTile);
        UpdateBottomNeighbor(tmpTile);
        UpdateLeftNeighbor(tmpTile);
        
        totalTile++;

        return tmpTile;
    }
}

Tile* CornerStitching::SpiltRight(Tile* newTile, Tile* topBottomTile)
{
    if(newTile -> rt.x == topBottomTile -> rt.x)
    {
        Tile* tmpTile = new Tile();
        tmpTile -> lb.x = 0, tmpTile -> lb.y = 0;
        tmpTile -> rt.x = 0, tmpTile -> rt.y = 0;
        return tmpTile;
    }
    else
    {
        Tile* tmpTile = new Tile();
        *tmpTile = *topBottomTile;

        topBottomTile -> rt.x = newTile -> rt.x;
        topBottomTile -> right = tmpTile;
        while(topBottomTile -> top && topBottomTile -> top -> lb.x >= topBottomTile -> rt.x)
            topBottomTile -> top = topBottomTile -> top -> left;

        tmpTile -> lb.x = newTile -> rt.x;
        tmpTile -> left = topBottomTile;
        while(tmpTile -> bottom && tmpTile -> bottom -> rt.x <= tmpTile -> lb.x)
            tmpTile -> bottom = tmpTile -> bottom -> right;
        
        UpdateTopNeighbor(tmpTile);
        UpdateBottomNeighbor(tmpTile);
        UpdateRightNeighbor(tmpTile);
        
        totalTile++;

        return tmpTile;
    }
}

void CornerStitching::UpdateTopNeighbor(Tile* t)
{
    Tile* topTile = t -> top;
    while(topTile && topTile -> lb.x >= t -> lb.x)
    {
        topTile -> bottom = t;
        topTile = topTile -> left;
    }
}

void CornerStitching::UpdateBottomNeighbor(Tile* t)
{
    Tile* bottomTile = t -> bottom;
    while(bottomTile && bottomTile -> rt.x <= t -> rt.x)
    {
        bottomTile -> top = t;
        bottomTile = bottomTile -> right;
    }
}

void CornerStitching::UpdateLeftNeighbor(Tile* t)
{
    Tile* leftTile = t -> left;
    while(leftTile && leftTile -> rt.y <= t -> rt.y)
    {
        leftTile -> right = t;
        leftTile = leftTile -> top;
    }
}

void CornerStitching::UpdateRightNeighbor(Tile* t)
{
    Tile* rightTile = t -> right;
    while(rightTile && rightTile -> lb.y >= t -> lb.y)
    {
        rightTile -> left = t;
        rightTile = rightTile -> bottom;
    }
}

void CornerStitching::VerticalTileMerging(Tile* t1, Tile* t2)
{
    t1 -> rt.y = t2 -> rt.y;
    t1 -> top = t2 -> top;
    t1 -> right = t2 -> right;

    UpdateTopNeighbor(t1);
    UpdateLeftNeighbor(t1);
    UpdateRightNeighbor(t1);

    totalTile--;

    delete t2;

}

void CornerStitching::NeighborFinding(Tile* t, int* solidTileNum, int* spaceTileNum)
{
    *solidTileNum = 0;
    *spaceTileNum = 0;
    NeighborFindingR(t, solidTileNum, spaceTileNum);
    NeighborFindingL(t, solidTileNum, spaceTileNum);
    NeighborFindingT(t, solidTileNum, spaceTileNum);
    NeighborFindingB(t, solidTileNum, spaceTileNum);
}

void CornerStitching::NeighborFindingR(Tile* t, int* solidTileNum, int* spaceTileNum)
{
    Tile* find = t -> right;
    while(find && find -> rt.y > t -> lb.y)
    {
        if(find -> blkIdx > 0)
            (*solidTileNum)++;
        else
            (*spaceTileNum)++;
        find = find -> bottom;
    }
}

void CornerStitching::NeighborFindingL(Tile* t, int* solidTileNum, int* spaceTileNum)
{
    Tile* find = t -> left;
    while(find && find -> lb.y < t -> rt.y)
    {
        if(find -> blkIdx > 0)
            (*solidTileNum)++;
        else
            (*spaceTileNum)++;
        find = find -> top;
    }
}

void CornerStitching::NeighborFindingT(Tile* t, int* solidTileNum, int* spaceTileNum)
{
    Tile* find = t -> top;
    while(find && find -> rt.x > t -> lb.x)
    {
        if(find -> blkIdx > 0)
            (*solidTileNum)++;
        else
            (*spaceTileNum)++;
        find = find -> left;
    }
}

void CornerStitching::NeighborFindingB(Tile* t, int* solidTileNum, int* spaceTileNum)
{
    Tile* find = t -> bottom;
    while(find && find -> lb.x < t -> rt.x)
    {
        if(find -> blkIdx > 0)
            (*solidTileNum)++;
        else
            (*spaceTileNum)++;
        find = find -> right;
    }
}

void CornerStitching::PrintTile()
{
    for(unsigned int i = 0; i < tileList.size(); i++)
    {
        cout << "===== " << i << " =====\n";
        cout << "blkIdx: " << tileList[i] -> blkIdx << endl;
        cout << "lb.x: " << tileList[i] -> lb.x << endl;;
        cout << "lb.y: " << tileList[i] -> lb.y << endl;;
        cout << "rt.x: " << tileList[i] -> rt.x << endl;;
        cout << "rt.y: " << tileList[i] -> rt.y << endl;;

        if(tileList[i] -> left)
            cout << "left: " << tileList[i] -> left -> lb.x << " " << tileList[i] -> left -> lb.y << " " << tileList[i] -> left -> rt.x << " " << tileList[i] -> left -> rt.y << " "  << endl;
        if(tileList[i] -> right)
            cout << "right: " << tileList[i] -> right -> lb.x << " " << tileList[i] -> right -> lb.y << " " << tileList[i] -> right -> rt.x << " " << tileList[i] -> right -> rt.y << " "  << endl;
        if(tileList[i] -> top)
            cout << "top: " << tileList[i] -> top -> lb.x << " " << tileList[i] -> top -> lb.y << " " << tileList[i] -> top -> rt.x << " " << tileList[i] -> top -> rt.y << " "  << endl;
        if(tileList[i] -> bottom)
            cout << "bottom: " << tileList[i] -> bottom -> lb.x << " " << tileList[i] -> bottom -> lb.y << " " << tileList[i] -> bottom -> rt.x << " " << tileList[i] -> bottom -> rt.y << " "  << endl;
        cout << "=====   =====\n";
    }
}

void CornerStitching::ShowAllTile(fstream& out)
{
    for(unsigned int i = 0; i < tileList.size(); i++)
    {
        int* solidTileNum = new int;
        int* spaceTileNum = new int;
        (*solidTileNum) = 0, (*spaceTileNum) = 0;
        NeighborFinding(tileList[i], solidTileNum, spaceTileNum);
        out << tileList[i] -> blkIdx << " " << *solidTileNum << " " << *spaceTileNum << endl;
    }
}

void CornerStitching::EnumerateAllTile(Tile* t, fstream& lay)
{
    lay << t -> blkIdx << " " << t -> lb.x << " " << t -> lb.y << " " << t -> rt.x - t -> lb.x << " " << t -> rt.y - t -> lb.y << endl;
    visited.insert(t);
    if(t -> right && t -> right -> lb.y >= t -> lb.y && !visited.count(t -> right))
        EnumerateAllTile(t -> right, lay);
    if(t -> bottom && !visited.count(t -> bottom))
        EnumerateAllTile(t -> bottom, lay);
}

# endif