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
    std::string line = "1113122113";
    for (unsigned i = 0; i < 40; ++i) {
        line = look_and_say(line);
    }
    fmt::print("1: {}\n", line.size());
}
