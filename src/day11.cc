#include <fmt/core.h>
#include <range/v3/view/drop.hpp>

#include <algorithm>

struct Password
{
    Password(std::string initial) noexcept : value{std::move(initial)} {}

    Password(Password&&) = default;
    Password(Password const&) = default;

    Password& operator=(Password&&) = default;
    Password& operator=(Password const&) = default;

    Password& operator++() {
        inc(1);
        return *this;
    }

    Password& operator+=(unsigned i) {
        inc(i);
        return *this;
    }

    bool operator==(Password const& oth) const noexcept { return value == oth.value; }

    std::string operator*() const noexcept { return value; }

    bool valid() const noexcept {
        return value.find_first_of("iol") == std::string::npos && has_3_inc() && has_2_doubles();
    }

private:
    bool has_3_inc() const noexcept;
    bool has_2_doubles() const noexcept;
    void inc(unsigned i) noexcept;

    std::string value;
};

void Password::inc(unsigned i) noexcept
{
    std::reverse(value.begin(), value.end());

    for (; i > 0; --i) {
        bool over = false;
        for (auto& c : value) {
            ++c;
            if (c == 'i' || c == 'o' || c == 'l') {
                ++c;
            }
            if ((over = (c > 'z'))) {
                c = 'a';
            } else {
                break;
            }
        }
        if (over) {
            value += 'a';
        }
    }

    std::reverse(value.begin(), value.end());
}

bool Password::has_3_inc() const noexcept
{
    unsigned count = 1;

    unsigned char a = value.front();
    for (unsigned char b : value | ranges::views::drop(1)) {
        if ((a + 1) == b) {
            ++count;
            if (count == 3) break;
        } else {
            count = 1;
        }
        a = b;
    }

    return count == 3;
}

bool Password::has_2_doubles() const noexcept
{
    unsigned count = 0;

    unsigned char a = value.front();
    for (unsigned char b : value | ranges::views::drop(1)) {
        if (a == b) {
            ++count;
            a = '\0';
        } else {
            a = b;
        }
    }
    return count > 1;
}

int main()
{
    Password pw("vzbxkghb");
    for (++pw; !pw.valid();++pw);
    fmt::print("1: {}\n", *pw);

    for (++pw; !pw.valid();++pw);
    fmt::print("2: {}\n", *pw);
}
