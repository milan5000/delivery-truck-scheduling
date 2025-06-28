// addresses.hpp
#ifndef ADDRESSES_HPP
#define ADDRESSES_HPP

class Address {
    private:
        double x, y;
        int delivery_deadline;
    public:
        Address(double x_in, double y_in, int delivery_in);
        double get_x() const;
        double get_y() const;
        double euclidean_dist(const Address &other) const;
        double manhattan_dist(const Address &other) const;
};

#endif