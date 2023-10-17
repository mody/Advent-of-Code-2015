#include <fmt/core.h>
#include <range/v3/view/drop.hpp>

#include <iostream>
#include <set>
#include <string_view>

using namespace ranges;

bool is_nice(std::string_view input)
{
    static const std::set<unsigned char> wowels = {'a', 'e', 'i', 'o', 'u'};
    static const std::set<std::string> illegals = {"ab", "cd", "pq", "xy"};

    unsigned char c1 = input.front();

    unsigned has_wowel = wowels.contains(c1);
    bool has_double = false;
    std::string key(2, '\0');

    for (unsigned char c2 : input | views::drop(1)) {
        has_wowel += wowels.contains(c2);
        has_double |= c1 == c2;
        key.at(0) = c1;
        key.at(1) = c2;
        if (illegals.contains(key)) return false;
        c1 = c2;
    }
    return has_wowel > 2 && has_double;
}

int main()
{
    unsigned nice = 0;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;
        nice += is_nice(line);
    }

    fmt::print("1: {}\n", nice);
}
