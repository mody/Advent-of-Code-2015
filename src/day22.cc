#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <boost/container_hash/hash_fwd.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/detail/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <fmt/core.h>

#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

struct Player
{
    std::string name;
    int hitpoints = 0;
    int damage = 0;
    int armor = 0;
    int mana = 0;

    bool operator==(const Player& oth) const noexcept
    {
        return hitpoints == oth.hitpoints && damage == oth.damage && armor == oth.armor && mana == oth.mana;
    }
};

struct Spell
{
    std::string name;
    int cost = 0;
    int lasts = 0;
    int damage = 0;
    int armor = 0;
    int heal = 0;
    int mana = 0;

    bool operator==(const Spell& oth) const noexcept
    {
        return cost == oth.cost && lasts == oth.lasts && damage == oth.damage && armor == oth.armor && heal == oth.heal
            && mana == oth.mana;
    }
};

using Spells = std::unordered_map<Spell, int, boost::hash<Spell>>;

static const Spells SPELLS = {
    {{"Magic Missile", 53, 0, 4, 0, 0, 0}, 0},
    {{"Drain", 73, 0, 2, 0, 2, 0}, 0},
    {{"Shield", 113, 6, 0, 7, 0, 0}, 0},
    {{"Poison", 173, 6, 3, 0, 0, 0}, 0},
    {{"Recharge", 229, 5, 0, 0, 0, 101}, 0},
};


struct State
{
    Player me, boss;
    Spells spells;
    unsigned spent = 0;

    bool operator==(State const& rhs) const noexcept
    {
        return me == rhs.me && boss == rhs.boss && spells == rhs.spells && spent == rhs.spent;
    }
};

size_t hash_value(const Player& p) noexcept
{
    size_t seed = 0;
    boost::hash_combine(seed, p.name);
    boost::hash_combine(seed, p.hitpoints);
    boost::hash_combine(seed, p.damage);
    boost::hash_combine(seed, p.armor);
    boost::hash_combine(seed, p.mana);
    return seed;
}

size_t hash_value(const Spell& s) noexcept {
    size_t seed = 0;
    boost::hash_combine(seed, s.name);
    boost::hash_combine(seed, s.cost);
    boost::hash_combine(seed, s.lasts);
    boost::hash_combine(seed, s.damage);
    boost::hash_combine(seed, s.armor);
    boost::hash_combine(seed, s.heal);
    boost::hash_combine(seed, s.mana);
    return seed;
}

template<>
struct boost::hash<State>
{
    size_t operator() (const State& s) const noexcept {
        size_t seed = 0;
        boost::hash_combine(seed, s.me);
        boost::hash_combine(seed, s.boss);
        boost::hash_combine(seed, s.spells);
        boost::hash_combine(seed, s.spent);
        return seed;
    }
};

void game(Player const& boss, int extra_damage = 0)
{
    State beginning;

    beginning.me.name = "ME";
    beginning.me.hitpoints = 50;
    beginning.me.mana = 500;

    beginning.boss = boss;

    beginning.spells = SPELLS;

    unsigned least_spent = 2000;

    std::deque<State> work;
    work.push_back(std::move(beginning));

    auto apply_spells = [](State& next) {
        next.me.armor = 0;
        for (auto& [spell, turns] : next.spells) {
            if (turns > 0) {
                // fmt::print("\tapply {}\n", spell.name);
                next.me.armor += spell.armor;
                next.me.mana += spell.mana;
                next.boss.hitpoints -= spell.damage;
                --turns;
            }
        }
    };

    while(!work.empty()) {
        const State now = work.front();
        work.pop_front();

        // fmt::print(
        //     "State - me: {}, boss: {}, mana: {}, spent: {}\n", now.me.hitpoints, now.boss.hitpoints, now.me.mana,
        //     now.spent);
        assert(now.me.hitpoints > 0);
        assert(now.boss.hitpoints > 0);

        ////////// MY TURN //////////

        // make move
        State next1 = now;

        next1.me.hitpoints -= extra_damage;

        // resolve active spells
        apply_spells(next1);
        // fmt::print("\tapplied spells - me: {}, boss: {}\n", next1.me.hitpoints, next1.boss.hitpoints);

        // check boss hitpoints
        if (next1.boss.hitpoints <= 0) {
            // WE WON
            least_spent = std::min(least_spent, next1.spent);
            // fmt::print("\t!! WE WON @ {}\n", least_spent);
            continue;
        }

        // cast a spell
        for (auto& [spell, turns] : next1.spells) {
            if (turns > 0 || next1.me.mana < spell.cost) {
                continue;
            }

            State next2 = next1;

            next2.me.mana -= spell.cost;
            next2.spent += spell.cost;

            // fmt::print("\tcast {} for {}\n", spell.name, spell.cost);

            if (next2.spent > least_spent) {
                // fmt::print("\t!! drop this branch\n");
                continue;
            }

            if (spell.lasts == 0) {
                // instant spell
                next2.boss.hitpoints -= spell.damage;
                next2.me.hitpoints += spell.heal;
                // fmt::print("\tinstant spell - me: {}, boss: {}\n", next2.me.hitpoints, next2.boss.hitpoints);
                //Check boss
                if (next2.boss.hitpoints <= 0) {
                    //WE WON
                    least_spent = std::min(least_spent, next2.spent);
                    // fmt::print("\t!! WE WON @ {}\n", least_spent);
                    continue;
                }
            } else {
                next2.spells.at(spell) = spell.lasts;
            }

            ////////// BOSS TURN //////////
            // fmt::print("\tBOSS turn\n");

            // resolve active spells
            apply_spells(next2);
            // fmt::print("\tapplied spells - me: {}, boss: {}\n", next2.me.hitpoints, next2.boss.hitpoints);

            if (next2.boss.hitpoints <= 0) {
                //WE WON
                least_spent = std::min(least_spent, next2.spent);
                // fmt::print("\t!! WE WON @ {}\n", least_spent);
                continue;
            }

            //BOSS attacks
            next2.me.hitpoints -= std::max(1, next2.boss.damage - next2.me.armor);

            // fmt::print("\tboss attacked - me: {}, boss: {}\n", next2.me.hitpoints, next2.boss.hitpoints);

            // check me
            if (next2.me.hitpoints <= 0) {
                //WE LOST
                // fmt::print("\t!! WE LOST\n");
                continue;
            }
            work.push_back(std::move(next2));
            // fmt::print(
            //     "\t-- state pushed - me: {}, boss: {}, mana: {}\n", next2.me.hitpoints, next2.boss.hitpoints,
            //     next2.me.mana);
        }
    }

    fmt::print("1: {}\n", least_spent);
}

int main()
{
    Player boss = {.name = "BOSS"};

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
    game(boss, 1);
}
