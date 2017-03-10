#ifndef SENTIMENTAL_SELECTION_CHI2_H
#define SENTIMENTAL_SELECTION_CHI2_H

#include <sentimental/feature.h>
#include <set>

namespace sm {
    namespace selection {
        // TODO: Revisit, probably wrong implementation
        std::set<std::pair<double, std::string>> Chi2(const TermDocFeature &feature);
    }
}

#endif
