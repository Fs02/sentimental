#ifndef SENTIMENTAL_BAYESNET_MUTUALINFORMATION_H
#define SENTIMENTAL_BAYESNET_MUTUALINFORMATION_H

#include <sentimental/feature.h>
#include <sentimental/frequency.h>
#include <sentimental/bayesnet/graph.h>
#include <sentimental/bayesnet/cpt.h>

namespace sm {
namespace bayesnet {

// conditional mutual information I(X; Y|Class)
double cond_mutual_information(const Graph &graph, const Cpt &cpt, const TermDocFeature &feature, std::size_t x, std::size_t y);

// mutual information I(X; Y)
double mutual_information(const Graph &graph, const Cpt &cpt, const TermDocFeature &feature, std::size_t x, std::size_t y);

double mutual_information(const TermDocFeature &feature, const std::string &x, const std::string &y, const std::vector<std::string> &conditions = {});

}
}


#endif
