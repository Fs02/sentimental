#include <sentimental/transform/repeats.h>

using namespace sm::transform;

std::unordered_set<std::string> Repeats::whitelist = {
    "too", "cool", "tomorrow", "will", "all", "sleep", "cheer", "happiness"
};

std::string Repeats::operator ()(const std::string &token)
{
    if (token.size() > 2 && token.substr(0, 2) == "{{" && token.substr(token.size() - 2, 2) == "}}")
        return token;

    if (whitelist.find(token) != whitelist.end())
        return token;

    std::string result;
    result.reserve(token.size());
    result += token[0];

    for (auto it = token.begin() + 1; it != token.end(); ++it)
    {
        if (*it != *(it-1))
            result += *it;
    }

    return result;
}
