#ifndef SENTIMENTAL_METRICS_ACCURACY_H
#define SENTIMENTAL_METRICS_ACCURACY_H

#include <vector>
#include <string>

namespace sm {
    namespace metrics {
        double accuracy_score(const std::vector<std::string> &truth, const std::vector<std::string> &pred);
    }
}

#endif
