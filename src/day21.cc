#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>
#include <iostream>
#include <limits>
#include <vector>

struct Player
{
    int hitpoints = 0;
    int damage = 0;
    int armor = 0;
};

struct Item
{
    std::string name;
    int cost = 0;
    int damage = 0;
    int armor = 0;
};

static const std::vector<Item> WEAPONS = {
    {"Dagger",        8,     4,       0},
    {"Shortsword",   10,     5,       0},
    {"Warhammer",    25,     6,       0},
    {"Longsword",    40,     7,       0},
    {"Greataxe",     74,     8,       0}
};

static const std::vector<Item> ARMORS = {
    {"<none>",        0,     0,       0},
    {"Leather",      13,     0,       1},
    {"Chainmail",    31,     0,       2},
    {"Splintmail",   53,     0,       3},
    {"Bandedmail",   75,     0,       4},
    {"Platemail",   102,     0,       5}
};

static const std::vector<Item> RINGS = {
    {"<none>",        0,     0,       0},
    {"Damage +1",    25,     1,       0},
    {"Damage +2",    50,     2,       0},
    {"Damage +3",   100,     3,       0},
    {"Defense +1",   20,     0,       1},
    {"Defense +2",   40,     0,       2},
    {"Defense +3",   80,     0,       3}
};

bool fight(Player me, Player boss)
{
    Player* attacks = &me, *defends = &boss;
    for (; defends->hitpoints > 0;) {
        defends->hitpoints -= std::max(1, (attacks->damage - defends->armor));
        std::swap(attacks, defends);
    }
    return me.hitpoints > boss.hitpoints;
}

void game(Player const& boss)
{
    int cheapest = std::numeric_limits<int>::max();
    int most_expensive_loss = 0;

    for (auto const& ring1 : RINGS) {
        for (auto const& ring2 : RINGS) {
            if (ring1.name == ring2.name) continue;
            for (auto const& armor : ARMORS) {
                for (auto const& weapon : WEAPONS) {
                    Player me;
                    me.hitpoints = 100;
                    me.damage = weapon.damage + ring1.damage + ring2.damage;
                    me.armor = armor.armor + ring1.armor + ring2.armor;
                    const int cost = weapon.cost + armor.cost + ring1.cost + ring2.cost;
                    if (fight(me, boss)) {
                        cheapest = std::min(cheapest, cost);
                    } else {
                        most_expensive_loss = std::max(most_expensive_loss, cost);
                    }
                }
            }
        }
    }
    fmt::print("1: {}\n2: {}\n", cheapest, most_expensive_loss);
}

int main()
{
    Player boss;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" :"));
        if (parts.front() == "Hit") {
            boss.hitpoints = std::stoul(parts.back());
        } else if (parts.front() == "Damage") {
            boss.damage = std::stoul(parts.back());
        } else if (parts.front() == "Armor") {
            boss.armor = std::stoul(parts.back());
        } else {
            assert(false);
        }
    }

    game(boss);
}
