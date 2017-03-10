#ifndef SENTIMENTAL_BAYESNET_FCLL_H
#define SENTIMENTAL_BAYESNET_FCLL_H

#include <sentimental/bayesnet/observefrequency.h>
#include <sentimental/bayesnet/graph.h>
#include <list>

namespace sm {
namespace bayesnet {

class fCLL
{
public:
    fCLL(const Cpt &cpt,
         const TermDocFeature &feature,
         std::size_t cache_size = 5,
         std::size_t label = 0,
         std::size_t true_state = 1,
         std::size_t false_state = 0);

    double operator ()(const Graph &graph);

private:
    bool ret_cache(const std::size_t &child,
                   const std::set<std::size_t> &parents,
                   double &firstterm, double &secondterm);

    void put_cache(const std::size_t &child,
                   const std::set<std::size_t> &parents,
                   double firstterm, double secondterm);

    const Cpt &cpt_;
    const TermDocFeature &feature_;
    std::size_t cache_size_;
    std::unordered_map<std::size_t, std::list<std::pair<std::set<std::size_t>, std::pair<double, double>>>> cache_;
    std::size_t label_;
    std::size_t true_state_;
    std::size_t false_state_;
};

}
}

#endif
