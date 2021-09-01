#include "GPMF.cpp"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[])
{
    if ( argc == 1 ) return 1;

    std::cout << "**** CREATE GPMF ****\n\n";

    GPMF::GPMF gpmf(argv[1]);

    std::cout << "\n**** TEST GPMF ****\n\n";
    
    //gpmf.printHierarchy();
    
    //gpmf.printHierarchyData();

    if (false) {
        auto accels = gpmf.getAcceleration();
        std::cout << accels.size() << std::endl;
        std::cout.precision(5);
        std::cout << std::fixed;
        int fixedWith = 10;
        int index = 1;
        for ( auto accel : accels ) {
            std::cout << "Sample: " << index << std::endl;
            std::cout << "  " << accel.time.info << ": " << accel.time.value << " " << accel.time.unit << std::endl;
            std::cout << "  " << accel.duration.info << ": " << accel.duration.value << " " << accel.duration.unit << std::endl;
            for ( auto entry : accel.entries )
                std::cout << "    " << entry.info << ": " << entry.value << " " << entry.unit << std::endl;
            index++;
        }
    }

    if (false) {
        auto accels = gpmf.getGyroscope();
        std::cout << accels.size() << std::endl;
        std::cout.precision(5);
        std::cout << std::fixed;
        int fixedWith = 10;
        int index = 1;
        for ( auto accel : accels ) {
            std::cout << "Sample: " << index << std::endl;
            std::cout << "  " << accel.time.info << ": " << accel.time.value << " " << accel.time.unit << std::endl;
            std::cout << "  " << accel.duration.info << ": " << accel.duration.value << " " << accel.duration.unit << std::endl;
            for ( auto entry : accel.entries )
                std::cout << "    " << entry.info << ": " << entry.value << " " << entry.unit << std::endl;
            index++;
        }
    }

    if (true) {
        auto GPSs = gpmf.getGPS();
        std::cout << GPSs.size() << std::endl;
    }

    std::cout << "\n**** END ****\n\n";

    return 0;
}