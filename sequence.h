//
// Created by BrilZliaN on 03.11.2018.
//

#pragma once

#include <cstddef>
#include <type_traits>
#include <cassert>
#include <stdexcept>

struct seq_traits {
    using type = char;

    static constexpr bool eq(type a, type b) noexcept;

    static constexpr bool lt(type a, type b) noexcept;

    static constexpr void assign(type &r, const type &a);

    static constexpr std::size_t length(const type *s);
};

template<class T, class Traits = seq_traits>
struct sequence {
    static_assert(std::is_same<T, typename Traits::char_type>::value, "");

    using size_type = std::size_t;
    using const_iterator = const T *;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    using const_reference = const T &;
    static constexpr size_type npos = size_type(-1);

    constexpr sequence() noexcept : len(0), data(nullptr) {
    }

    constexpr sequence(const sequence &other) noexcept : len(other.len) {
        data = new T[other.len];
        for (size_type i = 0; i < other.len; i++) {
            new (data + i) T(other.data[i]);
        }
    }

    constexpr sequence(const T *s, size_type count) : len(count) {
        data = new T[count];
        for (size_type i = 0; i < count; i++) {
            const T &obj = *(s + i);
            Traits::assign(*(data+i), obj);
        }
    }

    constexpr sequence(const T *s) {
        len = Traits::length(s);
        data = new T[len];
        for (size_type i = 0; i < len; i++) {
            const T &obj = *(s + i);
            Traits::assign(*(data+i), obj);
        }
    }

    constexpr sequence &operator=(const sequence &view) noexcept {
        sequence copy(view);
        swap(copy);
        return *this;
    }

    constexpr const_iterator begin() const noexcept {
        return data;
    }

    constexpr const_iterator end() const noexcept {
        return data + len;
    }

    constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    constexpr const_reference operator[](size_type pos) const {
        assert(pos >= 0 && pos < len);
        return at(pos);
    }

    constexpr const_reference at(size_type pos) const { //throws std::out_of_range
        if (pos >= 0 && pos < len) {
            return data[pos];
        } else {
            throw std::out_of_range("");
        }
    }

    constexpr const_reference front() const {
        return *begin();
    }

    constexpr const_reference back() const {
        return *(end() - 1);
    }

    constexpr size_type size() const noexcept {
        return len;
    }

    constexpr bool empty() const noexcept {
        return len == 0;
    }

    constexpr void remove_prefix(size_type n) {
        len -= n;
        data += n - 1;
    }

    constexpr void remove_suffix(size_type n) {
        len -= n;
    }

    constexpr void swap(sequence &v) noexcept {
        using std::swap;
        swap(v.len, len);
        swap(v.data, data);
    }

    //copies no more than count characters, return number of copyed elements
    size_type copy(T *dest, size_type count, size_type pos = 0) const {
        if (pos + count > len) {
            throw std::out_of_range("");
        }
        for (size_type i = 0; i < count; i++) {
            const T &obj = at(pos + i);
            Traits::assign(*(dest+i), obj);
        }
    } //throws std::out_of_range

    constexpr int compare(sequence v) const noexcept {
        size_type min = len < v.len ? len : v.len;
        size_type res = 0;
        for (size_t i = 0; i < min; i++) {
            T a = at(i);
            T b = v[i];
            if (!Traits::eq(a, b)) {
                res = Traits::lt(a,b) ? -1 : 1;
                break;
            }
        }
        return res;
    }

    constexpr sequence subseq(size_type pos = 0, size_type count = npos) const {
        if (pos + count > len || pos < 0) {
            throw std::out_of_range("");
        }
        sequence copy(*this);
        copy.remove_prefix(pos);
        if (count > 0) {
            copy.remove_suffix(copy.len - count);
        }
        return copy;
    } //throws std::out_of_range

    constexpr size_type find(sequence v, size_type pos = 0) const noexcept {
        if (pos < 0 || pos >= len || v.len > len - pos) {
            return npos;
        }
        for (; pos + v.size() < size(); ++pos) {
            if (subseq(pos, pos + v.size()) == v) {
                return pos;
            }
        }
        return npos;
    }

    constexpr size_type rfind(sequence v, size_type pos = npos) const noexcept {
        sequence rev(*this);
        rev.reverse();
        if (pos == -1)
            pos = len;
        return rev.find(v, len - pos);
    }

    template<class T2, class Traits2>
    friend constexpr bool operator==(sequence<T2, Traits2> lhs, sequence rhs) noexcept {
        return lhs.compare(rhs) == 0;
    }

    template<class T2, class Traits2>
    friend constexpr bool operator!=(sequence<T2, Traits2> lhs, sequence rhs) noexcept {
        return !(lhs == rhs);
    }

    template<class T2, class Traits2>
    friend constexpr bool operator<(sequence<T2, Traits2> lhs, sequence rhs) noexcept {
        return lhs.compare(rhs) < 0;
    }

    template<class T2, class Traits2>
    friend constexpr bool operator>(sequence<T2, Traits2> lhs, sequence rhs) noexcept {
        return lhs.compare(rhs) > 0;
    }

    template<class T2, class Traits2>
    friend constexpr bool operator<=(sequence<T2, Traits2> lhs, sequence rhs) noexcept {
        return lhs < rhs || lhs == rhs;
    }

    template<class T2, class Traits2>
    friend constexpr bool operator>=(sequence<T2, Traits2> lhs, sequence rhs) noexcept {
        return lhs > rhs || lhs == rhs;
    }

private:
    size_t len;
    T *data;

    void reverse() {
        T* a = begin();
        T* b = end();
        for (; a < b; a++, b--) {
            std::swap(a,b);
        }
    }
};

namespace std {
    template<class T, class Traits>
    struct hash<sequence<T, Traits>> {
        std::size_t operator()(const sequence<T, Traits> &s) const noexcept {
            size_t res = 0;
            for (T* begin = s.begin(); begin != s.end(); begin++) {
                size_t cur = std::hash<T>()(*begin);
                res = (res * 31 + cur) % 1234567890;
            }
        }
    };
}