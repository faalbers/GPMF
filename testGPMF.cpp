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

    if (false) {
        std::ofstream out("GPS_export.gpx", std::ios::binary);
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        out << "<gpx>\n\t<trk>\n\t\t<trkseg>\n";
        out << "\t\t\t<trkpt lat=\"" << 0.228990;
        out << "\" lon=\"" << 37.307772;
        out << "\"><time>2007-01-01T00:00:26Z</time></trkpt>\n";
        out << "\t\t</trkseg>\n\t</trk>\n</gpx>\n";
        out.close();
    }
    if (true) {
        std::ofstream out("GPS_export.gpx", std::ios::binary);
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        out << "<gpx>\n\t<trk>\n\t\t<trkseg>\n";
        auto sampels = gpmf.getGPS();
        std::cout << sampels.size() << std::endl;
        std::cout.precision(5);
        std::cout << std::fixed;
        int fixedWith = 10;
        float fracseconds;
        int seconds, minutes, hours;
        for ( auto sampel : sampels ) {
            seconds = (int) sampel.time.value;
            fracseconds = sampel.time.value - (float) seconds;
            minutes = seconds / 60;
            hours = minutes / 60;
            seconds %= 60;
            minutes %= 60;
            hours %= 24;
            fracseconds += seconds;
            //out << std::setprecision(9);
            out << std::fixed << std::setprecision(7);
            out << "\t\t\t<trkpt lat=\"" << sampel.entries[0].value;
            out << "\" lon=\"" << sampel.entries[1].value;
            out << "\"><time>2007-01-01T" << std::setfill('0')
            << std::setw(2) << hours << ":"
            << std::setw(2) << minutes << ":" 
            << std::setprecision(3) << std::setw(6) << fracseconds << "Z</time></trkpt>\n";
            /*
            out << "\"><time>2007-01-01T" << std::setfill('0')
            << std::setw(2) << hours << ":"
            << std::setw(2) << minutes << ":" 
            << std::fixed << std::setprecision(3) << seconds << "Z</time></trkpt>\n";
            */
        }
        out << "\t\t</trkseg>\n\t</trk>\n</gpx>\n";
        out.close();
    }

    std::cout << "\n**** END ****\n\n";

    return 0;
}