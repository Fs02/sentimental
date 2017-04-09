#ifndef SENTIMENTAL_TRANSFORM_BLACKLIST_H
#define SENTIMENTAL_TRANSFORM_BLACKLIST_H

#include <string>
#include <unordered_set>

namespace sm {
    namespace transform {
        class Blacklist
        {
        public:
            std::string operator ()(const std::string &token);

            static std::unordered_set<std::string> list;
        };
    }
}

#endif
