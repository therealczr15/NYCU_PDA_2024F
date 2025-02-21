# include "router.h"

int main(int argc, char* argv[])
{
    Router rt;
    rt.Read(argv[1], argv[2], argv[3]);
    rt.Dijkstra(argv[4], argv[5]);
    rt.Layout(argv[4], argv[6]);
    
    return 0;
}
