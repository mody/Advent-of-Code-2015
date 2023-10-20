#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fmt/core.h>
#include <range/v3/view/drop.hpp>

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Distances = std::unordered_map<std::string, std::unordered_map<std::string, unsigned>>;

int main()
{
    std::unordered_set<std::string> cities;
    Distances distances;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" "));

        cities.insert(parts.at(0));
        cities.insert(parts.at(2));
        distances[parts.at(0)][parts.at(2)] = std::stoul(parts.at(4));
        distances[parts.at(2)][parts.at(0)] = std::stoul(parts.at(4));
    }

    std::vector<std::string> path;
    for (auto const& c : cities) {
        path.push_back(c);
    }
    std::ranges::sort(path);

    unsigned shortest = std::numeric_limits<unsigned>::max(), longest = 0;
    do {
        unsigned dist = 0;
        std::string from = path.front();
        for (auto const& next : path | ranges::views::drop(1)) {
            dist += distances.at(from).at(next);
            from = next;
        }
        shortest = std::min(shortest, dist);
        longest = std::max(longest, dist);
    } while (std::ranges::next_permutation(path).found);

    fmt::print("1: {}\n", shortest);
    fmt::print("2: {}\n", longest);
}
