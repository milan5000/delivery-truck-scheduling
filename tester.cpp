// tester.cpp
// Contains all test code.
// Yes there's probably a better way to do TDD...
#include <iostream>
#include "addresses.hpp"

using std::cout;
using std::endl;

// test functions return true on success

// Tests: addresses.cpp addresses.hpp

bool test_addresses() {
    Address origin(0.0, 0.0, 5);
    Address triangle(3.0, 4.0, 5);
    Address other_triangle(-3.0, -4.0, 5);

    if (triangle.get_x() != 3.0 ||
        triangle.get_y() != 4.0 ||
        origin.euclidean_dist(triangle) != 5.0 ||
        origin.manhattan_dist(triangle) != 7.0 ||
        triangle.manhattan_dist(other_triangle) != 14.0 ||
        other_triangle.euclidean_dist(triangle) != 10.0 ||
        triangle.euclidean_dist(origin) != 5.0) {
        return false;
    }

    return true;
}

// Results

int main() {
    int total = 0;
    int total_pass = 0;

    cout << "Test Results:\n\n";

    cout << "Addresses: ";
    if (test_addresses()) {
        cout << "success\n";
        total_pass++;
    } else {
        cout << "failure\n";
    }
    total++;

    cout << "\nFinal Results: " << total_pass << " passed (out of " <<
        total << ")" << endl;
}
