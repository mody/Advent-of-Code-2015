#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>

#include <range/v3/action/sort.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Reindeer
{
    unsigned speed = 0;
    unsigned runtime = 0;
    unsigned resttime = 0;
    unsigned distance = 0;
    unsigned score = 0;
};

using Stats = std::unordered_map<std::string, Reindeer>;


unsigned fast_way(Stats const& stats, unsigned const race_time)
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

unsigned simulate(Stats const& stats, unsigned const race_time)
{
    unsigned max_score = 0;

    std::vector<Reindeer> data =
        stats | ranges::views::transform([](auto&& v) { return v.second; }) | ranges::to_vector;

    for (unsigned s = 0; s < race_time; ++s) {
        for (auto& r : data) {
            const unsigned split = s % (r.runtime + r.resttime);
            if (split < r.runtime) {
                r.distance += r.speed;
            }
        }
        ranges::actions::sort(data, [](auto const& lhs, auto const& rhs) { return lhs.distance > rhs.distance; });
        const unsigned best = data.front().distance;
        for (auto& r : data) {
            if (r.distance == best) {
                ++r.score;
            } else {
                break;
            }
        }
    }

    for (auto const& r : data) {
        max_score = std::max(max_score, r.score);
    }

    return max_score;
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

    fmt::print("1: {}\n", fast_way(stats, 2503));
    fmt::print("2: {}\n", simulate(stats, 2503));
}
