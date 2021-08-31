#include "GPMF.cpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if ( argc == 1 ) return 1;

    std::cout << "**** CREATE GPMF ****\n\n";

    GPMF::GPMF gpmf(argv[1]);

    std::cout << "\n**** TEST GPMF ****\n\n";
    
    //gpmf.printHierarchy();
    gpmf.printHierarchyData();
    
    std::cout << "\n**** END ****\n\n";

    return 0;
}