#include "point2d.h"

#include <fmt/core.h>

#include <iostream>
#include <limits>
#include <unordered_map>

using Coord = int;
using Point = Gfx_2d::Point<Coord>;
using Data = std::unordered_map<Point, unsigned char>;

struct World
{
    Data data;

    Coord min_x {}, min_y {};
    Coord max_x {}, max_y {};

    void min_max() noexcept {
        min_x = std::numeric_limits<Coord>::max();
        min_y = std::numeric_limits<Coord>::max();
        max_x = std::numeric_limits<Coord>::min();
        max_y = std::numeric_limits<Coord>::min();

        for (auto const& [px, _] : data) {
            min_x = std::min(min_x, px.x);
            min_y = std::min(min_y, px.y);
            max_x = std::max(max_x, px.x);
            max_y = std::max(max_y, px.y);
        }
        assert(min_x >= 0);
        assert(min_y >= 0);
        assert(max_x < 100);
        assert(max_y < 100);
    }

    void dump() const noexcept
    {
        for (Coord y = min_y; y <= max_y; ++y) {
            for (Coord x = min_x; x <= max_x; ++x) {
                const Point p {x, y};
                const auto it = data.find(p);
                if (it != data.end()) {
                    fmt::print("{:c}", it->second);
                } else {
                    fmt::print(".");
                }
            }
            fmt::print("\n");
        }
        fmt::print("\n");
    }


    unsigned around(Point const& me) const noexcept
    {
        unsigned count = 0;
        for (auto dir :
             {Gfx_2d::North, Gfx_2d::South, Gfx_2d::West, Gfx_2d::East, Gfx_2d::NW, Gfx_2d::NE, Gfx_2d::SW, Gfx_2d::SE})
        {
            if (data.contains(me + dir)) ++count;
        }
        return count;
    }
};

void part1(World world)
{
    for (unsigned i = 0; i < 100; ++i) {
        Data next;
        world.min_max();
        for (Coord y = world.min_y; y <= world.max_y; ++y) {
            for (Coord x = world.min_x; x <= world.max_x; ++x) {
                const unsigned neighbours = world.around({x, y});
                if (world.data.contains({x, y})) {
                    if (neighbours == 2 || neighbours == 3) {
                        next.insert({{x, y}, '#'});
                    }
                } else {
                    if (neighbours == 3) {
                        next.insert({{x, y}, '#'});
                    }
                }
            }
        }
        std::swap(next, world.data);
    }
    fmt::print("1: {}\n", world.data.size());
}

int main()
{
    World world;
    Coord y = 0, x = 0;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        x = 0;
        for (unsigned char c : line) {
            if (c == '#') {
                world.data.insert({{x, y}, c});
            }
            ++x;
        }
        ++y;
    }
    world.min_max();

    part1(world);

    return 0;
}
