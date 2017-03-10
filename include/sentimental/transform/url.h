#ifndef SENTIMENTAL_TRANSFORM_URL_H
#define SENTIMENTAL_TRANSFORM_URL_H

#include <string>

namespace sm {
    namespace transform {
        class Url
        {
        public:
            std::string operator ()(const std::string &token);
        };
    }
}

#endif
