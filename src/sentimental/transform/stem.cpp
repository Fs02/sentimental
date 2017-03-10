#include <sentimental/transform/stem.h>
extern "C"
{
    #include <stmr.c/stmr.h>
}

using namespace sm::transform;

std::string Stem::operator ()(const std::string &token)
{
    if (token.size() > 2 && token.substr(0, 2) == "{{" && token.substr(token.size() - 2, 2) == "}}")
        return token;

    std::string result = token;
    int end = stem(&result[0], 0, result.length() - 1);

    return result.substr(0, end + 1);
}
