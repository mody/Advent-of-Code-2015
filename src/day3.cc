#include "point2d.h"

#include <fmt/core.h>

#include <array>
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

void part1(const std::string& line)
{
    std::unordered_set<Point> world;

    Point px(0,0);
    world.insert(px);

    for (auto const& c : line) {
        px += MOVE2DIR.at(c);
        world.insert(px);
    }

    fmt::print("1: {}\n", world.size());
}

void part2(const std::string& line)
{
    std::unordered_set<Point> world;

    std::array<Point, 2> px;
    world.insert(px.at(0));

    unsigned index = 0;

    for (auto const& c : line) {
        px.at(index) += MOVE2DIR.at(c);
        world.insert(px.at(index));
        index = (index + 1) % px.size();
    }

    fmt::print("2: {}\n", world.size());
}

int main()
{
    std::string line;
    std::getline(std::cin, line);

    part1(line);
    part2(line);

    return 0;
}
