// addresses.hpp
#include <string>
#include <vector>
using std::string;
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
        int get_delivery_deadline() const;
        void set_delivery_deadline(int new_delivery);
        double euclidean_dist(const Address &other) const;
        double manhattan_dist(const Address &other) const;
        bool operator==(const Address &other) const;
        bool operator!=(const Address &other) const;
        string as_string() const;
};

class AddressList {
    protected:
        vector<Address> addrs;
    public:
        AddressList();
        virtual void add_address(Address addr);
        const Address &get_address_at(int index) const;
        const Address &get_final_addr() const;
        bool empty() const;
        int size() const;
        double euc_length() const;
        double man_length() const;
        int euc_index_closest_to(Address addr) const;
        int man_index_closest_to(Address addr) const;
        AddressList greedy_route(bool man_norm) const;
        string as_string() const;
};

class Route : public AddressList {
    public:
        Route(int depot_delivery_date);
        Route(Address startDepot, Address endDepot);
        virtual void add_address(Address addr) override;
        void add_unique_address(Address addr);
        const Address &get_final_nondepot() const;
        Route greedy_route(bool man_norm) const;
};

#endif
