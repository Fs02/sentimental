#ifndef SENTIMENTAL_WORDCOUNT_H
#define SENTIMENTAL_WORDCOUNT_H

#include <vector>
#include <unordered_map>

namespace sm {
    class WordCount
    {
    public:
        WordCount(std::size_t ngrams = 1);
        WordCount(const std::vector<std::string> &texts, std::size_t ngrams = 1);
        WordCount(const std::string &text, std::size_t ngrams = 1);

        std::size_t operator [](const std::string &word) const;

        WordCount &operator <<(const std::string &text);

        std::unordered_map<std::string, std::size_t> &container();
        const std::unordered_map<std::string, std::size_t> &container() const;

    private:
        std::unordered_map<std::string, std::size_t> counts_;
        std::size_t ngrams_;
    };
}

#endif
