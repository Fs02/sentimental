#ifndef SENTIMENTAL_BAYESNET_LOGLIKELIHOOD_H
#define SENTIMENTAL_BAYESNET_LOGLIKELIHOOD_H

#include <sentimental/bayesnet/observefrequency.h>
#include <sentimental/bayesnet/graph.h>

namespace sm {
namespace bayesnet {

class LogLikelihood
{
public:
    double operator ()(const Graph &graph);

    static double d_ll(const ObserveFrequency &n);
};

}
}

#endif
