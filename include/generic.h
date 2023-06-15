#ifndef GENERIC_H
#define GENERIC_H

#include <fstream>
#include <string>
#include <sstream>

using std::string;

namespace Util
{
    template <typename T>
    T findValueByKey(std::string const keyFilter, std::string const& path) {
        string line;
        string key;
        T value;
        std::ifstream filestream(path);
        if (filestream.is_open()) {
            while (std::getline(filestream, line)) {
                std::istringstream linestream(line);
                while (linestream >> key >> value) {
                    if (key == keyFilter) {
                        return value;
                    }
                }
            }
        }
        return value;
    }
}

#endif