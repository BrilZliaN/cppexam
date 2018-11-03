#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <string>
#include <exception>
#include "sequence.h"

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

TEST_CASE("constructor") {
    SUBCASE("empty") {
        char_sequence s;
        char_sequence_ci s2;
        char_sequence_ci_assign s3;
    }

    SUBCASE("from sequence") {
        char_sequence_ci_assign s("AaB", 3);
        CHECK(s.size() == 3);
        CHECK(s == char_sequence_ci_assign("AaB", 3));
    }

    SUBCASE("copy constructor") {
        char_sequence_ci_assign s("AaB", 3);
        char_sequence_ci_assign s_copy(s);
        CHECK(s_copy.size() == 3);
        CHECK(s == s_copy);
    }
}

TEST_CASE("iterators") {
    SUBCASE("begin/end") {
        char_sequence s("a", 1);
        CHECK(std::distance(std::begin(s), std::end(s)) == 1);
        CHECK(*s.begin() == 'a');

        char_sequence s2;
        CHECK(s2.begin() == s2.end());
    }

    SUBCASE("rbegin/rend") {
        char_sequence s("ab", 2);
        CHECK(*std::prev(s.rend()) == 'a');
        CHECK(*s.rbegin() == 'b');
    }
}

TEST_CASE("element access") {
    SUBCASE("operator[]") {
        char_sequence_ci_assign s("ABCDI", 5);
        CHECK(s[0] == 'a');
        CHECK(s[4] == 'i');
    }

    SUBCASE("at") {
        char_sequence_ci_assign s("ABCDI", 5);
        CHECK(s.at(0) == 'a');
        CHECK(s.at(4) == 'i');
        CHECK_THROWS_AS(s.at(5), std::out_of_range);
    }

    SUBCASE("front/back") {
        char_sequence_ci s("ABCDI", 5);
        CHECK(s.front() == 'A');
        CHECK(s.back() == 'I');
    }
}

TEST_CASE("capacity") {
    SUBCASE("empty") {
        char_sequence_ci_assign s("ABCDI", 5);
        CHECK(!s.empty());

        char_sequence_ci_assign s2;
        CHECK(s2.empty());
    }

    SUBCASE("size") {
        char_sequence_ci_assign s("ABCDI", 5);
        CHECK(s.size() == 5);

        char_sequence_ci_assign s2("", 0);
        CHECK(s2.size() == 0);
    }
}

TEST_CASE("modifiers") {
//    SUBCASE("remove_prefix") {
//        char_sequence_ci_assign s("ABCDI", 5);
//        s.remove_prefix(2);
//        CHECK(s == char_sequence_ci_assign("CDI", 3));
//
//        s.remove_prefix(3);
//        CHECK(s.empty());
//    }
//
//    SUBCASE("remove_suffix") {
//        char_sequence_ci_assign s("ABCDI", 5);
//        s.remove_suffix(2);
//        CHECK(s == char_sequence_ci_assign("ABC", 3));
//
//        s.remove_suffix(3);
//        CHECK(s.empty());
//    }

    SUBCASE("swap") {
        char_sequence_ci_assign a("AAAAAAAAA", 9);
        char_sequence_ci_assign b("B", 1);
        char_sequence_ci_assign a_copy(a);
        char_sequence_ci_assign b_copy(b);

        CHECK_NOTHROW(a_copy.swap(b_copy));
        CHECK(a_copy == b);
        CHECK(b_copy == a);

        CHECK_NOTHROW(a_copy.swap(b_copy));
        CHECK(a_copy == a);
        CHECK(b_copy == b);
    }
}

TEST_CASE("operations") {
    SUBCASE("copy") {
        char_sequence_ci_assign a("AbAb");
        std::string s(4, '0');
        a.copy(&s[0], 4);

        CHECK(s == "abab");
    }

    SUBCASE("compare_char_seq") {
        char_sequence cs1("aba");
        char_sequence cs1_u("AbA");
        char_sequence cs2("abc");

        CHECK(cs1.compare(cs1_u) != 0);
        CHECK(cs1.compare(cs1) == 0);
        CHECK(cs1.compare(cs2) < 0);
        CHECK(cs2.compare(cs1) > 0);
    }

    SUBCASE("compare_char_seq_ci") {
        char_sequence_ci cs1("aba");
        char_sequence_ci cs1_u("AbA");

        CHECK(cs1_u.front() == 'A');
        CHECK(cs1.front() == 'a');
        CHECK(cs1.compare(cs1_u) == 0);
    }

    SUBCASE("subseq") {

    }
}
