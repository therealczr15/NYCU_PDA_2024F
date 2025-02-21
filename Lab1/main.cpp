# include "corner_stitching.h"

using namespace std;

int main(int argc, char* argv[])
{
    fstream in, out, lay;
    in.open(argv[1]);
    out.open(argv[2], ios::out);
    lay.open(argv[3], ios::out);

    int width, height;
    in >> width >> height;
    CornerStitching cs(width, height);
    
    string tmpString;
    Point lb, rt;

    vector<string> outString;

    while(in >> tmpString)
    {
        if(tmpString == "P" || tmpString == "p")
        {
            in >> lb.x >> lb.y;
            Tile* find = cs.PointFinding(lb);
            tmpString =  to_string(find -> lb.x) + " " + to_string(find -> lb.y) + '\n';
            outString.push_back(tmpString);
        }
        else
        {
            in >> lb.x >> lb.y >> width >> height;
            rt.x = lb.x + width;
            rt.y = lb.y + height;
            Tile* newTile = new Tile();
            setTile(newTile, lb, rt, stoi(tmpString));
            cs.TileInsertion(newTile);
            delete newTile;
            //cs.PrintTile();
        }
    }

    sort(cs.tileList.begin(), cs.tileList.end(), cmp);
    out << cs.totalTile << endl;
    cs.ShowAllTile(out);

    for(unsigned int i = 0; i < outString.size(); i++)
        out << outString[i];


    lay << cs.totalTile << endl;
    lay << cs.width << " " << cs.height << endl;

    Point p;
    p.x = 0;
    p.y = cs.height - 1;
    Tile* newTile = cs.PointFinding(p);  
    cs.EnumerateAllTile(newTile, lay);
    
    //for()
    
    return 0;
}