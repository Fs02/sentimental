#include <sentimental/transform/money.h>

using namespace sm::transform;

std::string Money::operator ()(const std::string &token)
{
    if (token[0] == '$')
        return "{{money}}";

    return token;
}
