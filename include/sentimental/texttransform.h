#ifndef SENTIMENTAL_TEXTTRANSFORM_H
#define SENTIMENTAL_TEXTTRANSFORM_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <sentimental/wordcount.h>

namespace sm {
    class TextTransform
    {
    public:
        typedef std::function<std::string(const std::string&)> StringTransform;
        typedef std::function<std::string(const std::string&, const WordCount&)> CountTransform;

        std::vector<std::string> operator ()(const std::vector<std::string> &texts);
        std::string operator ()(const std::string &text);

        static std::string transform(const std::string &text, const std::vector<StringTransform> &transformer);
        static std::string transform(const std::string &text, const std::vector<CountTransform> &transformer, const WordCount &count);

        void add(const StringTransform &transform);
        void add(const CountTransform &transform);

    private:
        std::vector<StringTransform> string_transforms_;
        std::vector<CountTransform> count_transforms_;
        WordCount count_;
    };
}

#include <sentimental/transform/apostrophe.h>
#include <sentimental/transform/emoticon.h>
#include <sentimental/transform/hashtag.h>
#include <sentimental/transform/htmlescape.h>
#include <sentimental/transform/money.h>
#include <sentimental/transform/number.h>
#include <sentimental/transform/punctuation.h>
#include <sentimental/transform/repeats.h>
#include <sentimental/transform/url.h>
#include <sentimental/transform/username.h>
#include <sentimental/transform/rare.h>
#include <sentimental/transform/stem.h>

#endif
