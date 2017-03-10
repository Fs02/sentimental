#include <sentimental/transform/htmlescape.h>

using namespace sm::transform;

std::unordered_map<std::string, char> HTMLEscape::codes = {
    {"amp", '&'},
    {"quot", '"'},
    {"apos", '\''},
    {"lt", '<'},
    {"gt", '>'}
};

std::string HTMLEscape::operator ()(const std::string &token)
{
    std::string result;
    result.reserve(token.size());

    std::string sequence;
    bool is_escape = false;

    for (auto it = token.begin(); it != token.end(); ++it)
    {
        if (*it == '&')
        {
            is_escape = true;
        }
        else if (*it == ';')
        {
            auto code = codes.find(sequence);
            if (code != codes.end())
                result += code->second;
            else
                result.append("&" + sequence + ";");

            sequence.clear();
            is_escape = false;
        }
        else if (is_escape)
        {
            sequence += *it;
        }
        else
        {
            result += *it;
        }
    }

    return result;
}
