#include "point2d.h"

#include <boost/regex.hpp>
#include <fmt/core.h>

#include <cassert>
#include <iostream>
#include <unordered_set>


using Coord = unsigned;
using Point = Gfx_2d::Point<Coord>;
using World = std::unordered_set<Point>;

static const boost::regex RX("([a-z]+) ([0-9]+),([0-9]+) through ([0-9]+),([0-9]+)$");

int main()
{
    World world;

    std::string line;
    while(std::getline(std::cin, line))
    {
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
                        world.erase({x, y});
                    }
                }
            } else if (res.str(1) == "on") {
                for (Coord y = y1; y <= y2; ++y) {
                    for (Coord x = x1; x <= x2; ++x) {
                        world.insert({x, y});
                    }
                }
            } else if (res.str(1) == "toggle") {
                for (Coord y = y1; y <= y2; ++y) {
                    for (Coord x = x1; x <= x2; ++x) {
                        auto it = world.find({x, y});
                        if (it == world.end()) {
                            world.insert({x, y});
                        } else {
                            world.erase(it);
                        }
                    }
                }
            }
        }
    }

    fmt::print("1: {}\n", world.size());
}
