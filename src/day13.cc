#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>

#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>

#include <range/v3/view/drop.hpp>
#include <string>
#include <unordered_map>
#include <vector>

using namespace ranges;

using Data = std::unordered_map<std::string, std::unordered_map<std::string, int>>;

int main()
{
    Data seating;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" ."));

        // 0 name1
        // 2 gain/lose
        // 3 value
        // 10 name2
        int val = std::stoul(parts.at(3)) * (parts.at(2) == "gain" ? 1 : -1);
        seating[parts.at(0)][parts.at(10)] = val;
    }

    int max1 = 0;

    auto keys = seating | views::keys | to<std::vector> | actions::sort;
    do {
        int dist = 0;
        std::string from = keys.front();
        for (auto const& to : keys | views::drop(1)) {
            dist += seating.at(from).at(to);
            dist += seating.at(to).at(from);
            from = to;
        }
        dist += seating.at(from).at(keys.front());
        dist += seating.at(keys.front()).at(from);
        max1 = std::max(max1, dist);
    } while(ranges::next_permutation(keys));

    fmt::print("1: {}\n", max1);
}
