#include <sentimental/bayesnet/classificationrate.h>

using namespace sm::bayesnet;

ClassificationRate::ClassificationRate(const experimental::BayesNet &bayesnet, const TermDocFeature &feature, const Estimate &estimate)
    : bayesnet_(bayesnet), feature_(feature), estimate_(estimate), labels_(), documents_()
{
    for (auto term : feature.get().storage())
    {
        std::size_t id = bayesnet_.graph().vertex(term.first);
        if (!bayesnet_.graph().valid_vertex(id))
            continue;
        
        for (auto doc : term.second)
            documents_[doc.first][id] = 1; // true_state
    }

    for (auto label : feature.labels())
    {
        labels_.push_back(bayesnet_.cpt().state(label));
    }
}

double ClassificationRate::operator ()(const Graph &graph)
{
    // build the model
    bayesnet_.graph() = graph;
    estimate_(bayesnet_, feature_);

    // classify
    std::size_t correct = 0;
    for (auto doc : documents_)
    {
        if (bayesnet_.predict(doc.second) == labels_[doc.first])
            ++correct;
    }

    return correct/double(documents_.size());
}
