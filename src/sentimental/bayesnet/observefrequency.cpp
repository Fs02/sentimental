#include <sentimental/bayesnet/observefrequency.h>
#include <numeric>
#include <cassert>

using namespace sm::bayesnet;

ObserveFrequency::ObserveFrequency(std::size_t child,
                  const std::vector<std::size_t> &parents,
                  const Graph &graph,
                  const Cpt &cpt,
                  const TermDocFeature &feature,
                  std::size_t label,
                  std::size_t true_state,
                  std::size_t false_state)
{
    // map and cache label index
    std::map<std::string, std::size_t> label_c;
    for (std::size_t i = 0; i < cpt.states(0).size(); ++i)
        label_c[cpt.state(cpt.states(0)[i])] = i;

    // get possible parent instantiation
    std::vector<std::vector<std::size_t>> pa_i;
    pa_i.push_back(std::vector<std::size_t>());
    for (std::size_t i = 0; i < parents.size(); ++i)
    {
        auto states = cpt.states(parents[i]);

        // duplicates current permutation
        std::size_t size = pa_i.size();
        pa_i.reserve(pa_i.size() * states.size());
        for (std::size_t j = 0; j < states.size(); ++j)
        {
            for (std::size_t k = 0; k < size; ++k)
            {
                if (j > 0)
                    pa_i.push_back(pa_i[k]);

                if (pa_i[j*size + k].size() <= i)
                    pa_i[j*size + k].push_back(states[j]);
                else
                    pa_i[j*size + k][i] = states[j];
            }
        }
    }

    // initialize container
    {
        std::vector<std::pair<std::size_t, std::vector<std::size_t>>> f_pa;
        f_pa.assign(pa_i.size(), std::make_pair(0, std::vector<std::size_t>()));
        frequency_.assign(cpt.states(label).size(), std::make_pair(0, f_pa));
    }

    auto child_states = cpt.states(child);

    for (auto doc : feature.get().documents())
    {
        std::vector<bool> match;
        match.assign(pa_i.size(), true);

        // scan documents for N_IJ
        for (std::size_t i = 0; i < parents.size(); ++i)
        {
            if (parents[i] == label)
            {
                for (std::size_t j = 0; j < pa_i.size(); ++j)
                {
                    if (!match[j]) continue;

                    match[j] = cpt.state(pa_i[j][i]) == feature.labels()[doc];
                }
            }
            else
            {
                auto docs = feature.get().storage().find(graph.vertex(parents[i]));
                assert(docs != feature.get().storage().end());
                bool contains = docs->second.find(doc) != docs->second.end();

                for (std::size_t j = 0; j < pa_i.size(); ++j)
                {
                    if (!match[j]) continue;

                    match[j] = (contains && pa_i[j][i] == true_state)
                            || (!contains && pa_i[j][i] == false_state);
                }
            }
        }

        // Now for N_IJK
        std::vector<bool> match_child;
        match_child.assign(child_states.size(), true);
        if (child == label)
        {
            for (std::size_t i = 0; i < child_states.size(); ++i)
            {
                match_child[i] = cpt.state(child_states[i]) == feature.labels()[doc];
            }
        }
        else
        {
            auto docs = feature.get().storage().find(graph.vertex(child));
            assert(docs != feature.get().storage().end());
            bool contains = docs->second.find(doc) != docs->second.end();

            match_child[true_state] = contains;
            match_child[false_state] = !contains;
        }

        // increase counter
        std::size_t c = label_c[feature.labels()[doc]];
        for (std::size_t i = 0; i < pa_i.size(); ++i)
        {
            if (frequency_[c].second[i].second.empty())
                frequency_[c].second[i].second.assign(child_states.size(), 0);

            if (!match[i])
                continue;

            // count N_IJCK
            for (std::size_t j = 0; j < match_child.size(); ++j)
            {
                if (match_child[j])
                {
                    frequency_[c].second[i].second[j] += 1;
                }
            }
        }
    }

    // summation
    for (std::size_t c = 0; c < frequency_.size(); ++c)
    {
        // fill N_IJC
        std::size_t sum_c = 0;
        for (std::size_t j = 0; j < frequency_[c].second.size(); ++j)
        {
            frequency_[c].second[j].first = std::accumulate(
                        frequency_[c].second[j].second.begin(),
                        frequency_[c].second[j].second.end(), 0);
            sum_c += frequency_[c].second[j].first;
        }

        // fill N_IC
        frequency_[c].first = sum_c;
    }
}

std::size_t ObserveFrequency::i() const
{
    std::size_t sum = 0;
    for (std::size_t c = 0; c < s(); ++c)
        sum += frequency_[c].first;

    return sum;
}

std::size_t ObserveFrequency::ij(std::size_t j) const
{
    std::size_t sum = 0;
    for (std::size_t c = 0; c < s(); ++c)
        sum += frequency_[c].second[j].first;

    return sum;
}

std::size_t ObserveFrequency::ik(std::size_t k) const
{
    std::size_t sum = 0;
    for (std::size_t c = 0; c < s(); ++c)
        for (std::size_t j = 0; j < q(); ++j)
            sum += frequency_[c].second[j].second[k];

    return sum;
}

std::size_t ObserveFrequency::ijk(std::size_t j, std::size_t k) const
{
    std::size_t sum = 0;
    for (std::size_t c = 0; c < s(); ++c)
        sum += frequency_[c].second[j].second[k];

    return sum;
}

std::size_t ObserveFrequency::ic(std::size_t c) const
{
    return frequency_[c].first;
}

std::size_t ObserveFrequency::ick(std::size_t c, std::size_t k) const
{
    std::size_t sum = 0;
    for (std::size_t j = 0; j < q(); ++j)
        sum += frequency_[c].second[j].second[k];

    return sum;
}

std::size_t ObserveFrequency::ijc(std::size_t j, std::size_t c) const
{
    return frequency_[c].second[j].first;
}

std::size_t ObserveFrequency::ijck(std::size_t j, std::size_t c, std::size_t k) const
{
    return frequency_[c].second[j].second[k];
}
