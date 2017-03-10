#include <sentimental/metrics/accuracy.h>

double sm::metrics::accuracy_score(const std::vector<std::string> &truth, const std::vector<std::string> &pred)
{
    std::size_t correct = 0;
    for (std::size_t i = 0; i < truth.size(); ++i)
    {
        if (truth[i] == pred[i])
            ++correct;
    }

    return (double)correct/truth.size();
}
