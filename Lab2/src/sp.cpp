# ifndef SP_CPP
# define SP_CPP

# define TIMEOUT    280.000000
# define ITER1      300000
# define ITER2      150000

# include "sp.h"

void SP::Read(double a, string blkPath, string netPath)
{
    fstream blkFile, netFile;
    string str;
    blkFile.open(blkPath);
    blkFile >> str >> outlineSize.width >> outlineSize.height;
    curSize.width = outlineSize.width + 1, curSize.height = outlineSize.height + 1;
    blkFile >> str >> blkNum;
    blkFile >> str >> tmnNum;

    for(int i = 0; i < blkNum; i++)
    {
        Block blk;
        blkFile >> blk.name >> blk.size.width >> blk.size.height;
        blk.blk = true; // it is a block
        blkList.push_back(blk);
        blkMap[blk.name] = i;
        posLoci.push_back(i);
        negLoci.push_back(i);
    }

    bestResult.resize(blkNum);
    bestSize.resize(blkNum);
    count = 0;

    for(int i = 0; i < tmnNum; i++)
    {
        Block tmn;
        blkFile >> tmn.name >> str >> tmn.point.x >> tmn.point.y;
        tmn.size.width = 0, tmn.size.height = 0;
        tmn.blk = false; // it is a terminal
        blkList.push_back(tmn);
        blkMap[tmn.name] = i + blkNum;
    }

    netFile.open(netPath);
    netFile >> str >> netNum;
    for(int i = 0; i < netNum; i++)
    {
        Net net;
        netFile >> str >> net.blkNum;
        for(int j = 0; j < net.blkNum; j++)
        {
            netFile >> str;
            net.blkName.push_back(str);
        }
        netList.push_back(net);
    }

    alpha = a;

    random_shuffle(posLoci.begin(), posLoci.end());
    random_shuffle(negLoci.begin(), negLoci.end());
    posInvLoci.resize(blkNum);
    negInvLoci.resize(blkNum);
    
    for(int i = 0; i < blkNum; i++)
    {
        posInvLoci[posLoci[i]] = i;
        negInvLoci[negLoci[i]] = i;
    }
}

void SP::SimulatedAnnealing(clock_t &startTime, clock_t &endTime)
{
    bestArea = INT_MAX;
    bestCost = INT_MAX;
    bestbestCost = INT_MAX;
    int overArea = INT_MAX;

    endTime = clock();

    while(double (endTime - startTime) * 1.0 / CLOCKS_PER_SEC < TIMEOUT)
    {
        
        while(outlineSize.width < curSize.width || outlineSize.height < curSize.height)
        {
            for(int i = 0; i < ITER1 * alpha + 10000; i++)
            {
                int choice = rand() % 4;
                switch(choice)
                {
                    case 0: 
                        SwapPosLoci(overArea, 0);
                        break;
                    case 1:
                        SwapNegLoci(overArea, 0);
                        break;
                    case 2:
                        SwapBothLoci(overArea, 0);
                        break;
                    case 3:
                        RttOneBlk(overArea, 0);
                        break;
                    default:
                        break;
                }
                endTime = clock();
                if(double (endTime - startTime) * 1.0 / CLOCKS_PER_SEC >= TIMEOUT)
                    break;
                //cout << "i: " << i << " " << curSize.width << " " << curSize.height << " " << bestCost << " " << bestbestCost << " " << count << '\n';
            }
            endTime = clock();
            if(double (endTime - startTime) * 1.0 / CLOCKS_PER_SEC >= TIMEOUT)
                break;
        }

        endTime = clock();
        if(double (endTime - startTime) * 1.0 / CLOCKS_PER_SEC >= TIMEOUT)
            break;

        for(int i = 0; i < ITER2; i++)
        {
            int choice = rand() % 4;
            switch(choice)
            {
                case 0: 
                    SwapPosLoci(overArea, 1);
                    break;
                case 1:
                    SwapNegLoci(overArea, 1);
                    break;
                case 2:
                    SwapBothLoci(overArea, 1);
                    break;
                case 3:
                    RttOneBlk(overArea, 1);
                    break;
                default:
                    break;
            }
            endTime = clock();
            if(double (endTime - startTime) * 1.0 / CLOCKS_PER_SEC >= TIMEOUT)
                break;
            //cout << "i: " << i << " " << curSize.width << " " << curSize.height << " " << bestCost << " " << bestbestCost << " " << count << '\n';
        }

        endTime = clock();
        if(double (endTime - startTime) * 1.0 / CLOCKS_PER_SEC >= TIMEOUT)
            break;
    }
}

