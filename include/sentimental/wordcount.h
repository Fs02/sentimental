#ifndef SENTIMENTAL_WORDCOUNT_H
#define SENTIMENTAL_WORDCOUNT_H

#include <vector>
#include <unordered_map>

namespace sm {
    class WordCount
    {
    public:
        WordCount();
        WordCount(const std::vector<std::string> &texts);
        WordCount(const std::string &text);

        std::size_t operator [](const std::string &word) const;

        WordCount &operator <<(const std::string &text);

        std::unordered_map<std::string, std::size_t> &container();
        const std::unordered_map<std::string, std::size_t> &container() const;

    private:
        std::unordered_map<std::string, std::size_t> counts_;
    };
}

#endif
