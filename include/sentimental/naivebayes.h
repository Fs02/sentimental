#ifndef SENTIMENTAL_NAIVEBAYES_H
#define SENTIMENTAL_NAIVEBAYES_H

#include <sentimental/feature.h>

namespace sm {
    class NaiveBayes
    {
    public:
        NaiveBayes();

        void train(const TermDocFeature &feature);
        std::string predict(const std::string &text) const;

        double prior(const std::string &klass) const;
        double probability(const std::string &word, const std::string &klass) const;

        static Json::Value serialize(const NaiveBayes &naivebayes);
        static NaiveBayes deserialize(const Json::Value &value);
    private:
        std::size_t total_documents_;
        std::unordered_map<std::string, std::size_t> class_count_;
        std::unordered_map<std::string, std::size_t> document_length_;
        std::unordered_map<std::string, std::unordered_map<std::string, std::size_t>> word_count_;
    };
}

#endif
