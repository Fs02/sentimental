#ifndef SENTIMENTAL_TRANSFORM_NUMBER_H
#define SENTIMENTAL_TRANSFORM_NUMBER_H

#include <string>

namespace sm {
    namespace transform {
        class Number
        {
        public:
            std::string operator ()(const std::string &token);
        };
    }
}

#endif
