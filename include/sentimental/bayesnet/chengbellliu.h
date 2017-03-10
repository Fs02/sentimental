#ifndef SENTIMENTAL_BAYESNET_CHENGBELLIU_H
#define SENTIMENTAL_BAYESNET_CHENGBELLIU_H

#include <sentimental/feature.h>
#include <sentimental/bayesnet/undirectedgraph.h>

namespace sm {
namespace bayesnet {

class ChengBellLiu
{
public:
    ChengBellLiu();

    UndirectedGraph operator ()(const TermDocFeature &feature, const std::vector<std::string> &nodes);

    static void find_block_set(UndirectedGraph &ugraph, std::size_t node1, std::size_t node2);

    inline double debug() const { return debug_; }
    inline void set_debug(bool flag) { debug_ = flag; }
private:
    bool debug_;
};

}
}

#endif
