// addresses.cpp
#include <cmath>
#include "../include/addresses.hpp"

using std::abs;
using std::pow;
using std::sqrt;

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

