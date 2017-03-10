#include <sentimental/bayesnet/cpt.h>
#include <algorithm>

using namespace sm::bayesnet;

Cpt::Cpt()
    : state_name_(), states_(), values_()
{}

Cpt::Cpt(const Graph &graph, const std::set<std::string> &default_states)
    : state_name_(default_states.begin(), default_states.end()), states_(), values_()
{
    for (std::size_t id = 0; id < graph.vertex_count(); ++id)
    {
        states_[id] = std::vector<std::size_t>();
        values_[id] = std::vector<double>();
        for (std::size_t state_id = 0; state_id < state_name_.size(); ++state_id)
            states_[id].push_back(state_id);
    }
}

bool Cpt::add_table(std::size_t id)
{
    if (states_.find(id) != states_.end())
        return false;

    states_[id] = std::vector<std::size_t>();
    values_[id] = std::vector<double>();
    return true;
}

bool Cpt::rem_table(std::size_t id)
{
    auto state_it = states_.find(id);
    if (state_it == states_.end())
        return false;

    states_.erase(state_it);

    auto value_it = values_.find(id);
    if (value_it == values_.end())
        return false;

    values_.erase(value_it);
    return true;
}

bool Cpt::set_states(std::size_t id, const std::vector<std::string> &states)
{
    if (states_.find(id) == states_.end())
        return false;

    states_[id].clear();
    for (auto state : states)
    {
        std::size_t index = state_name_.size();
        auto it = std::find(state_name_.cbegin(), state_name_.cend(), state);
        if (it != state_name_.cend())
            index = std::distance(state_name_.cbegin(), it);
        else
            state_name_.push_back(state);

        states_[id].push_back(index);
    }
    return true;
}

bool Cpt::set_values(std::size_t id, const std::vector<double> &values)
{
    if (values_.find(id) == values_.end())
        return false;

    values_[id] = values;
    return true;
}

double Cpt::value(std::size_t id, std::size_t index)
{
    if (values_.find(id) == values_.end())
        return 0;

    return values_[id][index];
}

bool Cpt::set_value(std::size_t id, std::size_t index, double value)
{
    if (values_.find(id) == values_.end())
        return false;

    if (index >= values_[id].size())
        values_[id].resize(index + 1);

    values_[id][index] = value;
    return true;
}

std::size_t Cpt::state(const std::string &name) const
{
    auto it = std::find(state_name_.cbegin(), state_name_.cend(), name);
    if (it == state_name_.cend())
        return state_name_.size();

    return std::distance(state_name_.cbegin(), it);
}

bool Cpt::clear_values(std::size_t id)
{
    if (values_.find(id) == values_.end())
        return false;

    values_[id].clear();
    return true;
}
