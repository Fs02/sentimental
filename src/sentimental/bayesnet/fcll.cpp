#include <sentimental/bayesnet/fcll.h>
#include <sentimental/bayesnet/loglikelihood.h>
#include <cmath>
#include <limits>

using namespace sm::bayesnet;

fCLL::fCLL(const Cpt &cpt,
           const TermDocFeature &feature,
           std::size_t cache_size,
           std::size_t label,
           std::size_t true_state,
           std::size_t false_state)
    : cpt_(cpt), feature_(feature), cache_size_(cache_size), label_(label),
      true_state_(true_state), false_state_(false_state)
{}

double fCLL::operator ()(const Graph &graph)
{
    constexpr double pi = 3.14159265358979323846;

    double alpha = (std::pow(pi, 2) + 6.0)/24.0;
    double beta = (std::pow(pi, 2) - 18.0)/24.0;
    double lambda = std::pow(pi, 2)/6.0;

    double firsterm = 0.0; // first decomposable term (loglikelihood)
    double secondterm = 0.0; // 2nd decomposable term of fcllcriterion

    // for every nodes
    for (std::size_t child = 0; child < graph.vertex_count(); ++child)
    {
        // retrieve cache if availables
        std::set<std::size_t> parent_set(graph.adjacents(child).cbegin(),
                                         graph.adjacents(child).cend());

        double node_firstterm = 0.0;
        double node_secondterm = 0.0;
        if (ret_cache(child, parent_set, node_firstterm, node_secondterm))
        {
            firsterm += node_firstterm;
            secondterm += node_secondterm;
            continue;
        }

        ObserveFrequency n(child, graph.adjacents(child),
                           graph, cpt_, feature_, label_,
                           true_state_, false_state_);

        // compute first decomposable term
        node_firstterm = LogLikelihood::d_ll(n);
        firsterm += node_firstterm;

        // compute second decomposable term
        for (std::size_t j = 0; j < n.q(); ++j)
        {
            for (std::size_t k = 0; k < n.r(); ++k)
            {
                for (std::size_t c = 0; c < n.s(); ++c)
                {
                    double epsilon = std::numeric_limits<double>::epsilon();
                    double n_ijck = std::max(double(n.ijck(j, c, k)), epsilon);
                    double n_ij_k = std::max(double(n.ijk(j, k)), epsilon);
                    double n_ijc = std::max(double(n.ijc(j, c)), epsilon);
                    double n_ij_ = std::max(double(n.ij(j)), epsilon);

                    node_secondterm += n_ijck *
                        (std::log(n_ijck/n_ij_k)
                        -std::log(n_ijc/n_ij_));
                }

            }
        }
        secondterm += node_secondterm;

        // save cache
        put_cache(child, parent_set, node_firstterm, node_secondterm);
    }

    return (alpha + beta) * firsterm - beta * lambda * secondterm;
}

bool fCLL::ret_cache(const std::size_t &child,
                     const std::set<std::size_t> &parents,
                     double &firstterm, double &secondterm)
{
    for (auto item : cache_[child])
    {
        if (item.first == parents)
        {
            firstterm = item.second.first;
            secondterm = item.second.second;
            return true;
        }
    }
    return false;
}

void fCLL::put_cache(const std::size_t &child,
                     const std::set<std::size_t> &parents,
                     double firstterm, double secondterm)
{
    std::list<std::pair<std::set<std::size_t>, std::pair<double, double>>> &items = cache_[child];
    items.push_front(std::make_pair(parents, std::make_pair(firstterm, secondterm)));
    if (items.size() > cache_size_)
        items.pop_back();
}
