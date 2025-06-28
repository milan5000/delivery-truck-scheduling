// addresses.cpp
#include <cmath>
#include <vector>
#include "../include/addresses.hpp"

using std::abs;
using std::pow;
using std::sqrt;
using std::vector;

// Address class

Address::Address
    (double x_in, double y_in, int delivery_in)
    : x(x_in), y(y_in), delivery_deadline(delivery_in) {
};
double Address::get_x() const {
    return x;
}
double Address::get_y() const {
    return y;
}
double Address::euclidean_dist(const Address &other) const {
    return sqrt(pow(x - other.get_x(), 2) + pow(y - other.get_y(), 2));
}
double Address::manhattan_dist(const Address &other) const {
    return (abs(x - other.get_x()) + abs(y - other.get_y()));
}

// AddressList class

AddressList::AddressList() { };

void AddressList::add_address(Address addr) {
    addrs.push_back(addr);
};

const Address &AddressList::get_address_at(int index) const {
    return addrs.at(index);
}

double AddressList::euc_length() const {
    double sum_len = 0.0;

    for (int i = 1; i < addrs.size(); i++) {
        sum_len += addrs.at(i).euclidean_dist(addrs.at(i-1));
    }

    return sum_len;
}

double AddressList::man_length() const {
    double sum_len = 0.0;

    for (int i = 1; i < addrs.size(); i++) {
        sum_len += addrs.at(i).manhattan_dist(addrs.at(i-1));
    }

    return sum_len;
}

int AddressList::euc_index_closest_to(Address addr) const {
    int best_ind = -1;
    double best_len = std::numeric_limits<double>::max();

    for (int i = 0; i < addrs.size(); i++) {
        double test_len = addr.euclidean_dist(addrs.at(i));
        if (test_len < best_len) {
            best_len = test_len;
            best_ind = i;
        }
    }

    return best_ind;
}

int AddressList::man_index_closest_to(Address addr) const {
    int best_ind = -1;
    double best_len = std::numeric_limits<double>::max();

    for (int i = 0; i < addrs.size(); i++) {
        double test_len = addr.manhattan_dist(addrs.at(i));
        if (test_len < best_len) {
            best_len = test_len;
            best_ind = i;
        }
    }

    return best_ind;
}
