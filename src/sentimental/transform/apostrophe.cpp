#include <sentimental/transform/apostrophe.h>

using namespace sm::transform;

std::unordered_map<std::string, std::string> Apostrophe::apostrophes = {
    {"n't","not"},
    {"'s","is"},
    {"'m","am"},
    {"'ll", "will"},
    {"'d", "would"},
    {"'ve", "have"},
    {"'re", "are"}
};

std::unordered_map<std::string, std::string> Apostrophe::specials = {
    {"can't","can not"},
    {"won't","will not"}
};

std::string Apostrophe::operator()(const std::string &token)
{
    auto s = specials.find(token);
    if (s != specials.end())
        return s->second;

    if (token.size() >= 3)
    {
        auto a = apostrophes.find(token.substr(token.size() - 3, 3));
        if (a != apostrophes.end())
            return token.substr(0, token.size() - 3) + " " + a->second;
    }

    if (token.size() >= 2)
    {
        auto a = apostrophes.find(token.substr(token.size() - 2, 2));
        if (a != apostrophes.end())
            return token.substr(0, token.size() - 2) + " " + a->second;
    }

    return token;
}
