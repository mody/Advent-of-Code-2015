#include "point2d.h"

#include <boost/regex.hpp>
#include <fmt/core.h>

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <unordered_set>


using Coord = unsigned;
using Point = Gfx_2d::Point<Coord>;
using World1 = std::unordered_set<Point>;
using World2 = std::unordered_map<Point, int>;

static const boost::regex RX("([a-z]+) ([0-9]+),([0-9]+) through ([0-9]+),([0-9]+)$");

int main()
{
    World1 world1;
    World2 world2;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;

        boost::smatch res;
        if (boost::regex_search(line, res, RX)) {
            assert(res.size() == 6);
            const Coord x1 = std::stoul(res.str(2));
            const Coord y1 = std::stoul(res.str(3));
            const Coord x2 = std::stoul(res.str(4));
            const Coord y2 = std::stoul(res.str(5));

            if (res.str(1) == "off") {
                for (Coord y = y1; y <= y2; ++y) {
                    for (Coord x = x1; x <= x2; ++x) {
                        world1.erase({x, y});
                        auto it = world2.insert({{x, y}, 0}).first;
                        it->second--;
                        if (it->second < 0) {
                            it->second = 0;
                        }
                    }
                }
            } else if (res.str(1) == "on") {
                for (Coord y = y1; y <= y2; ++y) {
                    for (Coord x = x1; x <= x2; ++x) {
                        world1.insert({x, y});
                        world2.insert({{x, y}, 0}).first->second++;
                    }
                }
            } else if (res.str(1) == "toggle") {
                for (Coord y = y1; y <= y2; ++y) {
                    for (Coord x = x1; x <= x2; ++x) {
                        auto it = world1.find({x, y});
                        if (it == world1.end()) {
                            world1.insert({x, y});
                        } else {
                            world1.erase(it);
                        }
                        world2.insert({{x, y}, 0}).first->second += 2;
                    }
                }
            }
        }
    }

    fmt::print("1: {}\n", world1.size());

    unsigned lights = 0;
    for (auto const& [_, light] : world2) {
        assert(light >= 0);
        lights += light;
    }
    fmt::print("2: {}\n", lights);  // 14822364 too low
}
