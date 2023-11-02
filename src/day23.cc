#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <fmt/core.h>

#include <array>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

enum class Register
{
    A = 0,
    B,
};

enum class Op
{
    NOP = 0,
    HLF,
    TPL,
    INC,
    JMP,
    JIE,
    JIO,
};

using Argument = std::variant<bool, Register, int32_t>;

struct Instruction {
    Op op = Op::NOP;
    Argument arg1;
    Argument arg2;
};

using Program = std::vector<Instruction>;

struct CPU
{
    CPU(Program const& p) noexcept
        : program {p}
    { }

    void run() noexcept;

    Program const& program;
    std::array<uint64_t, sizeof(Register)> registers = {0};
    std::string output;
};


void CPU::run() noexcept
{
    output.clear();

    for (auto pc = program.begin(); pc != program.end();)
    {
        Instruction const& i = *pc;

        switch(i.op) {
        case Op::HLF:
            registers.at(static_cast<unsigned>(std::get<Register>(i.arg1))) >>= 1;
            ++pc;
            break;
        case Op::TPL:
            registers.at(static_cast<unsigned>(std::get<Register>(i.arg1))) *= 3;
            ++pc;
            break;
        case Op::INC:
            registers.at(static_cast<unsigned>(std::get<Register>(i.arg1))) += 1;
            ++pc;
            break;
        case Op::JMP:
            std::advance(pc, std::get<int32_t>(i.arg1));
            break;
        case Op::JIE:
            if ((registers.at(static_cast<unsigned>(std::get<Register>(i.arg1))) & 0x1) == 0) {
                std::advance(pc, std::get<int32_t>(i.arg2));
            } else {
                ++pc;
            }
            break;
        case Op::JIO:
            if (registers.at(static_cast<unsigned>(std::get<Register>(i.arg1))) == 1) {
                std::advance(pc, std::get<int32_t>(i.arg2));
            } else {
                ++pc;
            }
            break;
        default:
            assert(false);
        }
    }
}


int main()
{
    Program prog;

    auto str2reg = [](std::string_view const s) -> Register {
        const unsigned char c = s.at(0);
        switch (c) {
        case 'a': return Register::A;
        case 'b': return Register::B;
        default: assert(false);
        }
    };

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) break;

        static const std::map<std::string, Op> str2op = {
            {"hlf",    Op::HLF},
            {"tpl",    Op::TPL},
            {"inc",    Op::INC},
            {"jmp",    Op::JMP},
            {"jie",    Op::JIE},
            {"jio",    Op::JIO},
        };

        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(" "));
        assert(parts.size() > 1);

        Instruction i = {};
        i.op = str2op.at(parts.at(0));
        switch(i.op) {
        case Op::INC:
        case Op::HLF:
        case Op::TPL:
            assert(parts.size() == 2);
            i.arg1 = str2reg(parts.at(1));
            break;
        case Op::JMP:
            assert(parts.size() == 2);
            i.arg1 = std::stoi(parts.at(1));
            break;
        case Op::JIE:
        case Op::JIO:
            assert(parts.size() == 3);
            i.arg1 = str2reg(parts.at(1));
            i.arg2 = std::stoi(parts.at(2));
            break;
        default: assert(false);
        }
        prog.push_back(std::move(i));
    }

    CPU cpu1(prog);
    cpu1.registers = {0};
    cpu1.run();
    fmt::print("1: {}\n", cpu1.registers.at(static_cast<unsigned>(Register::B)));

    CPU cpu2(prog);
    cpu2.registers = {1, 0};
    cpu2.run();
    fmt::print("2: {}\n", cpu2.registers.at(static_cast<unsigned>(Register::B)));

    return 0;
}
