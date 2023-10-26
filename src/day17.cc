#include <fmt/core.h>

#include <iostream>
#include <vector>

using Containers = std::vector<unsigned>;

void part1(Containers const& containers)
{
    unsigned count = 0;
    for (unsigned mask = (1 << containers.size()) - 1; mask > 0; --mask) {
        unsigned sum = 0;
        for (unsigned i = 0; i < containers.size(); ++i) {
            sum += ((mask >> i) & 0x1) * containers.at(i);
        }
        if (sum == 150) {
            ++count;
        }
    }

    fmt::print("1: {}\n", count);
}

int main()
{
    Containers containers;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        containers.push_back(std::stoul(line));
    }

    part1(containers);
}
