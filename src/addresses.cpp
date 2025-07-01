// addresses.cpp
#include <cmath>
#include <string>
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

int Address::get_delivery_deadline() const {
    return delivery_deadline;
}

void Address::set_delivery_deadline(int new_delivery) {
    delivery_deadline = new_delivery;
}

double Address::euclidean_dist(const Address &other) const {
    return sqrt(pow(x - other.get_x(), 2) + pow(y - other.get_y(), 2));
}

double Address::manhattan_dist(const Address &other) const {
    return (abs(x - other.get_x()) + abs(y - other.get_y()));
}

bool Address::operator==(const Address &other) const {
    return (x == other.get_x() && y == other.get_y());
}

bool Address::operator!=(const Address &other) const {
    return (x != other.get_x() || y != other.get_y());
}

string Address::as_string() const {
    return ("(" + std::to_string(x) + "," + std::to_string(y) + ")");
}

// AddressList class

AddressList::AddressList() { };

void AddressList::add_address(Address addr) {
    addrs.push_back(addr);
};

const Address &AddressList::get_address_at(int index) const {
    return addrs.at(index);
}

const Address &AddressList::get_final_addr() const {
    return addrs.back();
}

bool AddressList::empty() const {
    return addrs.empty();
}

int AddressList::size() const {
    return addrs.size();
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

AddressList AddressList::greedy_route(bool man_norm) const {
    // construct route starting with first address in list,
    // using locally closest address at each step
    // time complexity O(n^2)
    AddressList path;
    
    if (addrs.size() <= 2) {
        for (Address elem : addrs) {
            path.add_address(elem);
        }
        return path;
    }

    path.add_address(addrs.at(0));

    // mark address 0 as "already included"
    vector<bool> includedAddrs(addrs.size());
    includedAddrs[0] = true;
    int addrsRem = addrs.size() - 1;

    // why do it this way? because alternatives are not very good
    // 1. modify original array - avoid
    // 2. make copy and modify - how would you modify?
    //   a. delete elements - way too much copying
    //   b. insert nullptr or something - ok but you still made a copy
    //   and now we have to switch to pointers vs objects
    // 3. (our choice) make bool vector showing unaccessed addresses
    // efficient and saves space compared to copying objects

    while (addrsRem > 0) {
        Address curr = path.get_final_addr();
        // find index of nearest address
        int nearestInd = -1;
        int nearestDist = std::numeric_limits<int>::max();
        
        for (int i = 0; i < addrs.size(); i++) {
            if (!includedAddrs[i]) {
                int dist;
                if (man_norm) {
                    dist = curr.manhattan_dist(addrs.at(i));
                } else {
                    dist = curr.euclidean_dist(addrs.at(i));
                }
                if (dist < nearestDist) {
                    nearestDist = dist;
                    nearestInd = i;
                }
            }
        }

        // add it to the path
        path.add_address(addrs.at(nearestInd));
        includedAddrs[nearestInd] = true;
        addrsRem--;
    }

    return path;
}

string AddressList::as_string() const {
    string str = "";
    
    for (Address addr : addrs) {
        str += addr.as_string() + " ";
    }

    return str;
}

// Route class
// inherits from AddressList, but with changes:
// - keeps 1st and last elements in place (symbolizes depots)
// - possible to update duplicate addresses' delivery dates
//   instead of adding them again

Route::Route(int depot_delivery_date) {
    addrs.push_back(Address(0, 0, depot_delivery_date));
    addrs.push_back(Address(0, 0, depot_delivery_date));
};

Route::Route(Address startDepot, Address endDepot)  {
    addrs.push_back(startDepot);
    addrs.push_back(endDepot);
};

void Route::add_address(Address addr) {
    if (addrs.size() < 2) {
        addrs.push_back(addr);
    } else {
        addrs.insert(addrs.end()-1, addr);
    }
}

void Route::add_unique_address(Address addr) {
    // check if address is already being delivered to
    vector<Address>::iterator it = std::find(addrs.begin(), addrs.end(), addr);

    if (it == addrs.end()) { // if not, add it
        if (addrs.size() < 2) {
            addrs.push_back(addr);
        } else {
            addrs.insert(addrs.end()-1, addr);
        }
    } else { // if so, update the delivery time if needed
        if (addrs.at(it - addrs.begin()).get_delivery_deadline() > addr.get_delivery_deadline()) {
            addrs.at(it - addrs.begin()).set_delivery_deadline(addr.get_delivery_deadline());
        }
    }
}

const Address &Route::get_final_nondepot() const {
    // if there are no nondepots present, just return final depot
    if (addrs.size() <= 2) {
        return addrs.back();
    } else {
        return addrs.at(addrs.size() - 2);
    }
}

Route Route::greedy_route(bool man_norm) const {
    // same as AddressList version, but preserve position of final depot
    // time complexity O(n^2)
    Route path(addrs.front(), addrs.back());

    vector<bool> includedAddrs(addrs.size());
    includedAddrs[0] = true;
    includedAddrs[addrs.size() - 1] = true;

    int addrsRem = addrs.size() - 2;

    while (addrsRem > 0) {
        Address curr = path.get_final_nondepot();
        // find index of nearest address
        int nearestInd = -1;
        int nearestDist = std::numeric_limits<int>::max();
        
        for (int i = 0; i < addrs.size(); i++) {
            if (!includedAddrs[i]) {
                int dist;
                if (man_norm) {
                    dist = curr.manhattan_dist(addrs.at(i));
                } else {
                    dist = curr.euclidean_dist(addrs.at(i));
                }
                if (dist < nearestDist) {
                    nearestDist = dist;
                    nearestInd = i;
                }
            }
        }

        // add it to the path
        path.add_address(addrs.at(nearestInd));
        includedAddrs[nearestInd] = true;
        addrsRem--;
    }

    return path;
}
