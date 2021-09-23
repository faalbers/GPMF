#include "GPMF.cpp"
#include <iostream>
#include <iomanip>
#include <chrono>


int main(int argc, char* argv[])
{
    if ( argc == 1 ) return 1;
    
    auto createStart = std::chrono::high_resolution_clock::now();

    std::cout << "**** CREATE GPMF ****\n\n";

    GPMF::GPMF gpmf(argv[1]);

    auto testStart = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(testStart - createStart);
    std::cout << "\nCreate Time: " << ms_int.count() << "ms\n";
    std::cout << "**** TEST GPMF ****\n\n";
    
    gpmf.printHierarchy();
    
    //gpmf.printHierarchyData();

    if (false) {
        //auto sampels = gpmf.getAcceleration();
        auto sampels = gpmf.getGyroscope();
        //auto sampels = gpmf.getGPS();
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

    auto end = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - testStart);
    std::cout << "\nTest Time : " << ms_int.count() << "ms\n";
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - createStart);
    std::cout << "Total Time: " << ms_int.count() << "ms\n";
    std::cout << "**** END ****\n\n";

    return 0;
}