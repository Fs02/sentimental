#include <sentimental/selection/chi2.h>
#include <cmath>

using namespace sm;

std::set<std::pair<double, std::string>> sm::selection::Chi2(const TermDocFeature &feature)
{
    auto labels = feature.labels();
    auto storage = feature.get().storage();

    // get class sum and feature sum
    std::unordered_map<std::string, int> class_sum;
    int N = 0;
    class_sum.reserve(storage.size());
    for (auto w_it = storage.begin(); w_it != storage.end(); ++w_it)
    {
        for (auto d_it = w_it->second.begin(); d_it != w_it->second.end(); ++d_it)
        {
            class_sum[labels[d_it->first]] += d_it->second;
            N += d_it->second;
        }
    }

    // calculate score for each class
    std::set<std::pair<double, std::string>> result;
    for (auto w_it = storage.begin(); w_it != storage.end(); ++w_it)
    {
        double score = 0.0;
        for (auto c_it = class_sum.begin(); c_it != class_sum.end(); ++c_it)
        {
            int observed = 0;
            int word_sum = 0;
            for (auto d_it = w_it->second.begin(); d_it != w_it->second.end(); ++d_it)
            {
                word_sum += d_it->second;
                if (labels[d_it->first] == c_it->first)
                    observed += d_it->second;
            }
            double expected = (double)word_sum * c_it->second / N;
            score += (double)pow(observed - expected, 2) / expected;
        }
        result.insert(std::make_pair(score, w_it->first));
    }
    return result;
}
