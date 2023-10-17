#include <fmt/core.h>
#include <range/v3/view/drop.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <set>
#include <string_view>

using namespace ranges;

bool is_nice1(std::string_view input)
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

bool is_nice2(std::string const& input)
{
    static const boost::regex RX1("(..).*\\1");
    static const boost::regex RX2("(.).\\1");

    boost::smatch res;
    return boost::regex_search(input, res, RX1) && boost::regex_search(input, res, RX2);
}

int main()
{
    unsigned nice1 = 0, nice2 = 0;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;
        nice1 += is_nice1(line);
        nice2 += is_nice2(line);
    }

    fmt::print("1: {}\n", nice1);
    fmt::print("2: {}\n", nice2);
}
