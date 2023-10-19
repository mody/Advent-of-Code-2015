#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <cctype>
#include <fmt/core.h>

#include <bitset>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using Value = std::bitset<16>;

int main()
{
    std::unordered_map<std::string, Value> data;
    std::vector<std::string> queue;

    auto process = [&data](std::string const& line) -> bool {
        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" "));

        if (parts.at(1) == "->") {
            if (std::isdigit(parts.at(0).at(0))) {
                // direct value set
                data[parts.at(2)] = std::stoul(parts.at(0));
            } else {
                // register to register set
                auto it = data.find(parts.at(0));
                if (it != data.end()) {
                    data[parts.at(2)] = it->second;
                } else {
                    return false;
                }
            }
        } else if (parts.at(0) == "NOT") {
            assert(!std::isdigit(parts.at(1).at(0)));
            auto it = data.find(parts.at(1));
            if (it != data.end()) {
                data[parts.at(3)] = ~it->second;
            } else {
                return false;
            }
        } else {
            if (parts.at(1) == "AND") {
                auto it = data.find(parts.at(2));
                if (it == data.end()) {
                    return false;
                }
                Value x, y = it->second;
                if (std::isdigit(parts.at(0).at(0))) {
                    x = std::stoul(parts.at(0));
                } else {
                    it = data.find(parts.at(0));
                    if (it == data.end()) {
                        return false;
                    }
                    x = it->second;
                }
                data[parts.at(4)] = x & y;
            } else if (parts.at(1) == "OR") {
                auto it = data.find(parts.at(2));
                if (it == data.end()) {
                    return false;
                }
                Value x, y = it->second;
                if (std::isdigit(parts.at(0).at(0))) {
                    x = std::stoul(parts.at(0));
                } else {
                    it = data.find(parts.at(0));
                    if (it == data.end()) {
                        return false;
                    }
                    x = it->second;
                }
                data[parts.at(4)] = x | y;
            } else if (parts.at(1) == "LSHIFT") {
                auto it = data.find(parts.at(0));
                if (it == data.end()) {
                    return false;
                }
                assert(std::isdigit(parts.at(2).at(0)));
                unsigned x = std::stoul(parts.at(2));
                data[parts.at(4)] = it->second << x;
            } else if (parts.at(1) == "RSHIFT") {
                auto it = data.find(parts.at(0));
                if (it == data.end()) {
                    return false;
                }
                assert(std::isdigit(parts.at(2).at(0)));
                unsigned x = std::stoul(parts.at(2));
                data[parts.at(4)] = it->second >> x;
            } else {
                assert(false);
            }
        }
        return true;
    };

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) break;

        if (!process(line)) {
            queue.push_back(line);
        }
        for (auto it = queue.begin(); it != queue.end();) {
            if (process(*it)) {
                queue.erase(it);
                it = queue.begin();
            } else {
                ++it;
            }
        }
    }
    assert(queue.empty());

    // for (auto const& [k,v] : data) {
    //     fmt::print("{}: {}\n", k, v.to_ulong());
    // }

    fmt::print("1: {}\n", data.at("a").to_ulong());

    return 0;
}
