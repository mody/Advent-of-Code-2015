#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Rules = std::unordered_multimap<std::string, std::string>;

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

int main()
{
    Rules rules;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" =>"), boost::token_compress_on);

        rules.insert({parts.at(0), parts.at(1)});
    }

    std::getline(std::cin, line);

    part1(rules, line);
}
