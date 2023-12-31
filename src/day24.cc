#include <fmt/core.h>

#include <range/v3/action/sort.hpp>
#include <range/v3/numeric/accumulate.hpp>

#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <string>

using Data = std::deque<unsigned>;
using CallBack = std::function<void(Data const&)>;

void iterate_level(unsigned level, Data input, Data output, CallBack cb)
{
    if (level == 0) {
        cb(output);
        return;
    }

    Data tmp1 = input;
    while(tmp1.size() >= level) {
        auto val = tmp1.front();
        tmp1.pop_front();

        Data tmp2 = output;
        tmp2.push_back(val);

        iterate_level(level - 1, tmp1, tmp2, cb);
    }
}


uint64_t process(unsigned groups, Data const& input)
{
    const unsigned total = ranges::accumulate(input, 0u);
    assert((total % groups) == 0);
    const unsigned group_value = total / groups;
    const unsigned group_size = input.size() / groups;

    Data data = input;
    ranges::sort(data, std::greater<unsigned>());

    unsigned best_size1 = input.size();
    uint64_t best_qe = std::numeric_limits<uint64_t>::max();

    for (unsigned level = 2; level <= group_size && level <= best_size1; ++level) {
        iterate_level(level, data, {}, [&](Data const& d) {
            if (ranges::accumulate(d, 0u) == group_value) {
                if (d.size() <= best_size1) {
                    uint64_t my_qe = ranges::accumulate(d, 1ull, [](uint64_t a, uint64_t b) { return a * b; });
                    best_size1 = d.size();
                    best_qe = std::min(best_qe, my_qe);
                }
            }
        });
    }

    return best_qe;
}

int main()
{
    Data data;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        data.push_back(std::stoi(line));
    }

    fmt::print("1: {}\n", process(3, data));
    fmt::print("2: {}\n", process(4, data));
}
