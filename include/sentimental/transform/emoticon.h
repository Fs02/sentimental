#ifndef SENTIMENTAL_TRANSFORM_EMOTICON_H
#define SENTIMENTAL_TRANSFORM_EMOTICON_H

#include <string>
#include <unordered_set>

namespace sm {
    namespace transform {
        class Emoticon
        {
        public:
            std::string operator ()(const std::string &token);

            static std::unordered_set<std::string> anger;
            static std::unordered_set<std::string> disgust;
            static std::unordered_set<std::string> fear;
            static std::unordered_set<std::string> joy;
            static std::unordered_set<std::string> sadness;
            static std::unordered_set<std::string> surprise;
        };
    }
}

#endif
