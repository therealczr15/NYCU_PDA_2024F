# ifndef LEGALIZER_CPP
# define LEGALIZER_CPP

# include "legalizer.h"

bool cmp(Cell a, Cell b)
{
    return a.point.x < b.point.x;
}

void Legalizer::Read(string logPath)
{
    fstream logFile;
    logFile.open(logPath);

    string str;
    logFile >> str >> alpha >> str >> beta >> str >> diePointLeft.x >> diePointLeft.y >> diePointRight.x >> diePointRight.y;
    dieSize.width  = diePointRight.x - diePointLeft.x;
    dieSize.height = diePointRight.y - diePointLeft.y;

    while(logFile >> str)
    {
        if(str != "PlacementRows")
        {
            Cell cell;
            cell.name = str;
            logFile >> cell.point.x >> cell.point.y >> cell.size.width >> cell.size.height >> str;
            if(str == "NOTFIX")
                cell.fixed = 0;
            else if(str == "FIX")
                cell.fixed = 1;
            cellMap[cell.name] = cell;
        }
        else
        {
            PlacementRow pr;
            logFile >> pr.point.x >> pr.point.y >> pr.siteSize.width >> pr.siteSize.height >> pr.siteCount;
            prSize = pr.siteSize;
            cellInPR.push_back(pr);
            if(cellInPR.size() == 1)
                prPoint = pr.point;
        }
    }

    for(auto i = cellMap.begin(); i != cellMap.end(); i++)
    {
        int startRow = (i -> second.point.y - prPoint.y) / prSize.height;
        double curY = i -> second.point.y;
        while(curY < i -> second.point.y + i -> second.size.height)
        {
            if((i -> second.point.x >= cellInPR[startRow].point.x) && (i -> second.point.x + i -> second.size.width <= cellInPR[startRow].point.x + cellInPR[startRow].siteSize.width * cellInPR[startRow].siteCount))
                cellInPR[startRow].cellList.push_back(i -> second);

            curY += prSize.height;
            startRow++;
        }
    }

    for(unsigned int i = 0; i < cellInPR.size(); i++)
    {
        sort(cellInPR[i].cellList.begin(), cellInPR[i].cellList.end(), cmp);
        cellInPR[i].istIdx = 0;
    }
}

void Legalizer::Run(string optPath, string outPath)
{
    fstream optFile, outFile;
    optFile.open(optPath);
    outFile.open(outPath, ios::out);

    string str;
    Cell cell;
    while(optFile >> str)
    {
        while(optFile >> str)
        {
            if(str != "-->")
                Remove(str);
            else
                break;
        }

        optFile >> cell.name >> cell.point.x >> cell.point.y >> cell.size.width >> cell.size.height;
        cell.fixed = 2;        
        Insert(cell, outFile);
        
    }
}

void Legalizer::Remove(string cellName)
{
    Cell rmCell = cellMap[cellName];
    int startRow = (rmCell.point.y - prPoint.y) / prSize.height;
    double curY = rmCell.point.y;
    while(curY < rmCell.point.y + rmCell.size.height)
    {
        for(auto i = cellInPR[startRow].cellList.begin(); i != cellInPR[startRow].cellList.end(); i++)
        {
            if(cellName == i -> name)
            {
                cellInPR[startRow].cellList.erase(i);
                break;
            }
        }

        curY += prSize.height;
        startRow++;
    }

    cellMap.erase(cellName);
}

