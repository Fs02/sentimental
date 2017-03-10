#ifndef SENTIMENTAL_TRANSFORM_APOSTROPHE_H
#define SENTIMENTAL_TRANSFORM_APOSTROPHE_H

#include <string>
#include <unordered_map>

namespace sm {
    namespace transform {
        class Apostrophe
        {
        public:
            std::string operator ()(const std::string &token);

            static std::unordered_map<std::string, std::string> apostrophes;
            static std::unordered_map<std::string, std::string> specials;
        };
    }
}

#endif
