#include <sentimental/transform/url.h>

using namespace sm::transform;

std::string Url::operator ()(const std::string &token)
{
    if (token.size() > 4 && token.substr(0, 4) == "http")
        return "{{url}}";

    return token;
}
