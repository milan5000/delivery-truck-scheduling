// addresses.cpp
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
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

/**
 * Appends a vector of addresses to the current AddressList,
 * preserving order.
 */
void AddressList::bulk_add_addresses(vector<Address> more_addrs) {
    addrs.insert(addrs.end(), more_addrs.begin(), more_addrs.end());
}

/**
 * Adds an address to the end of the list.
 */
void AddressList::add_address(Address addr) {
    addrs.push_back(addr);
};

/**
 * Returns a const ref to the address at the specified index.
 * Fails if the index is invalid.
 */
const Address &AddressList::get_address_at(int index) const {
    return addrs.at(index);
}

/**
 * Returns a const ref to the final address in the list.
 * Fails if the list is empty.
 */
const Address &AddressList::get_final_addr() const {
    return addrs.back();
}

bool AddressList::empty() const {
    return addrs.empty();
}

int AddressList::size() const {
    return addrs.size();
}

double AddressList::vectorial_length(vector<Address> path, bool man_norm) const {
    double sum_len = 0.0;

    if (man_norm) {
        for (int i = 1; i < path.size(); i++) {
            sum_len += path.at(i).manhattan_dist(path.at(i-1));
        }
    } else {
        for (int i = 1; i < path.size(); i++) {
            sum_len += path.at(i).euclidean_dist(path.at(i-1));
        }
    }

    return sum_len;
}

double AddressList::euc_length() const {
    return vectorial_length(addrs, false);
}

