#include <fmt/core.h>

#include <oneapi/tbb/blocked_range.h>
#include <oneapi/tbb/parallel_for.h>

#include <atomic>
#include <limits>
#include <range/v3/view/enumerate.hpp>
#include <vector>

// #define OPTIMIZE_FOR_MEMORY

static constexpr unsigned TARGET = 33100000;

constexpr unsigned compute1(const unsigned n)
{
    if (n == 1) {
        return 10;
    }

    unsigned sum = 10;

    if ((n & 0x1) == 0) {
        sum += 20;
    }

    for (unsigned i = 3; i <= n; ++i) {
        if ((n % i) == 0) {
            sum += 10 * i;
        }
    }

    return sum;
}

constexpr unsigned compute2(const unsigned n)
{
    unsigned sum = 0;

    for (int i = 1; i <= (int)n; ++i) {
        if ((n % i) == 0 && (n / i) <= 50) {
            sum += 11 * i;
        }
    }

    return sum;
}

void low_memory_solution()
{
    std::atomic<unsigned> result1 = std::numeric_limits<unsigned>::max();

    oneapi::tbb::parallel_for(
        oneapi::tbb::blocked_range<unsigned>(1, TARGET / 10),
        [&result1](oneapi::tbb::blocked_range<unsigned> const& r) {
            for (unsigned i = r.begin(); i < r.end(); ++i) {
                if (compute1(i) >= TARGET) {
                    bool ok = true;
                    do {
                        unsigned old = result1.load();
                        if (old > i) {
                            ok = result1.compare_exchange_strong(old, i);
                        }
                    } while (!ok);
                    break;
                }
            }
        });

    fmt::print("1: {}\n", result1.load());

    std::atomic<unsigned> result2 = std::numeric_limits<unsigned>::max();

    oneapi::tbb::parallel_for(
        oneapi::tbb::blocked_range<unsigned>(1, TARGET / 10),
        [&result2](oneapi::tbb::blocked_range<unsigned> const& r) {
            for (unsigned i = r.begin(); i < r.end(); ++i) {
                if (compute2(i) >= TARGET) {
                    bool ok = true;
                    do {
                        unsigned old = result2.load();
                        if (old > i) {
                            ok = result2.compare_exchange_strong(old, i);
                        }
                    } while (!ok);
                }
            }
        });

    fmt::print("2: {}\n", result2.load());
}

void speed_optimized_solution()
{
    static constexpr unsigned SIZE = TARGET / 10;

    {
        std::vector<unsigned> data(SIZE, 10);

        for (unsigned step = 2; step < SIZE; ++step) {
            for (unsigned idx = step; idx < SIZE; idx += step) {
                data.at(idx) += 10 * step;
            }
        }

        for (auto const& [idx, val] : ranges::views::enumerate(data)) {
            if (val >= TARGET) {
                fmt::print("1: {}\n", idx);
                break;
            }
        }
    }

    {
        std::vector<unsigned> data(SIZE, 0);

        for (unsigned step = 1; step < SIZE; ++step) {
            for (unsigned idx = step, count = 0; idx < SIZE && count < 50; idx += step, ++count) {
                data.at(idx) += 11 * step;
            }
        }

        for (auto const& [idx, val] : ranges::views::enumerate(data)) {
            if (val >= TARGET) {
                fmt::print("2: {}\n", idx);
                break;
            }
        }
    }
}


int main()
{
#ifdef OPTIMIZE_FOR_MEMORY
    low_memory_solution();
#else
    speed_optimized_solution();
#endif
    return 0;
}
