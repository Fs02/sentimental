#include <sentimental/bayesnet/bdeu.h>
#include <sentimental/bayesnet/observefrequency.h>
#include <cmath>
#include <limits>
#include <cassert>

using namespace sm::bayesnet;

BDeu::BDeu(const Cpt &cpt,
           const TermDocFeature &feature,
           double alpha,
           std::size_t cache_size,
           std::size_t label,
           std::size_t true_state,
           std::size_t false_state)
    : cpt_(cpt), feature_(feature), alpha_(alpha), cache_size_(cache_size),
      label_(label), true_state_(true_state), false_state_(false_state)
{}

double BDeu::operator ()(const Graph &graph)
{
    double score = 0.0;

    // for every nodes
    for (std::size_t child = 0; child < graph.vertex_count(); ++child)
    {
        // retrieve cache if availables
        std::set<std::size_t> parent_set(graph.adjacents(child).cbegin(),
                                         graph.adjacents(child).cend());
        double node_score = 0.0;

        if (ret_cache(child, parent_set, node_score))
        {
            score += node_score;
            continue;
        }

        ObserveFrequency n(child, graph.adjacents(child),
                           graph, cpt_, feature_, label_,
                           true_state_, false_state_);

        std::size_t q_i = n.q();
        for (std::size_t j = 0; j < q_i; ++j)
        {
            double n_ij = n.ij(j);
            node_score +=
                std::lgamma(alpha_/double(q_i)) -
                std::lgamma((alpha_/double(q_i))+n_ij);

            std::size_t r_i = n.r();
            for (auto k = 0; k < r_i; ++k)
            {
                double n_ijk = std::max(double(n.ijk(j, k)), std::numeric_limits<double>::epsilon());
                node_score +=
                    std::lgamma((alpha_/double(r_i*q_i))+n_ijk) -
                    std::lgamma(alpha_/double(r_i*q_i));
            }
        }
        score += node_score;

        // save cache
        put_cache(child, parent_set, node_score);
    }

    assert (score<0.0);

    return score;
}

bool BDeu::ret_cache(const std::size_t &child,
                     const std::set<std::size_t> &parents,
                     double &score)
{
    for (auto item : cache_[child])
    {
        if (item.first == parents)
        {
            score = item.second;
            return true;
        }
    }
    return false;
}

void BDeu::put_cache(const std::size_t &child,
                     const std::set<std::size_t> &parents,
                     double node_score)
{
    std::list<std::pair<std::set<std::size_t>, double>> &items = cache_[child];
    items.push_front(std::make_pair(parents, node_score));
    if (items.size() > cache_size_)
        items.pop_back();
}
