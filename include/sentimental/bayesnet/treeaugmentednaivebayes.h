#ifndef SENTIMENTAL_BAYESNET_TREEAUGMENTEDNAIVEBAYES_H
#define SENTIMENTAL_BAYESNET_TREEAUGMENTEDNAIVEBAYES_H

#include <sentimental/feature.h>
#include <sentimental/bayesnet.h>
#include <functional>

namespace sm {
namespace bayesnet {

// References:
// - https://web.archive.org/web/20160327123310/http://www.cs.technion.ac.il/~dang/journal_papers/friedman1997Bayesian.pdf
// - https://arxiv.org/pdf/1301.6684.pdf
// - https://www.slideshare.net/vangjee/a-quick-introduction-to-the-chow-liu-algorithm
// - http://www.cs.cmu.edu/~guestrin/Class/15781/recitations/r10/11152007chowliu.pdf

class TreeAugmentedNaiveBayes
{
public:
    typedef std::function<void(experimental::BayesNet&, const TermDocFeature&)> Estimate;

    TreeAugmentedNaiveBayes(const Estimate &estimate);

    experimental::BayesNet operator ()(const TermDocFeature &feature);

    inline double debug() const { return debug_; }
    inline void set_debug(bool flag) { debug_ = flag; }
private:
    Estimate estimate_;
    bool debug_;
};

}
}

#endif
