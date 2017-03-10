#ifndef SENTIMENTAL_BAYESNET_H
#define SENTIMENTAL_BAYESNET_H

#include <sentimental/feature.h>
#include <sentimental/bayesnet/graph.h>
#include <sentimental/bayesnet/cpt.h>
#include <initializer_list>

namespace sm {
namespace experimental {

class BayesNet
{
public:
    BayesNet();
    BayesNet(const TermDocFeature &feature, const std::string &label = "{{class}}", bool as_naive_bayes = false);

    std::string predict(const std::string &text) const;
    std::size_t predict(const std::unordered_map<std::size_t, std::size_t> &vars) const;

    double joint_log_likelihood(const std::unordered_map<std::string, std::string> &vars) const;
    double joint_log_likelihood(const std::unordered_map<std::size_t, std::size_t> &vars) const;

    bool add_node(const std::string &name);
    bool rem_node(const std::string &name);

    bool add_arc(const std::string &parent, const std::string &child);
    bool rem_arc(const std::string &parent, const std::string &child);

    bool set_states(const std::string &name, const std::vector<std::string> &states);
    bool set_values(const std::string &name, const std::vector<double> &values);

    bool set_probability(const std::string &node, const std::string &state,
                         const std::unordered_map<std::string, std::string> &parent_states, double p);

    bool set_probability(std::size_t node, std::size_t state,
                         const std::unordered_map<std::size_t, std::size_t> &parent_states, double p);

    double probability(const std::string &node, const std::string &state,
                         const std::unordered_map<std::string, std::string> &parent_states);

    double probability(std::size_t node, std::size_t state,
                         const std::unordered_map<std::size_t, std::size_t> &parent_states);

    inline bayesnet::Graph &graph() { return graph_; }
    inline const bayesnet::Graph &graph() const { return graph_; }

    inline bayesnet::Cpt &cpt() { return cpt_; }
    inline const bayesnet::Cpt &cpt() const { return cpt_; }

private:
    bayesnet::Graph graph_;
    bayesnet::Cpt cpt_;
};

}
}

#endif
