#include <fmt/core.h>

#include <cassert>
#include <iostream>

int main()
{
    std::string line;
    std::getline(std::cin, line);

    int val = 0;
    for (auto const& c : line) {
        if (c == '(') {
            ++val;
        } else if (c == ')') {
            --val;
        } else {
            assert(false);
        }
    }

    fmt::print("1: {}\n", val);

    return 0;
}
