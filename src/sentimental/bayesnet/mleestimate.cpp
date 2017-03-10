#include <sentimental/bayesnet/mleestimate.h>
#include <sentimental/frequency.h>

using namespace sm::bayesnet;

MleEstimate::MleEstimate(double alpha)
    : alpha_(alpha), replace_(true)
{}

void MleEstimate::operator ()(experimental::BayesNet &bayesnet, const TermDocFeature &feature)
{
    for (std::size_t child = 0; child < bayesnet.graph().vertex_count(); ++child)
    {
        if (!replace_ && !bayesnet.cpt().values(child).empty())
            continue;

        std::vector<double> values;

        const std::string child_tag = bayesnet.graph().vertex(child);
        std::vector<std::string> parents;
        std::vector<std::string> parents_child;
        parents_child.push_back(child_tag);
        // parent order is reversed in cpt container, see: bayesnet.cpp
        auto adjacents = bayesnet.graph().adjacents(child);
        for (auto it = adjacents.crbegin(); it != adjacents.crend(); ++it)
        {
            std::size_t vertex_id = *it;
            std::string vertex = bayesnet.graph().vertex(vertex_id);
            parents.push_back(vertex);
            parents_child.push_back(vertex);
        }

        Frequency f(feature, parents_child);

        auto pa_i = f.permutate(parents);

        for (std::size_t j = 0; j < pa_i.size(); ++j)
        {
            std::unordered_map<std::string, std::size_t> vars;
            for (std::size_t p = 0; p < pa_i[j].size(); ++p)
                vars[parents[p]] = pa_i[j][p];

            double denom = f(vars) + alpha_;

            for (std::size_t k = 0; k < f.arity(child_tag); ++k)
            {
                vars[child_tag] = k;
                values.push_back((f(vars) + alpha_) / denom);
            }
        }

        bayesnet.cpt().set_values(child, values);
    }
}
