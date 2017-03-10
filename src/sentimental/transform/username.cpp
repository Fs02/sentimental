#include <sentimental/transform/username.h>

using namespace sm::transform;

std::string Username::operator ()(const std::string &token)
{
    if (token[0] == '@')
        return "{{username}}";

    return token;
}
