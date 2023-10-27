#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/detail/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <fmt/core.h>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using Rules = std::vector<std::pair<std::string, std::string>>;

void part1(Rules const& rules, std::string const& input)
{
    std::unordered_set<std::string> output;

    for (auto const& [from, to] : rules) {
        for (auto at = input.find(from); at != std::string::npos; at = input.find(from, at)) {
            std::string replaced = input.substr(0, at) + to + input.substr(at + from.size());
            // fmt::print("{} => {}; {} -> {} @{}\n", from, to, input, replaced, at);
            output.insert(std::move(replaced));
            at += from.size();
        }
    }

    fmt::print("1: {}\n", output.size());
}

void part2(Rules rules, std::string molecule)
{
    std::random_device rd;
    std::mt19937 g(rd());

    for (;;) {
        std::string input = molecule;
        std::shuffle(rules.begin(), rules.end(), g);
        unsigned count = 0;

        for (bool todo = true; input != "e" && todo;) {
            todo = false;
            for (auto const& [from, to] : rules) {
                for (auto at = input.find(to); at != std::string::npos; at = input.find(to, at)) {
                    input = input.substr(0, at) + from + input.substr(at + to.size());
                    todo = true;
                    ++count;
                    at += from.size();
                }
            }
        }

        if (input == "e") {
            fmt::print("2: {}\n", count);
            break;
        }
        // fmt::print(".");
    }
}


int main()
{
    Rules rules;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" =>"), boost::token_compress_on);

        rules.push_back({parts.at(0), parts.at(1)});
    }

    std::getline(std::cin, line);

    part1(rules, line);
    part2(rules, line);
}
