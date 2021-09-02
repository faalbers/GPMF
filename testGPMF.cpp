#include "GPMF.cpp"
#include <iostream>
#include <iomanip>
#include <fstream>

int main(int argc, char* argv[])
{
    if ( argc == 1 ) return 1;

    std::cout << "**** CREATE GPMF ****\n\n";

    GPMF::GPMF gpmf(argv[1]);

    std::cout << "\n**** TEST GPMF ****\n\n";
    
    //gpmf.printHierarchy();
    
    //gpmf.printHierarchyData();

    if (false) {
        //auto sampels = gpmf.getAcceleration();
        //auto sampels = gpmf.getGyroscope();
        auto sampels = gpmf.getGPS();
        std::cout << sampels.size() << std::endl;
        std::cout.precision(5);
        std::cout << std::fixed;
        int fixedWith = 10;
        int index = 1;
        for ( auto sampel : sampels ) {
            std::cout << "Sample: " << index << std::endl;
            std::cout << "  " << sampel.time.info << ": " << sampel.time.value << " " << sampel.time.unit << std::endl;
            std::cout << "  " << sampel.duration.info << ": " << sampel.duration.value << " " << sampel.duration.unit << std::endl;
            for ( auto entry : sampel.entries )
                std::cout << "    " << entry.info << ": " << entry.value << " " << entry.unit << std::endl;
            index++;
        }
    }

    gpmf.exportGPStoGPX("Blah");

    std::cout << "\n**** END ****\n\n";

    return 0;
}