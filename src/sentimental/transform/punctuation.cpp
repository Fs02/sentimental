#include <sentimental/transform/punctuation.h>
#include <cctype>

using namespace sm::transform;

Punctuation::Punctuation(bool alpha_numeric)
    : alpha_numeric_(alpha_numeric)
{}

std::string Punctuation::operator ()(const std::string &token)
{
    if (token.size() > 2 && token.substr(0, 2) == "{{" && token.substr(token.size() - 2, 2) == "}}")
        return token;

    std::string result;
    for (auto it = token.begin(); it != token.end(); ++it)
    {
        if (alpha_numeric_)
        {
            if (isalnum(*it))
                result += *it;
        }
        else
        {
            if (ispunct(*it))
                result += *it;            
        }
    }

    return result;
}
