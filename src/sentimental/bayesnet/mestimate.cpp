#include <sentimental/bayesnet/mestimate.h>
#include <sentimental/bayesnet/observefrequency.h>
#include <algorithm>

using namespace sm::bayesnet;

MEstimate::MEstimate(double smoothing)
    : smoothing_(smoothing), replace_(true)
{}

void MEstimate::operator ()(experimental::BayesNet &bayesnet, const TermDocFeature &feature)
{
    for (std::size_t child = 0; child < bayesnet.graph().vertex_count(); ++child)
    {
        if (!replace_ && !bayesnet.cpt().values(child).empty())
            continue;

        std::vector<double> values;
        auto adjacents = bayesnet.graph().adjacents(child);
        auto it = std::find(adjacents.begin(), adjacents.end(), 0);
        if(it != adjacents.end())
            adjacents.erase(it);

        ObserveFrequency n(child, adjacents, bayesnet.graph(), bayesnet.cpt(), feature);

        if (child == 0)
        {
            for (std::size_t c = 0; c < n.s(); ++c)
                values.push_back((1 + n.ic(c))/double(n.s()+n.i()));
        }
        else
        {
            for (std::size_t c = 0; c < n.s(); ++c)
            {
                for (std::size_t j = 0; j < n.q(); ++j)
                {
                    for (std::size_t k = 0; k < n.r(); ++k)
                    {
                        values.push_back((1 + n.ijck(j, c, k))/double(n.s() + n.ijc(j, c)));
                    }
                }
            }
        }

        bayesnet.cpt().set_values(child, values);
    }
}