double AddressList::man_length() const {
    return vectorial_length(addrs, true);
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

/**
 * Returns a greedily constructed route, starting at the first
 * address in the AddressList. At each step in the route, the
 * next address is the nearest unvisited one. Distance is calculated
 * with the Manhattan norm if man_norm is true, or the Euclidean
 * norm otherwise. The route is returned as a new AddressList,
 * keeping the original constant.
 */
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

/**
 * Attempts to shorten a route with the 2-opt heuristic.
 * If the 2-opt heuristic successfully finds a shorter route,
 * returns that route as a new AddressList. Otherwise, returns
 * a copy of the original route. The original AddressList is
 * left constant. Distance is calculated with the Manhattan
 * norm if man_norm is true, or the Euclidean norm otherwise.
 */
AddressList AddressList::opt2_rearrange(bool man_norm) const {
    if (addrs.size() <= 2) {
        AddressList path;
        path.bulk_add_addresses(addrs);
        return path;
    }

    vector<Address> newAddrs = addrs;

    //double best_dist = vectorial_length(newAddrs, man_norm);
    bool changed;

    do {
        changed = false;
        for (int i = 0; i < addrs.size() - 1; i++) {
            for (int j = i + 1; j < addrs.size(); j++) {
                // test change that would be incurred in length by
                // swapping addresses i and j
                double old_len, new_len;
                if ((i != 0) && (j != addrs.size() - 1)) {
                    if (man_norm) {
                        old_len = newAddrs.at(i-1).manhattan_dist(newAddrs.at(i)) + newAddrs.at(j).manhattan_dist(newAddrs.at(j+1));
                        new_len = newAddrs.at(i-1).manhattan_dist(newAddrs.at(j)) + newAddrs.at(i).manhattan_dist(newAddrs.at(j+1));
                    } else {
                        old_len = newAddrs.at(i-1).euclidean_dist(newAddrs.at(i)) + newAddrs.at(j).euclidean_dist(newAddrs.at(j+1));
                        new_len = newAddrs.at(i-1).euclidean_dist(newAddrs.at(j)) + newAddrs.at(i).euclidean_dist(newAddrs.at(j+1));
                    }
                } else if ((i == 0) && (j == addrs.size() - 1)) {
                    // dummy values to skip this case
                    // this just reverses the entire list, there is no point
                    old_len = 0.0;
                    new_len = 1.0;
                } else if (i == 0) { // j != size - 1
                    if (man_norm) {
                        old_len = newAddrs.at(j).manhattan_dist(newAddrs.at(j+1));
                        new_len = newAddrs.at(i).manhattan_dist(newAddrs.at(j+1));
                    } else {
                        old_len = newAddrs.at(j).euclidean_dist(newAddrs.at(j+1));
                        new_len = newAddrs.at(i).euclidean_dist(newAddrs.at(j+1));
                    }
                } else { // j == size - 1 and i != 0
                    if (man_norm) {
                        old_len = newAddrs.at(i-1).manhattan_dist(newAddrs.at(i));
                        new_len = newAddrs.at(i-1).manhattan_dist(newAddrs.at(j));
                    } else {
                        old_len = newAddrs.at(i-1).euclidean_dist(newAddrs.at(i));
                        new_len = newAddrs.at(i-1).euclidean_dist(newAddrs.at(j));
                    }
                }
                if (old_len - new_len > 0) { // path shortens, so do the swap
                    std::reverse(newAddrs.begin()+i, newAddrs.begin()+j+1);
                    //best_dist -= (old_len - new_len);
                    changed = true;
                    goto quadbreak; // i'm so sorry
                }
            }
        }
        quadbreak:
        ; // prevent the compiler from whining (i'm really seriously sorry)
    } while (changed);

    AddressList path;
    path.bulk_add_addresses(newAddrs);
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

/**
 * Constructs a route using the address at the origin as the
 * start and end depot.
 */
Route::Route(int depot_delivery_date) {
    addrs.push_back(Address(0, 0, depot_delivery_date));
    addrs.push_back(Address(0, 0, depot_delivery_date));
};

Route::Route(Address startDepot, Address endDepot)  {
    addrs.push_back(startDepot);
    addrs.push_back(endDepot);
};

/**
 * Adds the address right before the end depot.
 * If there are less than 2 addresses, appends to the
 * end of the list instead.
 */
void Route::add_address(Address addr) {
    if (addrs.size() < 2) {
        addrs.push_back(addr);
    } else {
        addrs.insert(addrs.end()-1, addr);
    }
}

/**
 * Inserts a vector of addresses right before the end depot,
 * preserving order. If there are less than 2 addresses,
 * appends to the end of the list instead.
 */
void Route::bulk_add_addresses(vector<Address> more_addrs) {
    if (addrs.size() < 2) {
        addrs.insert(addrs.end(), more_addrs.begin(), more_addrs.end());
    } else {
        addrs.insert(addrs.end() - 1, more_addrs.begin(), more_addrs.end());
    }
}

/**
 * Checks if any address in the route is equal to addr.
 * If not, adds it in the manner of Route::add_address().
 * If so, for the first address in the route equal to addr,
 * sets the delivery time to that of addr, if it is equal.
 */
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

/**
 * Returns the address in the route right before the end depot.
 * If there are less than 2 addresses, returns the final one.
 * Fails if the route is empty.
 */
const Address &Route::get_final_nondepot() const {
    // if there are no nondepots present, just return final depot
    if (addrs.size() <= 2) {
        return addrs.back();
    } else {
        return addrs.at(addrs.size() - 2);
    }
}

/**
 * Follows the specification of AddressList::greedy_route(),
 * with the change that the final depot is kept as the final
 * address in the route.
 * 
 * Cf. AddressList::greedy_route():
 * 
 * > Returns a greedily constructed route, starting at the first
 * address in the AddressList. At each step in the route, the
 * next address is the nearest unvisited one. Distance is calculated
 * with the Manhattan norm if man_norm is true, or the Euclidean
 * norm otherwise. The route is returned as a new AddressList,
 * keeping the original constant.
 */
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

/**
 * Follows the specification of AddressList::opt2_rearrange(),
 * with the change that the final and initial depots maintain position.
 * If one or both of the depots are missing, returns a copy of the
 * original route instead.
 * 
 * Cf. AddressList::opt2_rearrange():
 * 
 * > Attempts to shorten a route with the 2-opt heuristic.
 * If the 2-opt heuristic successfully finds a shorter route,
 * returns that route as a new AddressList. Otherwise, returns
 * a copy of the original route. The original AddressList is
 * left constant. Distance is calculated with the Manhattan
 * norm if man_norm is true, or the Euclidean norm otherwise.
 */
Route Route::opt2_rearrange(bool man_norm) const {
    if (addrs.size() <= 3) {
        Route ret = *this;
        return ret;
    }

    vector<Address> newAddrs = addrs;

    bool changed;

    do {
        changed = false;
        for (int i = 1; i < addrs.size() - 2; i++) {
            for (int j = i + 1; j < addrs.size() - 1; j++) {
                double old_len, new_len;
                if ((i != 0) && (j != addrs.size() - 1)) {
                    if (man_norm) {
                        old_len = newAddrs.at(i-1).manhattan_dist(newAddrs.at(i)) + newAddrs.at(j).manhattan_dist(newAddrs.at(j+1));
                        new_len = newAddrs.at(i-1).manhattan_dist(newAddrs.at(j)) + newAddrs.at(i).manhattan_dist(newAddrs.at(j+1));
                    } else {
                        old_len = newAddrs.at(i-1).euclidean_dist(newAddrs.at(i)) + newAddrs.at(j).euclidean_dist(newAddrs.at(j+1));
                        new_len = newAddrs.at(i-1).euclidean_dist(newAddrs.at(j)) + newAddrs.at(i).euclidean_dist(newAddrs.at(j+1));
                    }
                } else if ((i == 0) && (j == addrs.size() - 1)) {
                    old_len = 0.0;
                    new_len = 1.0;
                } else if (i == 0) {
                    if (man_norm) {
                        old_len = newAddrs.at(j).manhattan_dist(newAddrs.at(j+1));
                        new_len = newAddrs.at(i).manhattan_dist(newAddrs.at(j+1));
                    } else {
                        old_len = newAddrs.at(j).euclidean_dist(newAddrs.at(j+1));
                        new_len = newAddrs.at(i).euclidean_dist(newAddrs.at(j+1));
                    }
                } else {
                    if (man_norm) {
                        old_len = newAddrs.at(i-1).manhattan_dist(newAddrs.at(i));
                        new_len = newAddrs.at(i-1).manhattan_dist(newAddrs.at(j));
                    } else {
                        old_len = newAddrs.at(i-1).euclidean_dist(newAddrs.at(i));
                        new_len = newAddrs.at(i-1).euclidean_dist(newAddrs.at(j));
                    }
                }
                if (old_len - new_len > 0) {
                    std::reverse(newAddrs.begin()+i, newAddrs.begin()+j+1);
                    changed = true;
                    goto quadbreak;
                }
            }
        }
        quadbreak:
        ;
    } while (changed);

    Route path(newAddrs.front(), newAddrs.back());
    for (Address ad : newAddrs) {
        path.add_address(ad);
    }
    return path;
}
