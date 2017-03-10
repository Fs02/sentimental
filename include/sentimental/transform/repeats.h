#ifndef SENTIMENTAL_TRANSFORM_REPEATS_H
#define SENTIMENTAL_TRANSFORM_REPEATS_H

#include <string>
#include <unordered_set>

namespace sm {
    namespace transform {
        class Repeats
        {
        public:
            std::string operator ()(const std::string &token);

            static std::unordered_set<std::string> whitelist;
        };
    }
}

#endif
