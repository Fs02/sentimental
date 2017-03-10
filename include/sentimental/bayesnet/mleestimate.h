#ifndef SENTIMENTAL_BAYESNET_MLEESTIMATE_H
#define SENTIMENTAL_BAYESNET_MLEESTIMATE_H

#include <sentimental/bayesnet.h>
#include <sentimental/feature.h>

namespace sm {
namespace bayesnet {

class MleEstimate
{
public:
    MleEstimate(double alpha = 1.0);

    void operator ()(experimental::BayesNet &bayesnet, const TermDocFeature &feature);

    inline bool replace() const { return replace_; }
    inline void set_replace(bool flag) { replace_ = flag; }
private:
    double alpha_;
    bool replace_;
};

}
}

#endif
