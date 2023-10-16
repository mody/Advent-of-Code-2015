#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

int main()
{
    uint64_t area =0;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of("x"));
        assert(parts.size() == 3);

        std::vector<unsigned> sizes(3, 0);
        sizes.at(0) = std::stoul(parts.at(0));
        sizes.at(1) = std::stoul(parts.at(1));
        sizes.at(2) = std::stoul(parts.at(2));

        std::ranges::sort(sizes);

        area += (sizes.at(0) * sizes.at(1)) + (2 * sizes.at(0) * sizes.at(1)) + (2 * sizes.at(1) * sizes.at(2))
            + (2 * sizes.at(0) * sizes.at(2));
    }

    fmt::print("1: {}\n", area);

    return 0;
}