void SP::UdtPosition()
{
    for(int i = 0; i < blkNum; i++)
        blkList[i].point.x = 0, blkList[i].point.y = 0;
    minY = 0, maxX = 0;
    for(int i = 0; i < blkNum - 1; i++)
    {
        for(int j = i + 1; j < blkNum; j++)
        {
            // i block is on the left of j block
            if(negInvLoci[posLoci[i]] < negInvLoci[posLoci[j]])
            {
                if(blkList[posLoci[i]].point.x + blkList[posLoci[i]].size.width > blkList[posLoci[j]].point.x)
                {
                    blkList[posLoci[j]].point.x = blkList[posLoci[i]].point.x + blkList[posLoci[i]].size.width;
                    if(blkList[posLoci[j]].point.x + blkList[posLoci[j]].size.width > maxX)
                        maxX = blkList[posLoci[j]].point.x + blkList[posLoci[j]].size.width;
                }
            }
            // j block is below i block
            else if(negInvLoci[posLoci[i]] > negInvLoci[posLoci[j]])
            {
                if(blkList[posLoci[j]].point.y > blkList[posLoci[i]].point.y - blkList[posLoci[i]].size.height)
                {
                    blkList[posLoci[j]].point.y = blkList[posLoci[i]].point.y - blkList[posLoci[i]].size.height;
                    if(blkList[posLoci[j]].point.y - blkList[posLoci[j]].size.height < minY)
                        minY = blkList[posLoci[j]].point.y - blkList[posLoci[j]].size.height;
                }
            }
        }
    }
    area = -minY * maxX;
    curSize.width = maxX, curSize.height = -minY;
}

