#include "format.h"

#include <string>
#include <iomanip>
#include <sstream>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    std::ostringstream formatted_time;
    formatted_time << std::setw(2) << std::setfill('0') << seconds / 3600 << ":"
        << std::setw(2) << std::setfill('0') << (seconds % 3600) / 60 << ":"
        << std::setw(2) << std::setfill('0') << seconds % 60;

    return formatted_time.str();
}