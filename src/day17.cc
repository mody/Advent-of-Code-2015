#include <fmt/core.h>

#include <iostream>
#include <vector>

using Containers = std::vector<unsigned>;

constexpr const unsigned VOLUME = 150;

void part1(Containers const& containers)
{
    unsigned count = 0;
    for (unsigned mask = (1 << containers.size()) - 1; mask > 0; --mask) {
        unsigned sum = 0;
        for (unsigned i = 0; i < containers.size(); ++i) {
            sum += ((mask >> i) & 0x1) * containers.at(i);
        }
        if (sum == VOLUME) {
            ++count;
        }
    }

    fmt::print("1: {}\n", count);
}

void part2(Containers const& containers)
{
    unsigned min_containers = containers.size() + 1;
    unsigned count = 0;

    for (unsigned mask = (1 << containers.size()) - 1; mask > 0; --mask) {
        unsigned count_container = 0;
        unsigned sum = 0;
        for (unsigned i = 0; i < containers.size(); ++i) {
            if ((mask >> i) & 0x1) {
                sum += containers.at(i);
                ++count_container;
            }
        }
        if (sum == VOLUME) {
            if (min_containers > count_container) {
                min_containers = count_container;
                count = 1;
            } else if (min_containers == count_container) {
                ++count;
            }
        }
    }

    fmt::print("2: {}\n", count);
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
    part2(containers);
}
