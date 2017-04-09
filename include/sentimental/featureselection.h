#ifndef SENTIMENTAL_FEATURESELECTION_H
#define SENTIMENTAL_FEATURESELECTION_H

#include <set>
#include <vector>
#include <functional>
#include <iostream>
#include <limits>
#include <sentimental/feature.h>

namespace sm {
    class FeatureSelection
    {
    public:
        typedef std::function<std::set<std::pair<double, std::string>>(const TermDocFeature&)> Selection;

        explicit FeatureSelection(const TermDocFeature &feature, const Selection &selection);

        TermDocFeature best(std::size_t k);
        TermDocFeature percentage(double percentage);
        TermDocFeature biggest_diff();
        TermDocFeature range(double min = 0.0, double max = std::numeric_limits<double>::infinity());

        friend std::ostream &operator <<(std::ostream &os, const FeatureSelection &fs);

    private:
        const TermDocFeature &feature_;
        std::set<std::pair<double, std::string>> weights_;
    };

    std::ostream &operator <<(std::ostream &os, const FeatureSelection &fs);
}

#include <sentimental/selection/chi2.h>

#endif
