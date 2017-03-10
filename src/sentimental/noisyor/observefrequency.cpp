#include <sentimental/noisyor/observefrequency.h>

using namespace sm::noisyor;

ObserveFrequency::ObserveFrequency(const TermDocFeature &feature)
    : vocab_count_(), label_count_(), term_label_count_()
{
    for (auto term : feature.get().storage())
    {
        std::set<std::string> labels;
        for (auto doc : term.second)
        {
            term_label_count_[term.first][feature.labels()[doc.first]] += doc.second;
            label_count_[feature.labels()[doc.first]] += doc.second; 
            labels.insert(feature.labels()[doc.first]);
        }

        for (auto l : labels)
            vocab_count_[l] += 1;
    }
}

std::size_t ObserveFrequency::term(const std::string &t) const
{
    auto term_it = term_label_count_.find(t);
    if (term_it == term_label_count_.end())
        return 0;

    std::size_t total = 0;
    for (auto label_it : term_it->second)
        total += label_it.second;

    return total;
}

std::size_t ObserveFrequency::term_label(const std::string &t, const std::string &l) const
{
    auto term_it = term_label_count_.find(t);
    if (term_it == term_label_count_.end())
        return 0;
    
    auto label_it = term_it->second.find(l);
    if (label_it == term_it->second.end())
        return 0;
    
    return label_it->second;
}

std::size_t ObserveFrequency::label(const std::string &l) const
{
    auto label_it = label_count_.find(l);
    if (label_it == label_count_.end())
        return 0;

    return label_it->second;
}

std::size_t ObserveFrequency::total() const
{
    std::size_t total = 0;
    for (auto label : label_count_)
        total += label.second;

    return total;
}

std::size_t ObserveFrequency::vocab(const std::string &l) const
{
    auto vocab_it = vocab_count_.find(l);
    if (vocab_it == vocab_count_.end())
        return 0;

    return vocab_it->second;
}

std::size_t ObserveFrequency::vocab() const
{
    std::size_t total = 0;
    for (auto vocab : vocab_count_)
        total += vocab.second;

    return total;
}
