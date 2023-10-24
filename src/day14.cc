#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Reindeer
{
    unsigned speed = 0;
    unsigned runtime = 0;
    unsigned resttime = 0;
};

using Stats = std::unordered_map<std::string, Reindeer>;


unsigned process(Stats const& stats, unsigned const race_time)
{
    unsigned max_dist = 0;

    for (auto const& [name, r] : stats) {
        const unsigned whole_time = r.runtime + r.resttime;
        const unsigned div = race_time / whole_time;
        const unsigned rest = race_time - (div * whole_time);
        const unsigned dist = ((r.speed * r.runtime) * div) + std::min(r.runtime, rest) * r.speed;
        max_dist = std::max(max_dist, dist);
    }

    return max_dist;
}

int main()
{
    Stats stats;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" "));

        Reindeer r = {0};
        r.speed = std::stoul(parts.at(3));
        r.runtime = std::stoul(parts.at(6));
        r.resttime = std::stoul(parts.at(13));
        stats.insert({parts.at(0), std::move(r)});
    }

    fmt::print("1: {}\n", process(stats, 2503));
}
