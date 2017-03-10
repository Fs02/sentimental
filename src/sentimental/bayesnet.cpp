#include <sentimental/bayesnet.h>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace sm;


experimental::BayesNet::BayesNet()
    : graph_(), cpt_()
{}

experimental::BayesNet::BayesNet(const TermDocFeature &feature, const std::string &label, bool as_naive_bayes)
    : graph_(), cpt_()
{
    graph_.add_vertex(label);
    for (auto term : feature.get().storage())
        graph_.add_vertex(term.first);

    if (as_naive_bayes)
    {
        for (std::size_t i = 1; i < graph_.vertex_count(); ++i)
            graph_.add_adjacent(i, 0);
    }

    cpt_ = bayesnet::Cpt(graph_, {"F", "T"});

    // set label states
    std::size_t label_id = graph_.vertex(label);
    auto states = std::set<std::string>(feature.labels().begin(), feature.labels().end());
    cpt_.set_states(label_id, std::vector<std::string>(states.begin(), states.end()));
}

std::string experimental::BayesNet::predict(const std::string &text) const
{
    std::stringstream ss(text);
    std::string token;
    std::unordered_map<std::size_t, std::size_t> vars;

    while(std::getline(ss, token, ' '))
    {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        std::size_t id = graph_.vertex(token);
        if (graph_.valid_vertex(id))
            vars[id] = 1;
    }

    return cpt_.state(predict(vars));
}

std::size_t experimental::BayesNet::predict(const std::unordered_map<std::size_t, std::size_t> &vars) const
{
    std::vector<std::size_t> labels = cpt_.states(0);
    std::unordered_map<std::size_t, double> log_likelihood;
    for (std::size_t id = 0; id < graph_.vertex_count(); ++id)
    {
        std::vector<std::size_t> joint = graph_.adjacents(id);
        joint.push_back(id);

        std::size_t stride = 1;
        std::unordered_map<std::size_t, std::size_t> index;
        for (auto it = joint.crbegin(); it != joint.crend(); ++it)
        {
            auto outcomes = cpt_.states(*it);
            if (*it != 0)
            {
                std::size_t value = 0;
                auto var = vars.find(*it);
                if (var != vars.end())
                    value = var->second;

                std::size_t pos = std::find(outcomes.cbegin(), outcomes.cend(), value) - outcomes.cbegin();
                if(pos >= outcomes.size())
                    pos = 0;

                for (auto label : labels)
                    index[label] += pos * stride;
            }
            else
            {
                for (auto label : labels)
                {
                    std::size_t value = label;

                    std::size_t pos = std::find(outcomes.cbegin(), outcomes.cend(), value) - outcomes.cbegin();
                    if(pos >= outcomes.size())
                        pos = 0;

                    index[label] += pos * stride;
                }
            }

            stride *= outcomes.size();
        }
        for (auto label : labels)
            log_likelihood[label] += std::log(cpt_.values(id)[index[label]]);
    }

    std::size_t label = 0;
    double best = -std::numeric_limits<double>::infinity();;
    for (auto ll : log_likelihood)
    {
        if (ll.second > best)
        {
            label = ll.first;
            best = ll.second;
        }
    }
    return label;
}

double experimental::BayesNet::joint_log_likelihood(const std::unordered_map<std::string, std::string> &vars) const
{
    std::unordered_map<std::size_t, std::size_t> vars_;
    for (auto v : vars)
        vars_[graph_.vertex(v.first)] = cpt_.state(v.second);

    return joint_log_likelihood(vars_);
}

double experimental::BayesNet::joint_log_likelihood(const std::unordered_map<std::size_t, std::size_t> &vars) const
{
    double probability = 0.0;
    for (std::size_t id = 0; id < graph_.vertex_count(); ++id)
    {
        std::vector<std::size_t> joint = graph_.adjacents(id);
        joint.push_back(id);
        std::size_t stride = 1;
        std::size_t index = 0;
        for (auto it = joint.crbegin(); it != joint.crend(); ++it)
        {
            std::size_t value = 0;
            auto var = vars.find(*it);
            if (var != vars.end())
                value = var->second;

            auto outcomes = cpt_.states(*it);
            std::size_t pos = std::find(outcomes.cbegin(), outcomes.cend(), value) - outcomes.cbegin();
            if(pos >= outcomes.size())
                pos = 0;

            index += pos * stride;
            stride *= outcomes.size();
        }
        probability += std::log(cpt_.values(id)[index]);
    }
    return probability;
}

