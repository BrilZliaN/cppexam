#include <iostream>
#include "optional.h"

using std::cout;
using std::endl;

struct copyable {
    int a;
    int b;

    copyable(int x, int y) {
        a = x;
        b = y;
    }

    copyable() = delete;
};

template <typename T>
bool isEmpty(optional<T> const& o) {
    if (o) {
        return true;
    }
    return false;
}

template <typename T>
bool testEmpty(optional<T> const& o) {
    bool empty = isEmpty(o);
    cout << "isEmpty: " << empty << endl;
    return empty;
}

int main() {
    optional<copyable> empty;
    optional<copyable> o(copyable(1, 2));
    optional<int> a(3);
    optional<int> b(4);
    optional<int> c;
    cout << "Empty\n";
    testEmpty(empty);
    cout << "Copyable\n";
    testEmpty(o);
    cout << "Contents: " << o->a << " " << o->b << endl;
    cout << "a\n";
    testEmpty(a);
    cout << "Contents: " << *a << endl;
    ++(*a);
    cout << "Contents (+1): " << *a << endl;
    cout << "b\n";
    testEmpty(b);
    cout << "Contents: " << *b << endl;
    --(*b);
    cout << "Contents: (-1) " << *b << endl;
    cout << "c\n";
    testEmpty(c);
    swap(a, b);
    cout << "Swapped a and b " << *a << " " << *b << endl;
    cout << "empty and o " << isEmpty(empty) << " " << isEmpty(o) << endl;
    swap(empty, o);
    cout << "Swapped empty and o " << isEmpty(empty) << " " << isEmpty(o) << endl;
    swap(empty, o);
    cout << "Swapped (2) empty and o " << isEmpty(empty) << " " << isEmpty(o) << endl;
//    cout << "empty < o" << (empty < o) << endl; //compile error
//    cout << "empty == o" << (empty == o) << endl; //compile error
    cout << "a < b " << (a < b) << " (should be) " << (*a < *b) << endl;
    cout << "a == b " << (a == b) << " (should be) " << (*a == *b) << endl;
    cout << "a != b " << (a == b) << " (should be) " << (*a == *b) << endl;
    cout << "a > b " << (a > b) << " (should be) " << (*a > *b) << endl;
    cout << "c < b " << (c < b) << " (should be) 1"<< endl;
    cout << "";

    return 0;
}

