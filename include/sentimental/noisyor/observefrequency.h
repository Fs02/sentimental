#ifndef SENTIMENTAL_NOISYOR_OBSERVEFREQUENCY_H
#define SENTIMENTAL_NOISYOR_OBSERVEFREQUENCY_H

#include <sentimental/feature.h>
#include <sentimental/bayesnet/graph.h>

namespace sm {
namespace noisyor {

class ObserveFrequency
{
public:
    ObserveFrequency(const TermDocFeature &feature);

    std::size_t term(const std::string &t) const;
    std::size_t label(const std::string &l) const;
    std::size_t term_label(const std::string &t, const std::string &l) const;
    std::size_t total() const;

    std::size_t vocab(const std::string &l) const;
    std::size_t vocab() const;

private:
    std::unordered_map<std::string, std::size_t> vocab_count_;
    std::unordered_map<std::string, std::size_t> label_count_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::size_t>> term_label_count_;
};

}
}

#endif
