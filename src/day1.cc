#include <fmt/core.h>
#include <range/v3/view.hpp>

#include <cassert>
#include <iostream>

int main()
{
    std::string line;
    std::getline(std::cin, line);

    unsigned pos = 0;
    int val = 0;
    for (auto const [index, c] : ranges::views::enumerate(line)) {
        if (c == '(') {
            ++val;
        } else if (c == ')') {
            --val;
        } else {
            assert(false);
        }
        if (pos == 0 && val == -1) {
            pos = index + 1;
        }
    }

    fmt::print("1: {}\n", val);
    fmt::print("2: {}\n", pos);

    return 0;
}
