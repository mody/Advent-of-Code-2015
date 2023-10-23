#include <cstdio>
#define RAPIDJSON_HAS_STDSTRING 1

#include <fmt/core.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

struct Handler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, Handler> {
    bool Int(int i) { value += i; return true; }
    bool Uint(unsigned i) { value += i; return true; }
    bool Int64(int64_t i) { value += i; return true; }
    bool Uint64(uint64_t i) { value += i; return true; }
    bool Double(double d) { value += d; return true; }

    int64_t value = 0;
};

void part1(const std::string& json)
{
    Handler handler;
    rapidjson::Reader reader;
    rapidjson::StringStream ss(json.c_str());
    reader.Parse(ss, handler);

    fmt::print("1: {}\n", handler.value);
}

void process(rapidjson::Document::ValueType const& node, int64_t& sum)
{
    if (node.GetType() == rapidjson::kNumberType) {
        if (node.IsDouble())
            sum += node.GetDouble();
        else if (node.IsInt())
            sum += node.GetInt();
        else if (node.IsUint())
            sum += node.GetUint();
        else if (node.IsInt64())
            sum += node.GetInt64();
        else
            sum += node.GetUint64();
        return;
    }

    if (node.IsObject()) {
        for (auto it = node.MemberBegin(); it != node.MemberEnd(); ++it) {
            auto const& v = it->value;
            if (v.IsString() && std::string_view(v.GetString(), v.GetStringLength()) == "red") {
                return;
            }
        }
    }

    if (node.IsObject()) {
        for (auto it = node.MemberBegin(); it != node.MemberEnd(); ++it) {
            process(it->value, sum);
        }
    }

    if (node.IsArray()) {
        for (auto it = node.Begin(); it != node.End(); ++it) {
            process(*it, sum);
        }
    }
}

void part2(const std::string& json)
{
    rapidjson::Document doc;
    doc.Parse(json);

    int64_t sum = 0;
    process(doc, sum);

    fmt::print("2: {}\n", sum);
}

int main()
{
    std::string json;
    std::getline(std::cin, json);

    part1(json);
    part2(json);
}
