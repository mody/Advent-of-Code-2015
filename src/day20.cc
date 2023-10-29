#include <fmt/core.h>

#include <oneapi/tbb/blocked_range.h>
#include <oneapi/tbb/parallel_for.h>

#include <atomic>
#include <limits>

constexpr unsigned compute(const unsigned n)
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

int main()
{
    std::atomic<unsigned> result1 = std::numeric_limits<unsigned>::max();

    oneapi::tbb::parallel_for(
        oneapi::tbb::blocked_range<unsigned>(1, 33100000 / 10),
        [&result1](oneapi::tbb::blocked_range<unsigned> const& r) {
            for (unsigned i = r.begin(); i < r.end(); ++i) {
                if (compute(i) >= 33100000) {
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
    return 0;
}
