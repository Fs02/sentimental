#include <sentimental/naivebayes.h>
#include <sentimental/wordcount.h>
#include <cmath>
#include <limits>

using namespace sm;

NaiveBayes::NaiveBayes()
    : total_documents_(0), ngrams_(1)
{}

void NaiveBayes::train(const TermDocFeature &feature)
{
    // data for word probabilities
    auto storage = feature.get().storage();
    auto labels = feature.labels();
    for (auto w_it = storage.begin(); w_it != storage.end(); ++w_it)
    {
        for (auto c_it = w_it->second.begin(); c_it != w_it->second.end(); ++c_it)
        {
            std::string klass = labels[c_it->first];
            word_count_[w_it->first][klass] += c_it->second;
            document_length_[klass] += c_it->second;
        }
    }

    // data for calculate class priors
    for (auto it = labels.begin(); it != labels.end(); ++it)
    {
        class_count_[*it] += 1;
    }

    total_documents_ += labels.size();
    ngrams_ = feature.get().ngrams();
}

std::string NaiveBayes::predict(const std::string &text) const
{
    std::string klass;
    double klass_prob = -std::numeric_limits<double>::infinity();

    WordCount counter(text, ngrams_);

    for (auto c_it = class_count_.begin(); c_it != class_count_.end(); ++c_it)
    {
        double prob = std::log(prior(c_it->first));
        for (auto w_it = counter.container().begin(); w_it != counter.container().end(); ++w_it)
        {
            prob += std::log(probability(w_it->first, c_it->first)) * w_it->second;
        }
        if (prob > klass_prob)
        {
            klass_prob = prob;
            klass = c_it->first;
        }
    }

    return klass;
}

double NaiveBayes::likelihood(const std::string &text, const std::string &klass, double prior) const
{
    WordCount counter(text, ngrams_);
    double prob = std::log(prior);
    for (auto w_it = counter.container().begin(); w_it != counter.container().end(); ++w_it)
    {
        prob += std::log(probability(w_it->first, klass)) * w_it->second;
    }

    return std::exp(prob);
}


double NaiveBayes::prior(const std::string &klass) const
{
    auto it = class_count_.find(klass);
    if (it == class_count_.end())
        return 0;

    return (double)(it->second)/total_documents_;
}

double NaiveBayes::probability(const std::string &word, const std::string &klass) const
{
    unsigned int word_count = 0;
    auto w_it = word_count_.find(word);
    auto d_it = document_length_.find(klass);

    if (d_it == document_length_.end())
        return 0;

    if (w_it != word_count_.end())
    {
        auto c_w_it = w_it->second.find(klass);
        if (c_w_it != w_it->second.end())
           word_count = c_w_it->second;
    }

    return (double)(word_count + 1)/double(d_it->second + word_count_.size());
}

Json::Value NaiveBayes::serialize(const NaiveBayes &naivebayes)
{
    Json::Value root;

    root["naivebayes"]["total_documents"] = naivebayes.total_documents_;

    for (auto count : naivebayes.class_count_)
        root["naivebayes"]["class_count"][count.first] = count.second;

    for (auto document : naivebayes.document_length_)
        root["naivebayes"]["document_length"][document.first] = document.second;

    for (auto word : naivebayes.word_count_)
    {
        for (auto klass : word.second)
            root["naivebayes"]["word_count"][word.first][klass.first] = klass.second;
    }

    return root;
}

NaiveBayes NaiveBayes::deserialize(const Json::Value &value)
{
    NaiveBayes nb;

    nb.total_documents_ = value["naivebayes"]["total_documents"].asLargestUInt();

    for (auto it = value["naivebayes"]["class_count"].begin(); it != value["naivebayes"]["class_count"].end(); ++it)
        nb.class_count_[it.key().asString()] = it->asLargestUInt();

    for (auto it = value["naivebayes"]["document_length"].begin(); it != value["naivebayes"]["document_length"].end(); ++it)
        nb.document_length_[it.key().asString()] = it->asLargestUInt();

    for (auto it = value["naivebayes"]["word_count"].begin(); it != value["naivebayes"]["word_count"].end(); ++it)
    {
        for (auto c_it = it->begin(); c_it != it->end(); ++c_it)
        {
            nb.word_count_[it.key().asString()][c_it.key().asString()] = c_it->asLargestUInt();
        }
    }

    return nb;
}
