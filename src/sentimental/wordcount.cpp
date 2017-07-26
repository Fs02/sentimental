#include <sentimental/wordcount.h>
#include <sentimental/utility.h>
#include <sstream>
#include <algorithm>
#include <list>

using namespace sm;

WordCount::WordCount(std::size_t ngrams)
    : counts_(), ngrams_(ngrams)
{}

WordCount::WordCount(const std::vector<std::string> &texts, std::size_t ngrams)
    : counts_(), ngrams_(ngrams)
{
    for (auto it = texts.begin(); it != texts.end(); ++it)
    {
        operator << (*it);
    }
}

WordCount::WordCount(const std::string &text, std::size_t ngrams)
    : counts_(), ngrams_(ngrams)
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
    std::list<std::string> words;

    while(std::getline(ss, token, ' '))
    {
        token = trim(token);

        if (token == "")
            continue;

        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        words.push_back(token);
        if (words.size() == ngrams_)
        {
            counts_[join(words.begin(), words.end(), std::string(" "))] += 1;
            words.pop_front();
        }
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
