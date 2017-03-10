#include <sentimental/bayesnet/loglikelihood.h>
#include <cmath>
#include <limits>

using namespace sm::bayesnet;

double LogLikelihood::operator ()(const Graph &graph)
{
    // todo
    return 0.0;
}

double LogLikelihood::d_ll(const ObserveFrequency &n)
{
    double score = 0.0;
    for (std::size_t j = 0; j < n.q(); ++j)
    {
        for (std::size_t k = 0; k < n.r(); ++k)
        {
            double epsilon = std::numeric_limits<double>::epsilon();
            double n_ijk = std::max(double(n.ijk(j, k)), epsilon);
            double n_ij = std::max(double(n.ij(j)), epsilon);
            score += n_ijk * std::log(n_ijk/n_ij);
        }
    }
    return score;
}
