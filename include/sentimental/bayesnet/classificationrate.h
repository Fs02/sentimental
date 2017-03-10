#ifndef SENTIMENTAL_BAYESNET_CLASSIFICATIONRATE_H
#define SENTIMENTAL_BAYESNET_CLASSIFICATIONRATE_H

#include <sentimental/bayesnet.h>
#include <sentimental/feature.h>
#include <functional>
#include <map>

namespace sm {
namespace bayesnet {

class ClassificationRate
{
public:
    typedef std::function<void(experimental::BayesNet&, const TermDocFeature&)> Estimate;

    ClassificationRate(const experimental::BayesNet &bayesnet, const TermDocFeature &feature, const Estimate &estimate);

    double operator ()(const Graph &graph);

private:
    experimental::BayesNet bayesnet_;
    const TermDocFeature &feature_;
    Estimate estimate_;
    std::vector<std::size_t> labels_;
    std::map<std::size_t, std::unordered_map<std::size_t, std::size_t>> documents_;
};


}
}

#endif
