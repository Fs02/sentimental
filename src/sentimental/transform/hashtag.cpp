#include <sentimental/transform/hashtag.h>

using namespace sm::transform;

Hashtag::Hashtag(std::size_t max)
    : max_(max)
{}

std::string Hashtag::operator ()(const std::string &token, const WordCount &count)
{
    if (token[0] != '#')
        return token;

    if (count[token] > max_)
        return token;

    return "{{hashtag}}";
}
