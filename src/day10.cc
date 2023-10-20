#include <fmt/core.h>

#include <cassert>
#include <sstream>
#include <string>

std::string look_and_say(std::string const& input)
{
    assert(!input.empty());

    std::stringstream ss;

    for (unsigned pos = 0; pos < input.size();) {
        unsigned cnt = 1;
        unsigned char c = input.at(pos++);
        while(pos < input.size() && input.at(pos) == c) {
            ++cnt;
            ++pos;
        }
        ss << cnt << c;
    }
    return ss.str();
}

int main()
{
    unsigned size1 = 0, size2 = 0;
    std::string line = "1113122113";
    for (unsigned i = 0; i < 50; ++i) {
        line = look_and_say(line);
        if (i == 39) {
            size1 = line.size();
        }
    }
    size2 = line.size();

    fmt::print("1: {}\n", size1);
    fmt::print("2: {}\n", size2);
}
