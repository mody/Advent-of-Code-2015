#include "point2d.h"

#include <fmt/core.h>

#include <iostream>
#include <map>
#include <unordered_set>

using Coord = int;
using Point = Gfx_2d::Point<Coord>;

static const std::map<unsigned char, Gfx_2d::Direction> MOVE2DIR = {
    {'^', Gfx_2d::North},
    {'>', Gfx_2d::East},
    {'v', Gfx_2d::South},
    {'<', Gfx_2d::West},
};

int main()
{
    std::string line;
    std::getline(std::cin, line);

    std::unordered_set<Point> world;

    Point px(0,0);
    world.insert(px);

    for (auto const& c : line) {
        px += MOVE2DIR.at(c);
        world.insert(px);
    }

    fmt::print("1: {}\n", world.size());

    return 0;
}
