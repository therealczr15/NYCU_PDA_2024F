# include "legalizer.h"

using namespace std;

int main(int argc, char* argv[])
{
    Legalizer lglz;
    lglz.Read(argv[1]);
    lglz.Run(argv[2], argv[3]);
    //lglz.Dump(argv[4]);
    
    return 0;
}