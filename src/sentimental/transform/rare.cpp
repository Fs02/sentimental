#include <sentimental/transform/rare.h>

using namespace sm::transform;
Rare::Rare(std::size_t max, bool remove)
    : max_(max), remove_(remove)
{}

std::string Rare::operator ()(const std::string &token, const WordCount &count)
{
    if (token.size() > 2 && token.substr(0, 2) == "{{" && token.substr(token.size() - 2, 2) == "}}")
        return token;

    if (count[token] > max_)
        return token;

    if (remove_)
        return "";

    return "{{rare}}";
}
