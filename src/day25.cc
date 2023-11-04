#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    int target_x = 0, target_y = 0;
    {
        std::string line;
        std::getline(std::cin, line);
        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" ,."), boost::token_compress_on);
        target_y = std::stoul(parts.at(15));
        target_x = std::stoul(parts.at(17));
    }

    uint64_t i = 20151125;
    unsigned level = 0;
    for (bool stop = false; !stop; ++level) {
        for (int y = level - 1, x = 1; y > 0; --y, ++x) {
            if (x == target_x && y == target_y) {
                fmt::print("1: {}\n", i);
                stop = true;
                break;
            }
            i = (i * 252533) % 33554393;
        }
    }
}