void SP::SwapPosLoci(int &overArea, int mode)
{
    int idx1 = rand() % blkNum;
    int idx2 = rand() % blkNum;
    int tmp = posInvLoci[idx1];
    posInvLoci[idx1] = posInvLoci[idx2];
    posInvLoci[idx2] = tmp;
    posLoci[posInvLoci[idx1]] = idx1;
    posLoci[posInvLoci[idx2]] = idx2;

    int oArea;
    UdtPosition();
    CalOverArea(oArea);
    //cout << oArea << endl;
    CalHPWL();
    CalCost();

    if(mode == 0)
    {
        if(overArea != 0 && overArea < oArea)
        {
            tmp = posInvLoci[idx1];
            posInvLoci[idx1] = posInvLoci[idx2];
            posInvLoci[idx2] = tmp;
            posLoci[posInvLoci[idx1]] = idx1;
            posLoci[posInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
        }
        else if(overArea == 0 && ((bestArea < area) || oArea != 0))
        {
            tmp = posInvLoci[idx1];
            posInvLoci[idx1] = posInvLoci[idx2];
            posInvLoci[idx2] = tmp;
            posLoci[posInvLoci[idx1]] = idx1;
            posLoci[posInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
        }
        else
        {
            bestArea = area;
            bestCost = cost;
            bestbestCost = cost;

            bestPosLoci.assign(posLoci.begin(), posLoci.end());
            bestNegLoci.assign(negLoci.begin(), negLoci.end());
            bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
            bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
            for(int i = 0; i < blkNum; i++)
                bestSize[i] = blkList[i].size;
            if(overArea == 0)
                overArea = 0;
            else
                overArea = oArea;
        }
    }
    else
    {
        double exceed = (cost - bestCost) * 1.0 / bestbestCost;
        if(count < ITER2)
        {
            if(bestCost < cost || oArea != 0)
            {
                tmp = posInvLoci[idx1];
                posInvLoci[idx1] = posInvLoci[idx2];
                posInvLoci[idx2] = tmp;
                posLoci[posInvLoci[idx1]] = idx1;
                posLoci[posInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }
            else if(oArea == 0)
            {
                if(cost < bestbestCost)
                {
                    bestbestCost = cost;
                    bestPosLoci.assign(posLoci.begin(), posLoci.end());
                    bestNegLoci.assign(negLoci.begin(), negLoci.end());
                    bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
                    bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
                    for(int i = 0; i < blkNum; i++)
                        bestSize[i] = blkList[i].size;
                    count = 0;
                }
                else
                    count++;
                bestCost = cost;
            }
            else
            {
                tmp = posInvLoci[idx1];
                posInvLoci[idx1] = posInvLoci[idx2];
                posInvLoci[idx2] = tmp;
                posLoci[posInvLoci[idx1]] = idx1;
                posLoci[posInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }

        }
        else if(oArea == 0)
        {
            if(cost < bestbestCost)
            {
                bestbestCost = cost;
                bestPosLoci.assign(posLoci.begin(), posLoci.end());
                bestNegLoci.assign(negLoci.begin(), negLoci.end());
                bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
                bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
                for(int i = 0; i < blkNum; i++)
                    bestSize[i] = blkList[i].size;
            }
            else if(exceed <= 0.05)
            {
                count = 0;
                bestCost = cost;
            }
            else
            {
                tmp = posInvLoci[idx1];
                posInvLoci[idx1] = posInvLoci[idx2];
                posInvLoci[idx2] = tmp;
                posLoci[posInvLoci[idx1]] = idx1;
                posLoci[posInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }

        }
        else
        {
            tmp = posInvLoci[idx1];
            posInvLoci[idx1] = posInvLoci[idx2];
            posInvLoci[idx2] = tmp;
            posLoci[posInvLoci[idx1]] = idx1;
            posLoci[posInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
            count++;
        }
        
    }
    
}

void SP::SwapNegLoci(int &overArea, int mode)
{
    int idx1 = rand() % blkNum;
    int idx2 = rand() % blkNum;
    int tmp = negInvLoci[idx1];
    negInvLoci[idx1] = negInvLoci[idx2];
    negInvLoci[idx2] = tmp;
    negLoci[negInvLoci[idx1]] = idx1;
    negLoci[negInvLoci[idx2]] = idx2;
    
    int oArea;
    UdtPosition();
    CalOverArea(oArea);
    //cout << oArea << endl;
    CalHPWL();
    CalCost();

    if(mode == 0)
    {
        if(overArea != 0 && overArea < oArea)
        {
            tmp = negInvLoci[idx1];
            negInvLoci[idx1] = negInvLoci[idx2];
            negInvLoci[idx2] = tmp;
            negLoci[negInvLoci[idx1]] = idx1;
            negLoci[negInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
        }
        else if(overArea == 0 && ((bestArea < area) || oArea != 0))
        {
            tmp = negInvLoci[idx1];
            negInvLoci[idx1] = negInvLoci[idx2];
            negInvLoci[idx2] = tmp;
            negLoci[negInvLoci[idx1]] = idx1;
            negLoci[negInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
        }
        else
        {
            bestArea = area;
            bestCost = cost;
            bestbestCost = cost;

            bestPosLoci.assign(posLoci.begin(), posLoci.end());
            bestNegLoci.assign(negLoci.begin(), negLoci.end());
            bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
            bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
            for(int i = 0; i < blkNum; i++)
                bestSize[i] = blkList[i].size;
            if(overArea == 0)
                overArea = 0;
            else
                overArea = oArea;
        }
    }
    else
    {
        double exceed = (cost - bestCost) * 1.0 / bestbestCost;
        if(count < ITER2)
        {
            if(bestCost < cost || oArea != 0)
            {
                tmp = negInvLoci[idx1];
                negInvLoci[idx1] = negInvLoci[idx2];
                negInvLoci[idx2] = tmp;
                negLoci[negInvLoci[idx1]] = idx1;
                negLoci[negInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }
            else if(oArea == 0)
            {
                if(cost < bestbestCost)
                {
                    bestbestCost = cost;
                    bestPosLoci.assign(posLoci.begin(), posLoci.end());
                    bestNegLoci.assign(negLoci.begin(), negLoci.end());
                    bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
                    bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
                    for(int i = 0; i < blkNum; i++)
                        bestSize[i] = blkList[i].size;
                    count = 0;
                }
                else
                    count++;
                bestCost = cost;
            }
            else
            {
                tmp = negInvLoci[idx1];
                negInvLoci[idx1] = negInvLoci[idx2];
                negInvLoci[idx2] = tmp;
                negLoci[negInvLoci[idx1]] = idx1;
                negLoci[negInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }
        }
        else if(oArea == 0)
        {
            if(cost < bestbestCost)
            {
                bestbestCost = cost;
                bestPosLoci.assign(posLoci.begin(), posLoci.end());
                bestNegLoci.assign(negLoci.begin(), negLoci.end());
                bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
                bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
                for(int i = 0; i < blkNum; i++)
                    bestSize[i] = blkList[i].size;
            }
            else if(exceed <= 0.05)
            {
                count = 0;
                bestCost = cost;
            }
            else
            {
                tmp = negInvLoci[idx1];
                negInvLoci[idx1] = negInvLoci[idx2];
                negInvLoci[idx2] = tmp;
                negLoci[negInvLoci[idx1]] = idx1;
                negLoci[negInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }

        }
        else
        {
            tmp = negInvLoci[idx1];
            negInvLoci[idx1] = negInvLoci[idx2];
            negInvLoci[idx2] = tmp;
            negLoci[negInvLoci[idx1]] = idx1;
            negLoci[negInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
            count++;
        }
    }
}

void SP::SwapBothLoci(int &overArea, int mode)
{
    int idx1 = rand() % blkNum;
    int idx2 = rand() % blkNum;
    int tmp = posInvLoci[idx1];
    posInvLoci[idx1] = posInvLoci[idx2];
    posInvLoci[idx2] = tmp;
    posLoci[posInvLoci[idx1]] = idx1;
    posLoci[posInvLoci[idx2]] = idx2;

    tmp = negInvLoci[idx1];
    negInvLoci[idx1] = negInvLoci[idx2];
    negInvLoci[idx2] = tmp;
    negLoci[negInvLoci[idx1]] = idx1;
    negLoci[negInvLoci[idx2]] = idx2;
    
    int oArea;
    UdtPosition();
    CalOverArea(oArea);
    //cout << oArea << endl;
    CalHPWL();
    CalCost();

    if(mode == 0)
    {
        if(overArea != 0 && overArea < oArea)
        {
            tmp = posInvLoci[idx1];
            posInvLoci[idx1] = posInvLoci[idx2];
            posInvLoci[idx2] = tmp;
            posLoci[posInvLoci[idx1]] = idx1;
            posLoci[posInvLoci[idx2]] = idx2;

            tmp = negInvLoci[idx1];
            negInvLoci[idx1] = negInvLoci[idx2];
            negInvLoci[idx2] = tmp;
            negLoci[negInvLoci[idx1]] = idx1;
            negLoci[negInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
        }
        else if(overArea == 0 && ((bestArea < area) || oArea != 0))
        {
            tmp = posInvLoci[idx1];
            posInvLoci[idx1] = posInvLoci[idx2];
            posInvLoci[idx2] = tmp;
            posLoci[posInvLoci[idx1]] = idx1;
            posLoci[posInvLoci[idx2]] = idx2;

            tmp = negInvLoci[idx1];
            negInvLoci[idx1] = negInvLoci[idx2];
            negInvLoci[idx2] = tmp;
            negLoci[negInvLoci[idx1]] = idx1;
            negLoci[negInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
        }
        else
        {
            bestArea = area;
            bestCost = cost;
            bestbestCost = cost;

            bestPosLoci.assign(posLoci.begin(), posLoci.end());
            bestNegLoci.assign(negLoci.begin(), negLoci.end());
            bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
            bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
            for(int i = 0; i < blkNum; i++)
                bestSize[i] = blkList[i].size;
            if(overArea == 0)
                overArea = 0;
            else
                overArea = oArea;
        }
    }
    else
    {
        double exceed = (cost - bestCost) * 1.0 / bestbestCost;
        if(count < ITER2)
        {
            if(bestCost < cost || oArea != 0)
            {
                tmp = posInvLoci[idx1];
                posInvLoci[idx1] = posInvLoci[idx2];
                posInvLoci[idx2] = tmp;
                posLoci[posInvLoci[idx1]] = idx1;
                posLoci[posInvLoci[idx2]] = idx2;

                tmp = negInvLoci[idx1];
                negInvLoci[idx1] = negInvLoci[idx2];
                negInvLoci[idx2] = tmp;
                negLoci[negInvLoci[idx1]] = idx1;
                negLoci[negInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }
            else if(oArea == 0)
            {
                if(cost < bestbestCost)
                {
                    bestbestCost = cost;
                    bestPosLoci.assign(posLoci.begin(), posLoci.end());
                    bestNegLoci.assign(negLoci.begin(), negLoci.end());
                    bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
                    bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
                    for(int i = 0; i < blkNum; i++)
                        bestSize[i] = blkList[i].size;
                    count = 0;
                }
                else
                    count++;
                bestCost = cost;
            }
            else
            {
                tmp = posInvLoci[idx1];
                posInvLoci[idx1] = posInvLoci[idx2];
                posInvLoci[idx2] = tmp;
                posLoci[posInvLoci[idx1]] = idx1;
                posLoci[posInvLoci[idx2]] = idx2;
                
                tmp = negInvLoci[idx1];
                negInvLoci[idx1] = negInvLoci[idx2];
                negInvLoci[idx2] = tmp;
                negLoci[negInvLoci[idx1]] = idx1;
                negLoci[negInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }
        }
        else if(oArea == 0)
        {
            if(cost < bestbestCost)
            {
                bestbestCost = cost;
                bestPosLoci.assign(posLoci.begin(), posLoci.end());
                bestNegLoci.assign(negLoci.begin(), negLoci.end());
                bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
                bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
                for(int i = 0; i < blkNum; i++)
                    bestSize[i] = blkList[i].size;
            }
            else if(exceed <= 0.1)
            {
                count = 0;
                bestCost = cost;
            }
            else
            {
                tmp = posInvLoci[idx1];
                posInvLoci[idx1] = posInvLoci[idx2];
                posInvLoci[idx2] = tmp;
                posLoci[posInvLoci[idx1]] = idx1;
                posLoci[posInvLoci[idx2]] = idx2;
                
                tmp = negInvLoci[idx1];
                negInvLoci[idx1] = negInvLoci[idx2];
                negInvLoci[idx2] = tmp;
                negLoci[negInvLoci[idx1]] = idx1;
                negLoci[negInvLoci[idx2]] = idx2;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }

        }
        else
        {
            tmp = posInvLoci[idx1];
            posInvLoci[idx1] = posInvLoci[idx2];
            posInvLoci[idx2] = tmp;
            posLoci[posInvLoci[idx1]] = idx1;
            posLoci[posInvLoci[idx2]] = idx2;

            tmp = negInvLoci[idx1];
            negInvLoci[idx1] = negInvLoci[idx2];
            negInvLoci[idx2] = tmp;
            negLoci[negInvLoci[idx1]] = idx1;
            negLoci[negInvLoci[idx2]] = idx2;
            UdtPosition();
            CalHPWL();
            CalCost();
            count++;
        }
    }
}

void SP::RttOneBlk(int &overArea, int mode)
{
    int idx = rand() % blkNum;
    int tmp = blkList[idx].size.width;
    blkList[idx].size.width = blkList[idx].size.height;
    blkList[idx].size.height = tmp;

    int oArea;
    UdtPosition();
    CalOverArea(oArea);
    //cout << oArea << endl;
    CalHPWL();
    CalCost();

    if(mode == 0)
    {
        if(overArea != 0 && overArea < oArea)
        {
            tmp = blkList[idx].size.width;
            blkList[idx].size.width = blkList[idx].size.height;
            blkList[idx].size.height = tmp;
            UdtPosition();
            CalHPWL();
            CalCost();
        }
        else if(overArea == 0 && ((bestArea < area) || oArea != 0))
        {
            tmp = blkList[idx].size.width;
            blkList[idx].size.width = blkList[idx].size.height;
            blkList[idx].size.height = tmp;
            UdtPosition();
            CalHPWL();
            CalCost();
        }
        else
        {
            bestArea = area;
            bestCost = cost;
            bestbestCost = cost;

            bestPosLoci.assign(posLoci.begin(), posLoci.end());
            bestNegLoci.assign(negLoci.begin(), negLoci.end());
            bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
            bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
            for(int i = 0; i < blkNum; i++)
                bestSize[i] = blkList[i].size;
            if(overArea == 0)
                overArea = 0;
            else
                overArea = oArea;
        }
    }
    else
    {
        double exceed = (cost - bestCost) * 1.0 / bestbestCost;
        if(count < ITER2)
        {
            if(bestCost < cost || oArea != 0)
            {
                tmp = blkList[idx].size.width;
                blkList[idx].size.width = blkList[idx].size.height;
                blkList[idx].size.height = tmp;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }
            else if(oArea == 0)
            {
                if(cost < bestbestCost)
                {
                    bestbestCost = cost;
                    bestPosLoci.assign(posLoci.begin(), posLoci.end());
                    bestNegLoci.assign(negLoci.begin(), negLoci.end());
                    bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
                    bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
                    for(int i = 0; i < blkNum; i++)
                        bestSize[i] = blkList[i].size;
                    count = 0;
                }
                else
                    count++;
                bestCost = cost;
            }
            else
            {
                tmp = blkList[idx].size.width;
                blkList[idx].size.width = blkList[idx].size.height;
                blkList[idx].size.height = tmp;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }
        }
        else if(oArea == 0)
        {
            if(cost < bestbestCost)
            {
                bestbestCost = cost;
                bestPosLoci.assign(posLoci.begin(), posLoci.end());
                bestNegLoci.assign(negLoci.begin(), negLoci.end());
                bestPosInvLoci.assign(posInvLoci.begin(), posInvLoci.end());
                bestNegInvLoci.assign(negInvLoci.begin(), negInvLoci.end());
                for(int i = 0; i < blkNum; i++)
                    bestSize[i] = blkList[i].size;  
            }
            else if(exceed <= 0.05)
            {
                count = 0;
                bestCost = cost;
            }
            else
            {
                tmp = blkList[idx].size.width;
                blkList[idx].size.width = blkList[idx].size.height;
                blkList[idx].size.height = tmp;
                UdtPosition();
                CalHPWL();
                CalCost();
                count++;
            }

        }
        else
        {
            tmp = blkList[idx].size.width;
            blkList[idx].size.width = blkList[idx].size.height;
            blkList[idx].size.height = tmp;
            UdtPosition();
            CalHPWL();
            CalCost();
            count++;
        }
    }
}

void SP::CalOverArea(int &oArea)
{
    if(outlineSize.width < curSize.width && outlineSize.height < curSize.height)
        oArea = (curSize.width - outlineSize.width) * outlineSize.height + (curSize.height - outlineSize.height) * outlineSize.width + (curSize.width - outlineSize.width) * (curSize.height - outlineSize.height);
    else if(outlineSize.width < curSize.width)
        oArea = (curSize.width - outlineSize.width) * outlineSize.height;
    else if(outlineSize.height < curSize.height)
        oArea = (curSize.height - outlineSize.height) * outlineSize.width;
    else
        oArea = 0;
}

void SP::CalHPWL()
{
    int sum = 0;   
    for(int i = 0; i < netNum; i++)
    {
        int right = 0;
        int left = INT_MAX;
        int up = 0;
        int down = INT_MAX;
        
        for(int j = 0; j < netList[i].blkNum; j++)
        {
            int midX = blkList[blkMap[netList[i].blkName[j]]].point.x + blkList[blkMap[netList[i].blkName[j]]].size.width / 2.0;
            int midY;
            if(blkList[blkMap[netList[i].blkName[j]]].blk)
                midY = blkList[blkMap[netList[i].blkName[j]]].point.y - blkList[blkMap[netList[i].blkName[j]]].size.height / 2.0 - minY;
            else
                midY = blkList[blkMap[netList[i].blkName[j]]].point.y - blkList[blkMap[netList[i].blkName[j]]].size.height / 2.0;
            if(midX > right)
                right = midX;
            if(midX < left)
                left = midX;
            if(midY > up)
                up = midY;
            if(midY < down)
                down = midY;
        }
        sum += right - left + up - down;
    }
    hpwl = sum;
}

void SP::CalCost()
{
    cost = alpha * area + (1.0 - alpha) * hpwl;
}

void SP::RttAllBlk()
{
    for(int i = 0; i < blkNum; i++)
    {
        int tmp = blkList[i].size.width;
        blkList[i].size.width = blkList[i].size.height;
        blkList[i].size.height = tmp;
    }

    reverse(negLoci.begin(), negLoci.end());
    for(int i = 0; i < blkNum; i++)
        negInvLoci[negLoci[i]] = i;
   
    UdtPosition();

    if(bestArea > area)
        bestArea = area;
}

void SP::Debugger()
{
    //PrintBlk();
    //PrintTmn();
    //PrintNet();
    PrintLoci();
    PrintInfo();
}

void SP::PrintBlk()
{
    cout << "===== Block =====" << endl;
    for(int i = 0; i < blkNum; i++)
        cout << blkList[i].name << ": " << blkList[i].size.width << " " << blkList[i].size.height << endl;
}

void SP::PrintTmn()
{
    cout << "===== Terminal =====" << endl;
    for(int i = blkNum; i < blkNum + tmnNum; i++)
        cout << blkList[i].name << ": " << blkList[i].size.width << " " << blkList[i].size.height << endl;
}

void SP::PrintNet()
{
    cout << "===== Net =====" << endl;
    for(int i = 0; i < netNum; i++)
        cout << netList[i].blkNum << endl;
}

void SP::PrintLoci()
{
    cout << "===== Positive Loci =====" << endl;
    for(int i = 0; i < blkNum; i++)
        cout << posLoci[i] << " ";
    cout << endl;

    cout << "===== Negative Loci =====" << endl;
    for(int i = 0; i < blkNum; i++)
        cout << negLoci[i] << " ";
    cout << endl;

    cout << "===== Positive Inverse Loci =====" << endl;
    for(int i = 0; i < blkNum; i++)
        cout << posInvLoci[i] << " ";
    cout << endl;

    cout << "===== Negative Inverse Loci =====" << endl;
    for(int i = 0; i < blkNum; i++)
        cout << negInvLoci[i] << " ";
    cout << endl;
}

void SP::PrintInfo()
{
    cout << "===== Info =====" << endl;
    for(int i = 0; i < blkNum; i++)
        cout << blkList[i].name << " " << blkList[i].point.x << " " << blkList[i].point.y - blkList[i].size.height - minY << " " << blkList[i].point.x + blkList[i].size.width << " " << blkList[i].point.y - minY << endl;
    cout << "Area: " << area << endl;
    cout << "Width: " << maxX << endl;
    cout << "Height: " << -minY << endl;
}

void SP::Dump(string outPath, string layPath, clock_t &startTime, clock_t &endTime)
{
    fstream outFile, layFile;
    outFile.open(outPath, ios::out);
    layFile.open(layPath, ios::out);
    layFile << outlineSize.width << " " << outlineSize.height << endl;
    posLoci.assign(bestPosLoci.begin(), bestPosLoci.end());
    negLoci.assign(bestNegLoci.begin(), bestNegLoci.end());
    posInvLoci.assign(bestPosInvLoci.begin(), bestPosInvLoci.end());
    negInvLoci.assign(bestNegInvLoci.begin(), bestNegInvLoci.end());
    for(int i = 0; i < blkNum; i++)
        blkList[i].size = bestSize[i];
    UdtPosition();
    CalHPWL();
    CalCost();
    outFile << cost << endl;
    outFile << hpwl << endl;
    outFile << area << endl;
    outFile << maxX << " " << -minY << endl;
    endTime = clock();
    outFile << double (endTime - startTime) * 1.0 / CLOCKS_PER_SEC << endl;
    for(int i = 0; i < blkNum; i++)
    {
        outFile << blkList[i].name << " " << blkList[i].point.x << " " << blkList[i].point.y - blkList[i].size.height - minY << " " << blkList[i].point.x + blkList[i].size.width << " " << blkList[i].point.y - minY << endl;
        layFile << blkList[i].name << " " << blkList[i].point.x << " " << blkList[i].point.y - blkList[i].size.height - minY << " " << blkList[i].point.x + blkList[i].size.width << " " << blkList[i].point.y - minY << endl;
    }
    endTime = clock();
    cout << "Total Execution Time: " << double (endTime - startTime) * 1.0 / CLOCKS_PER_SEC << "s\n";
}


# endif