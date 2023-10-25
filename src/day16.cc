#include <array>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>

#include <range/v3/view/enumerate.hpp>

#include <functional>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

enum
{
    children = 0,
    cats,
    samoyeds,
    pomeranians,
    akitas,
    vizslas,
    goldfish,
    trees,
    cars,
    perfumes,
    SIZE,
};

static const std::unordered_map<std::string, unsigned> to_id = {
    {"children",       children},
    {"cats",           cats},
    {"samoyeds",       samoyeds},
    {"pomeranians",    pomeranians},
    {"akitas",         akitas},
    {"vizslas",        vizslas},
    {"goldfish",       goldfish},
    {"trees",          trees},
    {"cars",           cars},
    {"perfumes",       perfumes},
};

using Cell = std::optional<char>;
using Memory = std::array<Cell, SIZE>;

using Comaparator = std::function<bool(const Cell&, const Cell&)>;

static const std::array<Comaparator, SIZE> to_compare = {
    std::equal_to<Cell>(),
    std::greater<Cell>(),
    std::equal_to<Cell>(),
    std::less<Cell>(),
    std::equal_to<Cell>(),
    std::equal_to<Cell>(),
    std::less<Cell>(),
    std::greater<Cell>(),
    std::equal_to<Cell>(),
    std::equal_to<Cell>(),
};


bool compare_memory1(const Memory& memory, const Memory& evidence) noexcept
{
    bool same = true;
    for (auto const& [idx, m] : ranges::views::enumerate(memory)) {
        if (m.has_value()) {
            same = same && (m == evidence.at(idx));
        }
    }
    return same;
}

bool compare_memory2(const Memory& memory, const Memory& evidence) noexcept
{
    bool same = true;
    for (auto const& [idx, m] : ranges::views::enumerate(memory)) {
        if (m.has_value()) {
            same = same && to_compare.at(idx)(m, evidence.at(idx));
        }
    }
    return same;
}

int main()
{
    static const Memory evidence = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};
    std::vector<Memory> memories;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" :,"), boost::token_compress_on);

        Memory m;
        m.at(to_id.at(parts.at(2))) = std::stoi(parts.at(3));
        m.at(to_id.at(parts.at(4))) = std::stoi(parts.at(5));
        m.at(to_id.at(parts.at(6))) = std::stoi(parts.at(7));
        memories.push_back(std::move(m));
    }

    for (auto const& [idx, m] : ranges::views::enumerate(memories)) {
        if (compare_memory1(m, evidence)) {
            fmt::print("1: {}\n", idx+1);
        }
        if (compare_memory2(m, evidence)) {
            fmt::print("2: {}\n", idx+1);
        }
    }
}
