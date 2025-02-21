# ifndef ROUTER_CPP
# define ROUTER_CPP

# include "router.h"

struct cmp
{
    bool operator()(Cost a, Cost b)
    {
        return a.cost > b.cost;
    }
};

bool cmpBump(Bump b1, Bump b2)
{
    return b1.length > b2.length;
};

void Router::Read(string gmp, string gcl, string cst)
{
    fstream gmpFile, gclFile, cstFile;
    gmpFile.open(gmp);
    gclFile.open(gcl);
    cstFile.open(cst);

    string str;
    gmpFile >> str;
    gmpFile >> raInfo.point.x >> raInfo.point.y >> raInfo.size.width >> raInfo.size.height;

    gmpFile >> str;
    gmpFile >> gridSize.width >> gridSize.height;

    gmpFile >> str;
    gmpFile >> chipInfo1.point.x >> chipInfo1.point.y >> chipInfo1.size.width >> chipInfo1.size.height;
    chipInfo1.point.x += raInfo.point.x;
    chipInfo1.point.y += raInfo.point.y;

    gmpFile >> str;
    bumpNum = 0;
    while(gmpFile >> str)
    {
        if(str != ".c")
        {
            Point p;
            gmpFile >> p.x >> p.y;
            p.x += chipInfo1.point.x;
            p.y += chipInfo1.point.y;
            bump1.push_back(p);
            bumpNum++;
        }
        else
            break;
    }

    gmpFile >> chipInfo2.point.x >> chipInfo2.point.y >> chipInfo2.size.width >> chipInfo2.size.height;
    chipInfo2.point.x += raInfo.point.x;
    chipInfo2.point.y += raInfo.point.y;

    gmpFile >> str;
    while(gmpFile >> str)
    {
        Point p;
        gmpFile >> p.x >> p.y;
        p.x += chipInfo2.point.x;
        p.y += chipInfo2.point.y;
        bump2.push_back(p);
    }

    for(int i = 0; i < bumpNum; i++)
    {
        Bump b;
        b.b1 = bump1[i];
        b.b2 = bump2[i];
        b.length = abs(b.b1.x - b.b2.x) + abs(b.b1.y - b.b2.y);
        b.idx = i + 1;
        bump1to2.push_back(b);
    }

    sort(bump1to2.begin(), bump1to2.end(), cmpBump);

    //cout << "Finish GMP\n";
    
    gclFile >> str;
    GCellCol = raInfo.size.width  / gridSize.width;
    GCellRow = raInfo.size.height / gridSize.height;
    GCell.resize(GCellRow, vector<Capacity> (GCellCol));
    for(int i = 0; i < GCellRow; i++)
        for(int j = 0; j < GCellCol; j++)
            gclFile >> GCell[i][j].left >> GCell[i][j].bottom;

    //cout << "Finish GCL\n";
    
    cstFile >> str;
    cstFile >> alpha;

    cstFile >> str;
    cstFile >> beta;

    cstFile >> str;
    cstFile >> gamma;

    cstFile >> str;
    cstFile >> delta;

    cstFile >> str;
    cstFile >> viaCost;

    cstFile >> str;
    m1Cost.resize(GCellRow, vector<double> (GCellCol));
    m2Cost.resize(GCellRow, vector<double> (GCellCol));
    maxCost = 0;
    for(int i = 0; i < GCellRow; i++)
        for(int j = 0; j < GCellCol; j++)
        {
            cstFile >> m1Cost[i][j];
            if(m1Cost[i][j] > maxCost)
                maxCost = m1Cost[i][j];
        }

    cstFile >> str;
    for(int i = 0; i < GCellRow; i++)
        for(int j = 0; j < GCellCol; j++)
        {
            cstFile >> m2Cost[i][j];
            if(m2Cost[i][j] > maxCost)
                maxCost = m2Cost[i][j];
        }

    //cout << "Finish CST\n";
}

