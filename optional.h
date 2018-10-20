//
// Created by BrilZliaN on 20.10.2018.
//

#pragma once

#include <type_traits>
#include <stdexcept>

template<typename T>
struct optional {
    optional() : nonEmpty(false){};

    ~optional() {
        clear();
    }

    optional(T const &other) : nonEmpty(true) {
        ::new ((void *)::std::addressof(object)) T(other);
    }

    optional(optional const &other) {
        if (other.nonEmpty) {
            nonEmpty = true;
            ::new ((void *)::std::addressof(object)) T(*other);
        } else {
            nonEmpty = false;
        }
    };

    optional &operator=(optional const &other) {
        optional copy = other;
        swap(copy);
        return *this;
    }

    void clear() {
        if (nonEmpty) {
            get()->~T();
            nonEmpty = false;
        }
    }

    explicit operator bool() const {
        return nonEmpty;
    }

    T &operator*() {
        throwIfEmpty();
        return *get();
    }

    T const &operator*() const {
        throwIfEmpty();
        return *get();
    }

    T *operator->() {
        throwIfEmpty();
        return get();
    }

    T const *operator->() const {
        throwIfEmpty();
        return get();
    }

    void swap(optional &other) {
        if (other.nonEmpty) {
            if (nonEmpty) {
                T &first = *reinterpret_cast<T *>(::std::addressof(object));
                T &second = *reinterpret_cast<T *>(::std::addressof(other.object));
                std::swap(first, second);
                std::swap(nonEmpty, other.nonEmpty);
            } else {
                ::new ((void *)::std::addressof(object)) T(*other);
                nonEmpty = true;
                other.clear();
            }
        } else if (nonEmpty) {
            ::new ((void *)::std::addressof(other.object)) T(*(*this));
            other.nonEmpty = true;
            clear();
        }
    }

private:
    bool nonEmpty = false;
    typename std::aligned_storage<sizeof(T), alignof(T)>::type object;

    T *get() {
        return reinterpret_cast<T *>(&object);
    }

    T const *get() const {
        return reinterpret_cast<T const *>(&object);
    }

    void throwIfEmpty() const {
        if (!nonEmpty)
            throw std::runtime_error("Optional is empty");
    }

    friend bool operator<(optional<T> const &first, optional<T> const &second) {
        if (first.nonEmpty ^ second.nonEmpty) {
            return second.nonEmpty;
        } else if (first.nonEmpty && second.nonEmpty) {
            return *first < *second;
        }
        return false;
    }

    friend bool operator<=(optional<T> const &first, optional<T> const &second) {
        return (first < second) || (first == second);
    }

    friend bool operator==(optional<T> const &first, optional<T> const &second) {
        if (first.nonEmpty && second.nonEmpty) {
            return *first == *second;
        }
        return first.nonEmpty == second.nonEmpty;
    }

    friend bool operator!=(optional<T> const &first, optional<T> const &second) {
        return !(first == second);
    }

    friend bool operator>(optional<T> const &first, optional<T> const &second) {
        return !(first <= second);
    }

    friend bool operator>=(optional<T> const &first, optional<T> const &second) {
        return !(first < second);
    }
};

template<typename T>
void swap(optional<T> &first, optional<T> &second) {
    first.swap(second);
}