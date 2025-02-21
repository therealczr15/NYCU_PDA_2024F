# include "sp.h"

using namespace std;

int main(int argc, char* argv[])
{
    clock_t startTime, endTime;
    startTime = clock();

    SP sp;
    sp.Read(stod(argv[1]), argv[2], argv[3]);
    sp.SimulatedAnnealing(startTime, endTime);
    sp.Dump(argv[4], argv[5], startTime, endTime);
    
    return 0;
}