void Legalizer::Insert(Cell c, fstream & outFile)
{
    int rowHeight = ceil(double (c.size.height * 1.0 / prSize.height * 1.0));
    int startRow = (c.point.y - prPoint.y) / prSize.height;
    bool overlap = false;
    
    for(int r = 0; r < rowHeight; r++)
    {
        if(startRow + r >= int (cellInPR.size()))
        {
            overlap = true;
            break;
        }
        for(auto i = cellInPR[startRow + r].cellList.begin(); i != cellInPR[startRow + r].cellList.end(); i++)
        {
            if(!((i -> point.x >= c.point.x + c.size.width) || (i -> point.x + i -> size.width <= c.point.x)))
            {
                overlap = true;
                break;
            }
        }       
        if(overlap)
            break;

    }
    if(!overlap)
    {
        cellMap[c.name] = c;
        startRow = (c.point.y - prPoint.y) / prSize.height;
        for(int r = 0; r < rowHeight; r++)
        {
            cellInPR[startRow + r].cellList.push_back(c);
            sort(cellInPR[startRow + r].cellList.begin(), cellInPR[startRow + r].cellList.end(), cmp);
        }
        outFile << int (c.point.x) << " " << int (c.point.y) << '\n' << "0\n";
        //cout << "Insert success "  << c.name << " " << int (c.point.x) << " " << int (c.point.y) << '\n';
        return;
    }
    else
    {       
        startRow = 0;
        bool find = false;
        double bestCost = DBL_MAX;
        Cell candidate = c;
        for(unsigned int i = startRow; i < cellInPR.size() - rowHeight; i++)
        {
            for(int j = 0; j < rowHeight; j++)
                cellInPR[i + j].istIdx = 0;
            while(1)
            {  
                double left = 0;
                double right = diePointRight.x;
                double cellLeft, cellRight;

                for(int j = 0; j < rowHeight; j++)
                {                    
                    if(cellInPR[i + j].istIdx == 0)
                    {
                        cellLeft = prPoint.x;
                        if(cellInPR[i + j].cellList.size() == 0)
                            cellRight = cellInPR[i + j].point.x + cellInPR[i + j].siteCount * cellInPR[i + j].siteSize.width;
                        else
                            cellRight = cellInPR[i + j].cellList[cellInPR[i + j].istIdx].point.x;                        
                    }                   
                    else if(cellInPR[i + j].istIdx == int (cellInPR[i + j].cellList.size()))
                    {
                        if(cellInPR[i + j].cellList.size() == 0)
                        {
                            cellLeft = prPoint.x;
                            cellRight = cellInPR[i + j].point.x + cellInPR[i + j].siteCount * cellInPR[i + j].siteSize.width;
                        }
                        else
                        {
                            cellLeft = cellInPR[i + j].cellList[cellInPR[i + j].istIdx - 1].point.x + cellInPR[i + j].cellList[cellInPR[i + j].istIdx - 1].size.width;
                            cellRight = cellInPR[i + j].point.x + cellInPR[i + j].siteCount * cellInPR[i + j].siteSize.width;
                        }
                    }
                    else
                    {
                        if(cellInPR[i + j].cellList.size() == 0)
                        {
                            cellLeft = prPoint.x;
                            cellRight = cellInPR[i + j].point.x + cellInPR[i + j].siteCount * cellInPR[i + j].siteSize.width;
                        }
                        else
                        {
                            cellLeft = cellInPR[i + j].cellList[cellInPR[i + j].istIdx - 1].point.x + cellInPR[i + j].cellList[cellInPR[i + j].istIdx - 1].size.width;
                            cellRight = cellInPR[i + j].cellList[cellInPR[i + j].istIdx].point.x;
                        }
                    }
                    if(cellLeft > left)
                        left  = cellLeft;
                    if(cellRight < right)
                        right = cellRight;
                    
                }

                while(right - left >= c.size.width)
                {                    
                    find = true;
                    if(bestCost > CalCost(left, cellInPR[i].point.y, c.point.x, c.point.y))
                    {
                        candidate.point.x = left;
                        candidate.point.y = cellInPR[i].point.y;
                        startRow = i;
                        bestCost = CalCost(left, cellInPR[i].point.y, c.point.x, c.point.y);
                    }
                    left += c.size.width;
                }

                int finishCount = 0;
                for(int j = 0; j < rowHeight; j++)
                    if(cellInPR[i + j].istIdx == int (cellInPR[i + j].cellList.size()))
                        finishCount++;

                if(finishCount == rowHeight)
                    break;

                for(int j = 0; j < rowHeight; j++)
                {
                    if(cellInPR[i + j].istIdx != int (cellInPR[i + j].cellList.size()))
                    {
                        if(cellInPR[i + j].cellList[cellInPR[i + j].istIdx].point.x <= right)
                        {
                            cellInPR[i + j].istIdx++;
                        }
                    }
                }
            }
        }

        if(find)
        {
            cellMap[c.name] = candidate;
            for(int r = 0; r < rowHeight; r++)
            {
                cellInPR[startRow + r].cellList.push_back(candidate);
                sort(cellInPR[startRow + r].cellList.begin(), cellInPR[startRow + r].cellList.end(), cmp);
            }
            outFile << int (candidate.point.x) << " " << int (candidate.point.y) << '\n' << "0\n";
            return;
        }
        else
            return;

    }
}

double Legalizer::CalCost(double x1, double y1, double x2, double y2)
{
    double cost = fabs(x1 - x2) + fabs(y1 - y2);
    return cost;
}

void Legalizer::Debugger()
{
    string cellName= "FF_1_11224";
    for(auto i = cellInPR[417].cellList.begin(); i != cellInPR[417].cellList.end(); i++)
        if(i -> name == cellName)
            cout << i -> point.x << " " << i -> point.y << " " << i -> size.width << " " << i -> size.height << endl;
    
}

void Legalizer::Dump(string lay)
{
    fstream layout;
    layout.open(lay, ios::out);
    layout << int (diePointRight.x) << " " << int (diePointRight.y) << endl;
    for(auto i = cellMap.begin(); i != cellMap.end(); i++)
    {
        if(i -> second.fixed == 0)
            layout << i -> first << " " << int (i -> second.point.x) << " " << int (i -> second.point.y) << " " << int (i -> second.size.width) << " " << int (i -> second.size.height) << " " << "NOTFIX" << endl;
        else if(i -> second.fixed == 1)
            layout << i -> first << " " << int (i -> second.point.x) << " " << int (i -> second.point.y) << " " << int (i -> second.size.width) << " " << int (i -> second.size.height) << " " << "FIX" << endl;
        else
            layout << i -> first << " " << int (i -> second.point.x) << " " << int (i -> second.point.y) << " " << int (i -> second.size.width) << " " << int (i -> second.size.height) << " " << "BLANKING" << endl;
    }
}

# endif