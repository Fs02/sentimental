#include <sentimental/wordcount.h>
#include <sstream>
#include <algorithm>

using namespace sm;

WordCount::WordCount()
{}

WordCount::WordCount(const std::vector<std::string> &texts)
{
    for (auto it = texts.begin(); it != texts.end(); ++it)
    {
        operator << (*it);
    }
}

WordCount::WordCount(const std::string &text)
{
    operator << (text);
}

std::size_t WordCount::operator [](const std::string &word) const
{
    auto word_ = word;
    std::transform(word_.begin(), word_.end(), word_.begin(), ::tolower);

    auto it = counts_.find(word_);
    if (it != counts_.end())
        return it->second;

    return 0;
}

WordCount &WordCount::operator <<(const std::string &text)
{
    std::stringstream ss(text);
    std::string token;

    while(std::getline(ss, token, ' '))
    {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        counts_[token] += 1;
    }

    return *this;
}

std::unordered_map<std::string, std::size_t> &WordCount::container()
{
    return counts_;
}

const std::unordered_map<std::string, std::size_t> &WordCount::container() const
{
    return counts_;
}
