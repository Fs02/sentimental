#include <sentimental/texttransform.h>
#include <sentimental/utility.h>
#include <sstream>

using namespace sm;

std::vector<std::string> TextTransform::operator ()(const std::vector<std::string> &texts)
{
    std::vector<std::string> result;

    for (auto it = texts.begin(); it != texts.end(); ++it)
    {
        result.push_back(transform(*it, string_transforms_));
        count_ << result.back();
    }

    for (auto it = result.begin(); it != result.end(); ++it)
    {
        *it = transform(*it, count_transforms_, count_);
    }

    return result;
}

std::string TextTransform::operator ()(const std::string &text)
{
    auto res = transform(text, string_transforms_);
    count_ << res;
    return transform(res, count_transforms_, count_);
}

std::string TextTransform::transform(const std::string &text, const std::vector<TextTransform::StringTransform> &transformer)
{
    std::string result = text;

    for (auto tr = transformer.begin(); tr != transformer.end(); ++tr)
    {
        std::stringstream ss(result);
        std::string token;
        result.clear();

        while(std::getline(ss, token, ' '))
        {
            token = trim(token);

            if (token == "")
                continue;

            result += (*tr)(token) + " ";
        }
    }
    return result;
}

std::string TextTransform::transform(const std::string &text, const std::vector<TextTransform::CountTransform> &transformer, const WordCount &count)
{
    std::string result;
    std::string token;
    std::stringstream ss(text);

    while(std::getline(ss, token, ' '))
    {
        token = trim(token);

        if (token == "")
            continue;

        for (auto tr = transformer.begin(); tr != transformer.end(); ++tr)
        {
            if (token.size() > 2 && token.substr(0, 2) == "{{" && token.substr(token.size() - 2, 2) == "}}")
                break;

            token = (*tr)(token, count);
        }

        result += token + " ";
    }

    return result;
}

void TextTransform::add(const StringTransform &transform)
{
    string_transforms_.push_back(transform);
}

void TextTransform::add(const CountTransform &transform)
{
    count_transforms_.push_back(transform);
}
