#include <sentimental/frequency.h>
#include <set>
#include <list>
#include <cassert>

using namespace sm;

Frequency::Frequency(const TermDocFeature &feature, const std::vector<std::string> variables, const std::string &label_tag)
    : index_(), variables_(), arity_(), count_()
{
    const std::size_t false_state = 0;
    const std::size_t true_state = 1;

    // store labels
    std::vector<std::string> labels;
    {
        auto labels_set = std::set<std::string>(feature.labels().begin(), feature.labels().end());
        labels.assign(labels_set.begin(), labels_set.end());
    }

    // assign variables and stores each arity
    {
        auto variables_set = std::set<std::string>(variables.begin(), variables.end());
        std::size_t index = 0;
        for (auto it = variables_set.begin(); it != variables_set.end(); ++it)
        {
            variables_.push_back(*it);
            arity_.push_back(*it == label_tag ? labels.size() : 2);
            index_[*it] = index;
            ++index;
        }
    }

    // generate permutation
    auto permutation = permutate(variables_);
    count_.resize(permutation.size(), 0);

    // count documents
    for (std::size_t doc = 0; doc < feature.labels().size(); ++doc)
    {
        // initial value
        std::vector<bool> match;
        match.assign(permutation.size(), true);

        // scan documents
        for (std::size_t i = 0; i < variables_.size(); ++i)
        {
            if (variables_[i] == label_tag)
            {
                for (std::size_t j = 0; j < permutation.size(); ++j)
                {
                    if (!match[j]) continue;

                    match[j] = labels[permutation[j][i]] == feature.labels()[doc];
                }
            }
            else
            {
                auto docs = feature.get().storage().find(variables_[i]);
                assert(docs != feature.get().storage().end());
                bool contains = docs->second.find(doc) != docs->second.end();

                for (std::size_t j = 0; j < permutation.size(); ++j)
                {
                    if (!match[j]) continue;

                    if (contains && (permutation[j][i] == true_state))
                        match[j] = true;
                    else if (!contains && (permutation[j][i] == false_state))
                        match[j] = true;
                    else
                        match[j] = false;
                }
            }
        }

        // count matching permutations
        for (std::size_t i = 0; i < permutation.size(); ++i)
        {
            if (match[i])
                count_[i] += 1;
        }
    }
}

std::size_t Frequency::operator ()(const std::unordered_map<std::string, std::size_t> &vars) const
{
    std::size_t stride = 1;
    std::size_t index = 0;
    for (auto it = variables_.rbegin(); it != variables_.rend(); ++it)
    {
        auto var_it = vars.find(*it);

        // missing? marginalize
        if (var_it == vars.end())
        {
            std::size_t sum = 0;
            auto new_vars = vars;
            for (std::size_t i = 0; i < arity(*it); ++i)
            {
                new_vars[*it] = i;
                sum += operator ()(new_vars);
            }
            return sum;
        }

        // find the index
        index += var_it->second * stride;
        stride *= arity(*it);
    }

    return count_[index];
}

std::vector<std::vector<std::size_t>> Frequency::permutate(const std::vector<std::string> variables)
{
    std::vector<std::vector<std::size_t>> permutation;
    permutation.push_back(std::vector<std::size_t>());
    for (std::size_t i = 0; i < variables.size(); ++i)
    {
        std::size_t arity_size = arity(variables[i]);

        std::size_t size = permutation.size();
        permutation.reserve(size * arity_size);

        // duplicates current permutation
        std::vector<std::vector<std::size_t>> new_perm;
        for (std::size_t k = 0; k < size; ++k)
        {
            for (std::size_t j = 0; j < arity_size; ++j)
            {
                new_perm.push_back(permutation[k]);
            }
        }
        permutation = new_perm;

        // fill
        for (std::size_t k = 0; k < size; ++k)
        {
            for (std::size_t j = 0; j < arity_size; ++j)
            {
                permutation[j + k*arity_size].push_back(j);
            }
        }
    }

    return permutation;
}
