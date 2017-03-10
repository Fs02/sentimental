#ifndef SENTIMENTAL_BAYESNET_MESTIMATE_H
#define SENTIMENTAL_BAYESNET_MESTIMATE_H

#include <sentimental/bayesnet.h>
#include <sentimental/feature.h>

namespace sm {
namespace bayesnet {

// References:
// - http://ieeexplore.ieee.org/document/1181210

class MEstimate
{
public:
    MEstimate(double smoothing = 1.0);

    void operator ()(experimental::BayesNet &bayesnet, const TermDocFeature &feature);

    inline bool replace() const { return replace_; }
    inline void set_replace(bool flag) { replace_ = flag; }
private:
    double smoothing_;
    bool replace_;
};

}
}

#endif
