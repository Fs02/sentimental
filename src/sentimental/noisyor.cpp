#include <sentimental/noisyor.h>
#include <sentimental/noisyor/observefrequency.h>
#include <algorithm>
#include <cmath>

using namespace sm;

NoisyOr::NoisyOr(const TermDocFeature &feature, bool mle_parameter)
    : graph_(), weight_(), label_vertices_()
{
    noisyor::ObserveFrequency n(feature);
    // add label nodes
    auto labels = std::set<std::string>(feature.labels().begin(), feature.labels().end());
    for (auto label : labels)
    {
        graph_.add_vertex(label);
        std::size_t vertex = graph_.vertex(label);
        label_vertices_[vertex] = label;
    }

    // add term nodes and it's arc
    for (auto term : feature.get().storage())
    {
        graph_.add_vertex(term.first);
        for (auto doc : term.second)
        {
            std::size_t parent = graph_.vertex(term.first);
            std::size_t child = graph_.vertex(feature.labels()[doc.first]);

            graph_.add_adjacent(child, parent);
        }
    }

    // learn weight
    for (auto label : label_vertices_)
    {
        for (auto term_id : graph_.adjacents(label.first))
        {
            auto term = graph_.vertex(term_id);
            double weight = 0.0;
            
            if (mle_parameter)
            {
                weight = (n.term_label(term, label.second) + 1) / double(n.term(term) + 2);
            }
            else
            {
                weight = n.term_label(term, label.second) / double(n.vocab(label.second) * n.term(term));
                for (auto other_term_id : graph_.adjacents(label.first))
                {
                    if (term_id == other_term_id)
                        continue;

                    auto other_term = graph_.vertex(other_term_id);
                    weight *= (n.label(label.second) - n.term_label(term, label.second)) * n.total()
                        / double((n.total() - n.term(other_term)) * n.label(label.second));
                }
            }

            weight_.set(label.first, term_id, weight);
        }
    }
}

std::string NoisyOr::predict(const std::string &text) const
{
    std::stringstream ss(text);
    std::string token;
    std::unordered_map<std::size_t, std::size_t> vars;

    while(std::getline(ss, token, ' '))
    {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        std::size_t id = graph_.vertex(token);
        if (graph_.valid_vertex(id))
            vars[id] += 1;
    }

    return label_vertices_.at(predict(vars));
}

std::size_t NoisyOr::predict(const std::unordered_map<std::size_t, std::size_t> &vars) const
{
    double best_prob = 0.0;
    std::size_t best_label = 0;
    for (auto label : label_vertices_)
    {
        double prob = posterior(label.first, vars);
        if (prob >= best_prob)
        {
            best_prob = prob;
            best_label = label.first;
        }
    }
    return best_label;
}

double NoisyOr::posterior(std::size_t node, const std::unordered_map<std::size_t, std::size_t> &vars) const
{
    double factor = 1.0;
    for (auto parent : graph_.adjacents(node))
    {
        auto it = vars.find(parent);
        if (it != vars.end())
            factor *= std::pow(1.0 - weight_(node, parent), it->second);
    }
    return 1.0 - factor;
}
