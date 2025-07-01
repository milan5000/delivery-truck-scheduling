// tester.cpp
// Contains all test code.
// Yes there's probably a better way to do TDD...
#include <iostream>
#include "include/addresses.hpp"

using std::cout;
using std::endl;

// test functions return true on success

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

    AddressList myList;

    if (myList.euc_length() != 0.0 ||
        myList.man_length() != 0.0 ||
        myList.euc_index_closest_to(triangle) != -1 ||
        myList.man_index_closest_to(triangle) != -1) {
        return false;
    }

    myList.add_address(triangle);
    myList.add_address(other_triangle);
    myList.add_address(origin);

    Address other(5.0, 0.0, 5);

    if (myList.euc_length() != 15.0 ||
        myList.man_length() != 21.0 ||
        myList.euc_index_closest_to(other) != 0 ||
        myList.man_index_closest_to(other) != 2 ||
        myList.get_address_at(1).get_y() != other_triangle.get_y()) {
        return false;
    }

    Route myRoute(triangle, origin);
    myRoute.add_unique_address(triangle);
    myRoute.add_unique_address(other_triangle);

    if (myRoute.get_address_at(1) != other_triangle) {
        return false;
    }

    return true;
}

bool test_greedy_route() {
    Route deliveries(0);

    deliveries.add_address(Address(0, 5, 0));
    deliveries.add_address(Address(5, 0, 0));
    deliveries.add_address(Address(5, 5, 0));
    
    //std::cerr << "Travel in order: " << deliveries.euc_length() << "\n";

    if (deliveries.size() != 5) return false;

    Route route = deliveries.greedy_route(false);

    if (route.size() != 5) return false;

    double len = route.euc_length();

    // std::cerr << "Square route: " << route.as_string() << "\nhas length " << len << "\n";

    if (len != 20) return false;

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

    cout << "Greedy Routing: ";
    if (test_greedy_route()) {
        cout << "success\n";
        total_pass++;
    } else {
        cout << "failure\n";
    }
    total++;

    cout << "\nFinal Results: " << total_pass << " passed (out of " <<
        total << ")" << endl;
}
