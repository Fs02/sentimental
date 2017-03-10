#ifndef SENTIMENTAL_TRANSFORM_STEM_H
#define SENTIMENTAL_TRANSFORM_STEM_H

#include <string>

namespace sm {
    namespace transform {
        class Stem
        {
        public:
            std::string operator ()(const std::string &token);
        };
    }
}

#endif