bool experimental::BayesNet::add_node(const std::string &name)
{
    if (!graph_.add_vertex(name))
        return false;

    std::size_t id = graph_.vertex(name);
    return cpt_.add_table(id);
}

bool experimental::BayesNet::rem_node(const std::string &name)
{
    std::size_t id = graph_.vertex(name);
    if (!graph_.valid_vertex(id))
        return false;

    graph_.rem_vertex(id);
    return cpt_.rem_table(id);
}

bool experimental::BayesNet::add_arc(const std::string &parent, const std::string &child)
{
    std::size_t from = graph_.vertex(child);
    if (!graph_.valid_vertex(from))
        return false;

    std::size_t to = graph_.vertex(parent);
    if (!graph_.valid_vertex(to))
        return false;

    return graph_.add_adjacent(from, to) && cpt_.clear_values(from);
}

bool experimental::BayesNet::rem_arc(const std::string &parent, const std::string &child)
{
    std::size_t from = graph_.vertex(child);
    if (!graph_.valid_vertex(from))
        return false;

    std::size_t to = graph_.vertex(parent);
    if (!graph_.valid_vertex(to))
        return false;

    return graph_.rem_adjacent(from, to) && cpt_.clear_values(from);
}

bool experimental::BayesNet::set_states(const std::string &name, const std::vector<std::string> &states)
{
    std::size_t id = graph_.vertex(name);
    if (!graph_.valid_vertex(id))
        return false;

    return cpt_.set_states(id, states);
}

bool experimental::BayesNet::set_values(const std::string &name, const std::vector<double> &values)
{
    std::size_t id = graph_.vertex(name);
    if (!graph_.valid_vertex(id))
        return false;

    return cpt_.set_values(id, values);
}

bool experimental::BayesNet::set_probability(const std::string &node, const std::string &state,
                    const std::unordered_map<std::string, std::string> &parent_states, double p)
{
    std::unordered_map<std::size_t, std::size_t> parent_states_;
    for (auto v : parent_states)
    {
        parent_states_[graph_.vertex(v.first)] = cpt_.state(v.second);
    }

    return set_probability(graph_.vertex(node), cpt_.state(state), parent_states_, p);
}

bool experimental::BayesNet::set_probability(std::size_t node, std::size_t state,
                     const std::unordered_map<std::size_t, std::size_t> &parent_states, double p)
{
    // TODO
    std::vector<std::size_t> joint = graph_.adjacents(node);
    joint.push_back(node);
    auto vars = parent_states;
    vars[node] = state;

    std::size_t stride = 1;
    std::size_t index = 0;
    for (auto it = joint.crbegin(); it != joint.crend(); ++it)
    {
        std::size_t value = 0;
        auto var = vars.find(*it);
        if (var != vars.end())
            value = var->second;
        else
            return false;

        auto outcomes = cpt_.states(*it);
        std::size_t pos = std::find(outcomes.cbegin(), outcomes.cend(), value) - outcomes.cbegin();
        if(pos >= outcomes.size())
            return false;

        index += pos * stride;
        stride *= outcomes.size();
    }
    return cpt_.set_value(node, index, p);
}

double experimental::BayesNet::probability(const std::string &node, const std::string &state,
        const std::unordered_map<std::string, std::string> &parent_states)
{
    std::unordered_map<std::size_t, std::size_t> parent_states_;
    for (auto v : parent_states)
    {
        parent_states_[graph_.vertex(v.first)] = cpt_.state(v.second);
    }

    return probability(graph_.vertex(node), cpt_.state(state), parent_states_);
}

double experimental::BayesNet::probability(std::size_t node, std::size_t state,
    const std::unordered_map<std::size_t, std::size_t> &parent_states)
{
    // TODO
    std::vector<std::size_t> joint = graph_.adjacents(node);
    joint.push_back(node);
    auto vars = parent_states;
    vars[node] = state;

    std::size_t stride = 1;
    std::size_t index = 0;
    for (auto it = joint.crbegin(); it != joint.crend(); ++it)
    {
        std::size_t value = 0;
        auto var = vars.find(*it);
        if (var != vars.end())
            value = var->second;
        else
            return false;

        auto outcomes = cpt_.states(*it);
        std::size_t pos = std::find(outcomes.cbegin(), outcomes.cend(), value) - outcomes.cbegin();
        assert(pos < outcomes.size());

        index += pos * stride;
        stride *= outcomes.size();
    }
    return cpt_.value(node, index);
}
