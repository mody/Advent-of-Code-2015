#include <fmt/core.h>

#include <cassert>
#include <iostream>
#include <sstream>

std::string descape(std::string data)
{
    unsigned in = 0, out = 0;
    for (;in < data.size();) {
        if (data.at(in) != '\\') {
            data.at(out) = data.at(in);
        } else {
            ++in;
            if (data.at(in) == 'x') {
                ++in;
                data.at(out) = std::stoul(data.substr(in, 2), nullptr, 16);
                ++in;
            } else {
                data.at(out) = data.at(in);
            }
        }
        ++in;
        ++out;
    }
    return data.substr(0, out);
}

std::string escape(const std::string& input)
{
    std::stringstream ss;

    for (auto const& c : input) {
        switch(c) {
        case '\\':
        case '\"':
        case '\'':
            ss << "\\";
        default:
            ss << c;
        }
    }

    return ss.str();
}

int main()
{
    unsigned total_size = 0, clean_size = 0, escaped_size = 0;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;
        total_size += line.size();
        clean_size += descape(line).size() - 2;
        escaped_size += escape(line).size() + 2;
    }

    fmt::print("1: {}\n", total_size - clean_size);
    fmt::print("2: {}\n", escaped_size - total_size);
}