void Router::Dijkstra(string lg, string out)
{
    fstream lgFile, outFile;
    lgFile.open(lg, ios::out);
    outFile.open(out, ios::out);

    vector<vector<Capacity>> cap;
    cap.resize(GCellRow, vector<Capacity> (GCellCol));
    for(int i = 0; i < GCellRow; i++)
        for(int j = 0; j < GCellCol; j++)
        {
            cap[i][j].left = 0;
            cap[i][j].bottom = 0;
        }

    for(int i = 0; i < bumpNum; i++)
    {
        Position start, end;
        start.c = (bump1to2[i].b1.x - raInfo.point.x) / gridSize.width;
        start.r = (bump1to2[i].b1.y - raInfo.point.y) / gridSize.height;
        Point p;
        p = bump1to2[i].b2;
        end.c = (p.x - raInfo.point.x) / gridSize.width;
        end.r = (p.y - raInfo.point.y) / gridSize.height;
        
        vector<vector<double>> cost;
        cost.resize(GCellRow, vector<double> (GCellCol));
        vector<vector<bool>> chosen;
        chosen.resize(GCellRow, vector<bool> (GCellCol));
        vector<vector<Position>> prev;
        prev.resize(GCellRow, vector<Position> (GCellCol));
        
        priority_queue<Cost, vector<Cost>, cmp> pq;
        
        for(int i = 0; i < GCellRow; i++)
            for(int j = 0; j < GCellCol; j++)
            {
                cost[i][j] = INT_MAX;
                chosen[i][j] = false;
            }
        cost[start.r][start.c] = m1Cost[start.r][start.c];
        Cost c;
        c.position = start;
        c.cost = cost[start.r][start.c];
        c.metal = 1;
        pq.push(c);
        
        while(!chosen[end.r][end.c])
        {
            Cost tmpCost = pq.top();

            Cost newCost;
            pq.pop();
            if(chosen[tmpCost.position.r][tmpCost.position.c])
                continue;
            chosen[tmpCost.position.r][tmpCost.position.c] = true;

            // Top
            if(tmpCost.position.r < GCellRow - 1)
            {
                newCost.metal = 1;
                newCost.position.r = tmpCost.position.r + 1;
                newCost.position.c = tmpCost.position.c;
                double wlCost = 10 * alpha;

                double ovCost;
                if(cap[newCost.position.r][newCost.position.c].bottom >= GCell[newCost.position.r][newCost.position.c].bottom)
                    ovCost = 0.5 * maxCost * beta;
                else
                    ovCost = 0;

                double cellCost = m1Cost[newCost.position.r][newCost.position.c] * gamma;
                double vCost = 0;
                if(tmpCost.metal == 2)
                {
                    cellCost = (m1Cost[newCost.position.r][newCost.position.c] + m2Cost[newCost.position.r][newCost.position.c]) * 0.5 * gamma;
                    vCost = viaCost * delta;
                }

                newCost.cost = tmpCost.cost + wlCost + ovCost + cellCost + vCost;
                if(cost[newCost.position.r][newCost.position.c] > tmpCost.cost + wlCost + ovCost + cellCost + vCost)
                {
                    cost[newCost.position.r][newCost.position.c] = newCost.cost;
                    prev[newCost.position.r][newCost.position.c] = tmpCost.position;
                    pq.push(newCost);
                }
            }

            // down
            if(tmpCost.position.r > 0)
            {
                newCost.metal = 1;
                newCost.position.r = tmpCost.position.r - 1;
                newCost.position.c = tmpCost.position.c;
                double wlCost = 10 * alpha;
                double ovCost;
                if(cap[tmpCost.position.r][tmpCost.position.c].bottom >= GCell[tmpCost.position.r][tmpCost.position.c].bottom)
                    ovCost = 0.5 * maxCost * beta;
                else
                    ovCost = 0;
                double cellCost = m1Cost[newCost.position.r][newCost.position.c] * gamma;
                double vCost = 0;
                if(tmpCost.metal == 2)
                {
                    cellCost = (m1Cost[newCost.position.r][newCost.position.c] + m2Cost[newCost.position.r][newCost.position.c]) * 0.5 * gamma;
                    vCost = viaCost * delta;
                }

                newCost.cost = tmpCost.cost + wlCost + ovCost + cellCost + vCost;
                if(cost[newCost.position.r][newCost.position.c] > tmpCost.cost + wlCost + ovCost + cellCost + vCost)
                {
                    cost[newCost.position.r][newCost.position.c] = newCost.cost;
                    prev[newCost.position.r][newCost.position.c] = tmpCost.position;
                    pq.push(newCost);
                }
            }

            // right
            if(tmpCost.position.c < GCellCol - 1)
            {
                newCost.metal = 2;
                newCost.position.r = tmpCost.position.r;
                newCost.position.c = tmpCost.position.c + 1;
                double wlCost = 10 * alpha;
                double ovCost;
                if(cap[newCost.position.r][newCost.position.c].left >= GCell[newCost.position.r][newCost.position.c].left)
                    ovCost = 0.5 * maxCost * beta;
                else
                    ovCost = 0;
                double cellCost = m2Cost[newCost.position.r][newCost.position.c] * gamma;
                double vCost = 0;
                if(tmpCost.metal == 1)
                {
                    cellCost = (m1Cost[newCost.position.r][newCost.position.c] + m2Cost[newCost.position.r][newCost.position.c]) * 0.5 * gamma;
                    vCost = viaCost * delta;
                }

                newCost.cost = tmpCost.cost + wlCost + ovCost + cellCost + vCost;
                if(cost[newCost.position.r][newCost.position.c] > tmpCost.cost + wlCost + ovCost + cellCost + vCost)
                {
                    cost[newCost.position.r][newCost.position.c] = newCost.cost;
                    prev[newCost.position.r][newCost.position.c] = tmpCost.position;
                    pq.push(newCost);
                }
            }
            // left
            if(tmpCost.position.c > 0)
            {
                newCost.metal = 2;
                newCost.position.r = tmpCost.position.r;
                newCost.position.c = tmpCost.position.c - 1;
                double wlCost = 10 * alpha;
                double ovCost;
                if(cap[tmpCost.position.r][tmpCost.position.c].left >= GCell[tmpCost.position.r][tmpCost.position.c].left)
                    ovCost = 0.5 * maxCost * beta;
                else
                    ovCost = 0;
                double cellCost = m2Cost[newCost.position.r][newCost.position.c] * gamma;
                double vCost = 0;
                if(tmpCost.metal == 1)
                {
                    cellCost = (m1Cost[newCost.position.r][newCost.position.c] + m2Cost[newCost.position.r][newCost.position.c]) * 0.5 * gamma;
                    vCost = viaCost * delta;
                }

                newCost.cost = tmpCost.cost + wlCost + ovCost + cellCost + vCost;
                if(cost[newCost.position.r][newCost.position.c] > tmpCost.cost + wlCost + ovCost + cellCost + vCost)
                {
                    cost[newCost.position.r][newCost.position.c] = newCost.cost;
                    prev[newCost.position.r][newCost.position.c] = tmpCost.position;
                    pq.push(newCost);
                }
            }
        }

        lgFile << "n" << bump1to2[i].idx << endl;
        outFile << "n" << bump1to2[i].idx << endl;
        Position tmpPosition = end;
        Position s = end;
        stack<string> outputList;
        int dir;
        if(prev[tmpPosition.r][tmpPosition.c].r == tmpPosition.r)
        {
            dir = 2;
            
            // left
            if(prev[tmpPosition.r][tmpPosition.c].c < tmpPosition.c)
                cap[tmpPosition.r][tmpPosition.c].left++;
            else
                cap[prev[tmpPosition.r][tmpPosition.c].r][prev[tmpPosition.r][tmpPosition.c].c].left++;
            tmpPosition = prev[tmpPosition.r][tmpPosition.c];
            outputList.push("via");
        }
        else
        {
            dir = 1;
            if(prev[tmpPosition.r][tmpPosition.c].r < tmpPosition.r)
                cap[tmpPosition.r][tmpPosition.c].bottom++;
            else
                cap[prev[tmpPosition.r][tmpPosition.c].r][prev[tmpPosition.r][tmpPosition.c].c].bottom++;
            tmpPosition = prev[tmpPosition.r][tmpPosition.c];
        }
        while(tmpPosition.r != start.r || tmpPosition.c != start.c)
        {
            if(prev[tmpPosition.r][tmpPosition.c].r == tmpPosition.r)
            {
                if(prev[tmpPosition.r][tmpPosition.c].c < tmpPosition.c)
                    cap[tmpPosition.r][tmpPosition.c].left++;
                else
                    cap[prev[tmpPosition.r][tmpPosition.c].r][prev[tmpPosition.r][tmpPosition.c].c].left++;
                if(dir == 1)
                {
                    string tmpStr = "";
                    tmpStr += "M1 ";
                    tmpStr += to_string(tmpPosition.c * gridSize.width  + raInfo.point.x);
                    tmpStr += " ";
                    tmpStr += to_string(tmpPosition.r * gridSize.height + raInfo.point.y);
                    tmpStr += " ";

                    tmpStr += to_string(s.c * gridSize.width  + raInfo.point.x);
                    tmpStr += " ";
                    tmpStr += to_string(s.r * gridSize.height + raInfo.point.y);
                    tmpStr += " ";
                    outputList.push(tmpStr);
                    outputList.push("via");

                    s = tmpPosition;
                    dir = 2;
                }
                tmpPosition = prev[tmpPosition.r][tmpPosition.c];
            }
            else
            {
                if(prev[tmpPosition.r][tmpPosition.c].r < tmpPosition.r)
                    cap[tmpPosition.r][tmpPosition.c].bottom++;
                else
                    cap[prev[tmpPosition.r][tmpPosition.c].r][prev[tmpPosition.r][tmpPosition.c].c].bottom++;
                if(dir == 2)
                {
                    string tmpStr = "";
                    tmpStr += "M2 ";
                    tmpStr += to_string(tmpPosition.c * gridSize.width  + raInfo.point.x);
                    tmpStr += " ";
                    tmpStr += to_string(tmpPosition.r * gridSize.height + raInfo.point.y);
                    tmpStr += " ";

                    tmpStr += to_string(s.c * gridSize.width  + raInfo.point.x);
                    tmpStr += " ";
                    tmpStr += to_string(s.r * gridSize.height + raInfo.point.y);
                    tmpStr += " ";
                    outputList.push(tmpStr);
                    outputList.push("via");

                    s = tmpPosition;
                    dir = 1;                   
                }
                tmpPosition = prev[tmpPosition.r][tmpPosition.c];
            }
        }

        if(s.r != tmpPosition.r || s.c != tmpPosition.c)
        {
            if(s.r == tmpPosition.r)
            {
                string tmpStr = "";
                tmpStr += "M2 ";
                tmpStr += to_string(tmpPosition.c * gridSize.width  + raInfo.point.x);
                tmpStr += " ";
                tmpStr += to_string(tmpPosition.r * gridSize.height + raInfo.point.y);
                tmpStr += " ";

                tmpStr += to_string(s.c * gridSize.width  + raInfo.point.x);
                tmpStr += " ";
                tmpStr += to_string(s.r * gridSize.height + raInfo.point.y);
                tmpStr += " ";
                outputList.push(tmpStr);
                outputList.push("via");

            }
            else
            {
                string tmpStr = "";
                tmpStr += "M1 ";
                tmpStr += to_string(tmpPosition.c * gridSize.width  + raInfo.point.x);
                tmpStr += " ";
                tmpStr += to_string(tmpPosition.r * gridSize.height + raInfo.point.y);
                tmpStr += " ";

                tmpStr += to_string(s.c * gridSize.width  + raInfo.point.x);
                tmpStr += " ";
                tmpStr += to_string(s.r * gridSize.height + raInfo.point.y);
                tmpStr += " ";
                outputList.push(tmpStr);
            }
        }

        while(!outputList.empty())
        {
            string tmpStr = outputList.top();
            lgFile << tmpStr << endl;
            outFile << tmpStr << endl;
            outputList.pop();
        }

        lgFile << ".end\n";
        outFile << ".end\n";

        cost.clear();
        chosen.clear();
        pq = priority_queue<Cost, vector<Cost>, cmp>();
        prev.clear();
    }
}

