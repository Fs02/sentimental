#include <sentimental/transform/number.h>

using namespace sm::transform;

std::string Number::operator ()(const std::string &token)
{
    char* p;
    strtod(token.c_str(), &p);
    if (*p)
        return token;
    else
        return "{{number}}";
}
