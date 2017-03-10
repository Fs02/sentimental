#include <sentimental/bayesnet/treeaugmentednaivebayes.h>
#include <sentimental/bayesnet/mutualinformation.h>
#include <cmath>
#include <iostream>

using namespace sm::bayesnet;

TreeAugmentedNaiveBayes::TreeAugmentedNaiveBayes(const Estimate &estimate)
    : estimate_(estimate), debug_(false)
{}

sm::experimental::BayesNet TreeAugmentedNaiveBayes::operator ()(const TermDocFeature &feature)
{
    const std::string label_tag = "{{class}}";
    experimental::BayesNet bn(feature, label_tag, true);

    if (debug_)
        std::cout << "=> Calculating weights" << std::endl;
    std::size_t total = std::pow(bn.graph().vertex_count()-1, 2)/2 - (bn.graph().vertex_count()-1)/2;
    std::size_t progress = 1;

    // calculate weight (conditional mutual information)
    std::set<std::pair<double, std::pair<std::size_t, std::size_t>>> weights;
    for (std::size_t i = 1; i < bn.graph().vertex_count()-1; ++i)
    {
        for (std::size_t j = i; j < bn.graph().vertex_count(); ++j)
        {
            if (i == j)
                continue;

            double w = mutual_information(feature, bn.graph().vertex(i), bn.graph().vertex(j), {label_tag});
            weights.insert(std::make_pair(w, std::make_pair(i, j)));

            if (debug_)
                std::cout << "["  << progress << "/" << total << "] Performing conditional mutual information tests\r" << std::flush;
            ++progress;
        }
    }

    if (debug_)
    {
        std::cout << std::endl;
        std::cout << "=> Constructing graph" << std::endl;
    }

    // construct maximum weight spanning tree
    bn.graph().set_max_adjacents(2); // first adjacent is the class, second will be the tree
    for (auto it = weights.rbegin(); it != weights.rend(); ++it)
    {
        if (it->first > 0.0)
            bn.graph().add_adjacent(it->second.first, it->second.second);
    }

    if (debug_)
        std::cout << "=> Estimating parameter" << std::endl;

    estimate_(bn, feature);

    if (debug_)
        std::cout << "=> Done!" << std::endl;
    return bn;
}