void Router::Layout(string lg, string lay)
{
    fstream lgFile, layFile;
    lgFile.open(lg);
    layFile.open(lay, ios::out);

    layFile << "RoutingArea " << raInfo.point.x << " " << raInfo.point.y << " " << raInfo.point.x + raInfo.size.width << " " << raInfo.point.y + raInfo.size.height << endl;
    layFile << "Chip1 " << chipInfo1.point.x << " " << chipInfo1.point.y << " " << chipInfo1.point.x + chipInfo1.size.width << " " << chipInfo1.point.y + chipInfo1.size.height << endl;
    layFile << "Chip2 " << chipInfo2.point.x << " " << chipInfo2.point.y << " " << chipInfo2.point.x + chipInfo2.size.width << " " << chipInfo2.point.y + chipInfo2.size.height << endl;
    
    for(int i = 0; i < bumpNum; i++)
        layFile << "BumpChip1 " << bump1[i].x << " " << bump1[i].y << endl;
    for(int i = 0; i < bumpNum; i++)
        layFile << "BumpChip2 " << bump2[i].x << " " << bump2[i].y << endl;

    string str;
    int count = 0;
    while(lgFile >> str)
    {
        int x1, x2, y1, y2;
        if(str == "M1" || str == "M2")
        {
            lgFile >> x1 >> y1 >> x2 >> y2;
            if(count == 0)
                layFile << "Connection " << x1 << " " << y1 << " " << x2 << " " << y2;
            else
                layFile << " " << x2 << " " << y2;
            count++;
        }
        else if(str == ".end")
        {
            layFile << "\n";
            count = 0;
        }
    }
}

# endif