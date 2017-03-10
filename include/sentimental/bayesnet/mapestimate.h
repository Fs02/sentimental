#ifndef SENTIMENTAL_BAYESNET_MAPESTIMATE_H
#define SENTIMENTAL_BAYESNET_MAPESTIMATE_H

#include <sentimental/bayesnet.h>
#include <sentimental/feature.h>

namespace sm {
namespace bayesnet {

class MapEstimate
{
public:
    MapEstimate(double alpha);

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
