#ifndef SENTIMENTAL_BAYESNET_CPT_H
#define SENTIMENTAL_BAYESNET_CPT_H

#include <sentimental/bayesnet/graph.h>
#include <vector>
#include <set>
#include <unordered_map>

namespace sm {
namespace bayesnet {

class Cpt
{
public:
    Cpt();
    Cpt(const Graph &graph, const std::set<std::string> &default_states);

    bool add_table(std::size_t id);
    bool rem_table(std::size_t id);

    std::vector<std::size_t> states(std::size_t id) const
        { return states_.at(id); }
    bool set_states(std::size_t id, const std::vector<std::string> &states);

    std::vector<double> values(std::size_t id) const
        { return values_.at(id); }
    bool set_values(std::size_t id, const std::vector<double> &values);

    double value(std::size_t id, std::size_t index);
    bool set_value(std::size_t id, std::size_t index, double value);

    bool clear_values(std::size_t id);

    std::size_t state(const std::string &name) const;
    inline std::string state(std::size_t id) const { return state_name_[id]; }
private:
    std::vector<std::string> state_name_;
    std::unordered_map<std::size_t, std::vector<std::size_t>> states_;
    std::unordered_map<std::size_t, std::vector<double>> values_;
};

}
}

#endif
