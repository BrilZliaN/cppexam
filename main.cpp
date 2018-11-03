#include "sequence.h"
#include <iostream>
#include <cstring>
#include <exception>
#include <algorithm>

struct case_insensitive_char_traits : public std::char_traits<char> {
    static char to_upper(char ch) { return std::toupper((unsigned char) ch); }

    static bool eq(char c1, char c2) { return to_upper(c1) == to_upper(c2); }

    static bool lt(char c1, char c2) { return to_upper(c1) < to_upper(c2); }

    static int compare(const char *s1, const char *s2, size_t n) {
        while (n-- != 0) {
            if (to_upper(*s1) < to_upper(*s2)) return -1;
            if (to_upper(*s1) > to_upper(*s2)) return 1;
            ++s1;
            ++s2;
        }
        return 0;
    }

    static const char *find(const char *s, int n, char a) {
        auto const ua(to_upper(a));
        while (n-- != 0) {
            if (to_upper(*s) == ua)
                return s;
            s++;
        }
        return nullptr;
    }
};

struct case_insensitive_assign_char_traits : public std::char_traits<char> {
    static void assign(char &r, const char &a) { r = std::tolower(a); }

    static char *assign(char *p, std::size_t count, char a) { return std::fill_n(p, count, std::tolower(a)); }

    static char_type* copy(char_type* dest, const char_type* src, std::size_t count) {
        return std::transform(src, src + count, dest, [](char c) { return std::tolower(c); });
    }
};

template<class T, class Traits>
using sequence = sequence<T, Traits>;
using char_sequence = sequence<char, std::char_traits<char>>;
using char_sequence_ci = sequence<char, case_insensitive_char_traits>;
using char_sequence_ci_assign = sequence<char, case_insensitive_assign_char_traits>;

int main() {
    char_sequence_ci_assign a("AbAb");
    std::string s(4, '0');
    a.copy(&s[0], 4);

    bool ans = s == "abab";

    std::cout << ans << std::endl;
}