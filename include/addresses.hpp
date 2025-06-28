// addresses.hpp
#include <vector>
using std::vector;

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

class AddressList {
    private:
        vector<Address> addrs;
    public:
        AddressList();
        void add_address(Address addr);
        const Address &get_address_at(int index) const;
        double euc_length() const;
        double man_length() const;
        int euc_index_closest_to(Address addr) const;
        int man_index_closest_to(Address addr) const;
};

#endif
