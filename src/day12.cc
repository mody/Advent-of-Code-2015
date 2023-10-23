#include <fmt/core.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/reader.h>

#include <cstdint>
#include <iostream>

struct Handler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, Handler> {
    bool Int(int i) { value += i; return true; }
    bool Uint(unsigned i) { value += i; return true; }
    bool Int64(int64_t i) { value += i; return true; }
    bool Uint64(uint64_t i) { value += i; return true; }
    bool Double(double d) { value += d; return true; }

    int64_t value = 0;
};

int main()
{
    Handler handler;
    rapidjson::Reader reader;
    rapidjson::IStreamWrapper ss(std::cin);
    reader.Parse(ss, handler);

    fmt::print("1: {}\n", handler.value);
}
