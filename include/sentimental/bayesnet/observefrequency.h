#ifndef SENTIMENTAL_BAYESNET_OBSERVEFREQUENCY_H
#define SENTIMENTAL_BAYESNET_OBSERVEFREQUENCY_H

#include <sentimental/bayesnet/graph.h>
#include <sentimental/bayesnet/cpt.h>
#include <sentimental/feature.h>
#include <vector>

namespace sm {
namespace bayesnet {

class ObserveFrequency
{
public:
    ObserveFrequency(std::size_t child,
                      const std::vector<std::size_t> &parents,
                      const Graph &graph,
                      const Cpt &cpt,
                      const TermDocFeature &feature,
                      std::size_t label = 0,
                      std::size_t true_state = 1,
                      std::size_t false_state = 0);

    inline std::size_t q() const { return frequency_[0].second.size(); }
    inline std::size_t r() const { return frequency_[0].second[0].second.size(); }
    inline std::size_t s() const { return frequency_.size(); }

    std::size_t i() const;
    std::size_t ij(std::size_t j) const;
    std::size_t ik(std::size_t k) const;
    std::size_t ijk(std::size_t j, std::size_t k) const;

    std::size_t ic(std::size_t c) const;
    std::size_t ick(std::size_t c, std::size_t k) const;
    std::size_t ijc(std::size_t j, std::size_t c) const;
    std::size_t ijck(std::size_t j, std::size_t c, std::size_t k) const;

private:
    // vector [(c, [(j, [k])]]
    // c : class frequency
    // j : parent frequency
    // k : child frequency
    std::vector<std::pair<std::size_t, std::vector<std::pair<std::size_t, std::vector<std::size_t>>>>> frequency_;
};

}
}

#endif
